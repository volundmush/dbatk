#include "dbatk/dgscript.h"
#include "dbatk/database.h"
#include "dbatk/commands.h"
#include "dbatk/api.h"
#include "boost/lexical_cast.hpp"
#include "dbatk/gametime.h"
#include "dbatk/components.h"
#include "dbatk/search.h"

namespace dbat {
    std::unordered_map<std::size_t, std::shared_ptr<DgScriptPrototype>> dgScripts{};

    static std::string trimNum(double val) {
        // Given a value, if value is a whole number, return just the integer value
        // as a string, otherwise return the value as a string.
        if(val == std::floor(val)) {
            return std::to_string(static_cast<int64_t>(val));
        }
        return std::to_string(val);
    }

    HasDgVars globalDgVars;
    bool globalDgDirty = false;

    nlohmann::json DgScriptPrototype::serialize() {
        nlohmann::json j;

        j["id"] = id;
        if(!name.empty()) j["name"] = name;
        if(scriptType) j["scriptType"] = scriptType;
        if(triggerType.any()) j["triggerType"] = bitsetToJson(triggerType);
        if(!lines.empty()) j["lines"] = lines;
        if(!arglist.empty()) j["arglist"] = arglist;
        if(narg) j["narg"] = narg;

        return j;
    }

    DgScriptPrototype::DgScriptPrototype(const nlohmann::json &j) {
        if(j.contains("id")) id = j["id"];
        if(j.contains("name")) name = j["name"];
        if(j.contains("scriptType")) scriptType = j["scriptType"];
        if(j.contains("triggerType")) jsonToBitset(j["triggerType"], triggerType);
        if(j.contains("lines")) lines = j["lines"];
        if(j.contains("arglist")) arglist = j["arglist"];
        if(j.contains("narg")) narg = j["narg"];
    }

    static std::optional<Location> parseDgObjectRef(const std::string& val) {
        boost::smatch match;
        if (!boost::regex_search(val, match, obj_regex)) return std::nullopt;
        auto id = std::stoull(match["id"]);
        auto gen = std::stoull(match["gen"]);
        auto obj = getObject(id, gen);
        if(!registry.valid(obj)) return std::nullopt;
        if(match["slash"].matched) {
            std::string room = match["room"];
            if(room.empty()) room = "0";
            return validDestination(obj, room);
        } else {
            Location loc;
            loc.locationType == LocationType::Absolute;
            loc.data = obj;
            return loc;
        }
    }

    std::string serializeDgRef(entt::entity ent) {
        if(registry.any_of<Room>(ent)) {
            auto &room = registry.get<Room>(ent);
            return room.obj.toString() + "/" + std::to_string(room.id);
        } else {
            auto &obj = registry.get<ObjectId>(ent);
            return obj.toString();
        }
    }

    DgVariant makeVar(const std::string& val) {
        if(isDgRef(val)) {
            auto ent = parseDgObjectRef(val);
            if(ent) return *ent;
        }
        return val;
    }

    nlohmann::json HasDgVars::serializeVars() {
        nlohmann::json j = nlohmann::json::array();

        for(auto &[name, v] : vars) {
            if(v.index() == 0) {
                j.push_back({name, std::get<std::string>(v)});
            } else {
                auto loc = std::get<Location>(v);
                j.push_back({name, loc.serialize()});
            }
        }

        return j;
    }

    void HasDgVars::deserializeVars(const nlohmann::json &j) {
        // j is an array of [<string>, <json>] pairs. The first is the name of the variable,
        // the second element is the value of the variable.

        // If the second variable is a json object, it is a serialized Location object.
        // Otherwise, it's a string.
        for(auto &var : j) {
            if(var.is_array() && var.size() == 2) {
                auto name = var[0].get<std::string>();
                if(var[1].is_string()) {
                    setVar(name, var[1].get<std::string>());
                } else if(var[1].is_object()) {
                    setVar(name, Location(var[1]));
                }
            }
        }
    }

    void HasDgVars::setVar(const std::string& name, const DgVariant& var) {
        auto lower = boost::to_lower_copy(name);
        boost::trim(lower);
        vars[lower] = var;
    }

    void HasDgVars::setVar(const std::string& name, const std::string& var) {
        setVar(name, makeVar(var));
    }

    void HasDgVars::setVar(const std::string& name, entt::entity var) {
        Location l;
        l.locationType = LocationType::Absolute;
        l.data = var;
        setVar(name, l);
    }

    bool HasDgVars::hasVar(const std::string &name) {
        auto lower = boost::to_lower_copy(name);
        boost::trim(lower);
        return vars.find(lower) != vars.end();
    }

    std::optional<DgVariant> HasDgVars::getVar(const std::string &name) {
        auto lower = boost::to_lower_copy(name);
        boost::trim(lower);
        auto find = vars.find(lower);
        if(find == vars.end()) return std::nullopt;
        return find->second;
    }

    void HasDgVars::clearVar(const std::string &name) {
        auto lower = boost::to_lower_copy(name);
        boost::trim(lower);
        vars.erase(lower);
    }

    void HasDgVars::clearVars() {
        vars.clear();
    }

    DgScripts::DgScripts(entt::entity ent) {
        this->ent = ent;
    }

    DgScripts::DgScripts(entt::entity ent, const nlohmann::json &j) {
        this->ent = ent;
        if(j.contains("scripts")) {
            // scripts is an array of ids. check for matching dgScripts...
            for(auto& id : j["scripts"]) {
                auto find = dgScripts.find(id);
                if(find != dgScripts.end()) {
                    // ...and add them to the entity
                    auto script = std::make_shared<DgScript>(ent, find->second);
                    scripts.emplace_back(script);
                    dgScriptInstances.insert(script);
                }
            }
        }
        if(j.contains("vars")) {
            deserializeVars(j["vars"]);
        }
    }

    nlohmann::json DgScripts::serialize() {
        nlohmann::json j;

        if(!scripts.empty()) {
            std::vector<std::size_t> ids;
            for(auto& script : scripts) {
                ids.emplace_back(script->prototype->id);
            }
            j["scripts"] = ids;
        }
        if(!vars.empty()) {
            j["vars"] = serializeVars();
        }

        return j;
    }

    std::set<std::shared_ptr<DgScript>> dgScriptInstances;

    static std::size_t matchingQuote(std::string_view str, std::size_t start) {
        // Given a string and a starting index, finding the matching quote (").
        // Return npos if no match is found. We must respect the escaped status of a \
        // character, so we can't just look for the first unescaped quote.

        // If the starting index is out of bounds, return npos
        if(start >= str.size()) return std::string_view::npos;

        // If the starting index is not a quote, return npos
        if(str[start] != '"') return std::string_view::npos;

        bool escaped = false;
        std::size_t current = start + 1;
        while(current < str.size()) {
            if(str[current] == '"' && !escaped) {
                return current;
            }
            escaped = str[current] == '\\';
            current++;
        }
        return std::string_view::npos;
    }

    static std::size_t matchingParen(std::string_view str, std::size_t start) {
        // Similar to matchingQuote, but now we seek an ending paren (). start
        // SHOULD be our starting paren position. If it's not, we return npos.
        // If we encounter an opening quote ", we skip to the matching quote.
        // Additionally, we care about depth. if we encounter another opening paren,
        // we increase depth and decrease upon encountering closing ones until we
        // have our match. (hopefully.) Return npos for any invalid results.

        // If the starting index is out of bounds, return npos
        if(start >= str.size()) return std::string_view::npos;

        // If the starting index is not a paren, return npos
        if(str[start] != '(') return std::string_view::npos;

        std::size_t depth = 1;
        std::size_t current = start + 1;
        while(current < str.size()) {
            if(str[current] == '(') {
                depth++;
            } else if(str[current] == ')') {
                depth--;
                if(depth == 0) {
                    return current;
                }
            } else if(str[current] == '"') {
                current = matchingQuote(str, current);
                if(current == std::string_view::npos) {
                    return std::string_view::npos;
                }
            }
            current++;
        }
        return std::string_view::npos;
    }

    std::size_t matchingPerc(std::string_view str, std::size_t start) {
        // Similar to matchingParen, but now we seek an ending percent sign %.
        // If we encounter an opening paren, skip to the closing paren and keep
        // searching for our %. Return npos on any invalid results.

        // If the starting index is out of bounds, return npos
        if(start >= str.size()) return std::string_view::npos;

        // If the starting index is not a percent sign, return npos
        if(str[start] != '%') return std::string_view::npos;

        std::size_t current = start + 1;
        while(current < str.size()) {
            if(str[current] == '(') {
                current = matchingParen(str, current);
                if(current == std::string_view::npos) {
                    return std::string_view::npos;
                }
            } else if(str[current] == '%') {
                return current;
            }
            current++;
        }
        return std::string_view::npos;
    }

    std::string DgScriptPrototype::getLine(std::size_t line) {
        // Return the line at the given index. If the index is out of bounds,
        // return an empty string.
        if(line >= lines.size()) return "";
        return boost::trim_copy(lines[line]);
    }

    DgScript::DgScript(entt::entity ent, const std::shared_ptr<DgScriptPrototype> &prototype) {
        this->ent = ent;
        this->prototype = prototype;
    }

    void DgScript::reset() {
        // Reset the script to its initial state
        currentLine = 0;
        loops = 0;
        vars.clear();
        state = DgState::DORMANT;
        waiting = 0.0;
        depth.clear();
    }

    DgScripts& DgScript::getHandler() {
        return registry.get<DgScripts>(ent);
    }

    void DgScript::setState(DgState st) {
        this->state = st;
    }

    void DgScript::onHeartbeat(double deltaTime) {
        if(state == DgState::WAITING) {
            waiting -= deltaTime;
            if(waiting <= 0.0) {
                waiting = 0.0;
                execute();
            }
        }
    }

    std::string DgScript::scriptInfo() {
        return fmt::format("Script {} on {} Line {}", prototype->id, serializeDgRef(ent), currentLine+1);
    }

    void DgScript::scriptError(const std::string& msg) {
        auto formatted = fmt::format("{} - {}", scriptInfo(), msg);
        adminAlert(formatted);
        logger->error(formatted);
    }

    int64_t DgScript::execute() {
        try {
            switch(state) {
                case DgState::RUNNING:
                case DgState::ERROR:
                case DgState::DONE:
                case DgState::PURGED:
                    throw DgScriptException(fmt::format("Script called in invalid state: {}", static_cast<int>(state)));
                    break;
                default:
                    break;
            }
            if(prototype->lines.empty()) {
                throw DgScriptException("Script has no lines!");
            }
            setState(DgState::RUNNING);
            auto results = executeBlock(currentLine, prototype->lines.size());
            if(state == DgState::DONE) {
                reset();
            }
            return results;
        } catch(const DgScriptException& e) {
            setState(DgState::ERROR);
            scriptError(e.what());
            return 0;
        }
    }

    int64_t DgScript::executeBlock(std::size_t start, std::size_t end) {
        int64_t retVal = 1;

        currentLine = start;

        while(currentLine < end) {
            if(state != DgState::RUNNING) {
                // something changed our state so we need to bail out.
                return retVal;
            }

            if(loops > 0 && loops % 500 == 0) {
                setState(DgState::WAITING);
                waiting = 0.1;
                return retVal;
            }
            loops++;

            if(loops > 2000) {
                throw DgScriptException("Runaway script halted.");
            }

            auto line = prototype->getLine(currentLine);

            if(line.empty() || boost::istarts_with(line, "*")) {
                // Do nothing! not even a continue!
            }

            // Cover all of the if blocks...
            else if(boost::istarts_with(line, "if ")) {
                depth.emplace_back(DgNest::IF, currentLine);
                if(!executeIf(line.substr(3))) {
                    currentLine = findElseEnd();
                    continue;
                }
            }

            else if(boost::istarts_with(line, "elseif ")) {
                if(depth.empty() || depth.back().first != DgNest::IF) {
                    throw DgScriptException("elseif without if");
                }
                currentLine = findEnd();
            }

            else if(boost::iequals(line, "else") || boost::istarts_with(line, "else ")) {
                if(depth.empty() || depth.back().first != DgNest::IF) {
                    throw DgScriptException("else without if");
                }
                currentLine = findEnd();
            }

            else if(boost::iequals(line, "end") || boost::istarts_with(line, "end ")) {
                if(depth.empty() || depth.back().first != DgNest::IF) {
                    throw DgScriptException("end without if");
                }
                depth.pop_back();
            }

            // Now for the while...
            else if(boost::istarts_with(line, "while ")) {
                depth.emplace_back(DgNest::WHILE, currentLine);
                if(!executeIf(line.substr(6))) {
                    currentLine = findDone();
                    continue;
                }
            }

            // and the switch...
            else if(boost::istarts_with(line, "switch ")) {
                depth.emplace_back(DgNest::SWITCH, currentLine);
                currentLine = findCase(line.substr(7));
                continue;
            }

            else if(boost::iequals(line, "break") || boost::istarts_with(line, "break ")) {
                if(depth.empty() || depth.back().first != DgNest::SWITCH) {
                    throw DgScriptException("break without switch");
                }
                currentLine = findDone();
                continue;
            }

            else if(boost::istarts_with(line, "case ")) {
                if(depth.empty() || depth.back().first != DgNest::SWITCH) {
                    throw DgScriptException("case without switch");
                }
                // fall through behavior...
                continue;
            }

            else if(boost::iequals(line, "done") || boost::istarts_with(line, "done ")) {
                if(depth.empty()) {
                    throw DgScriptException("done without while or switch");
                }
                auto nest = depth.back();
                switch(nest.first) {
                    case DgNest::WHILE:
                        currentLine = nest.second;
                        depth.pop_back();
                        continue;
                    case DgNest::SWITCH:
                        depth.pop_back();
                        break;
                    default:
                        throw DgScriptException("done without while or switch");
                }
            }

            else {
                // This is an action line! Not control flow!
                auto subCmd = varSubstitute(line);
                auto matched = parseCommand(subCmd);

                auto cmd = matched["cmd"];

                if(boost::iequals(cmd, "nop")) {
                    // do nothing.
                }

                else if(boost::iequals(cmd, "return")) {
                    auto arg = matched["args"];
                    if(!arg.empty()) retVal = truthy(arg);
                }

                else if(boost::iequals(cmd, "wait")) {
                    waiting = cmdWait(matched);
                    setState(DgState::WAITING);
                    currentLine++;
                    return retVal;
                }

                else if(boost::iequals(cmd, "global")) {
                    cmdGlobal(matched);
                }

                else if(boost::iequals(cmd, "context")) {
                    cmdContext(matched);
                }

                else if(boost::iequals(cmd, "rdelete")) {
                    cmdRdelete(matched);
                }

                else if(boost::iequals(cmd, "remote")) {
                    cmdRemote(matched);
                }

                else if(boost::iequals(cmd, "set")) {
                    cmdSet(matched);
                }

                else if(boost::iequals(cmd, "unset")) {
                    cmdUnset(matched);
                }

                else if(boost::iequals(cmd, "load")) {
                    cmdLoad(matched);
                }

                else if(boost::iequals(cmd, "attach")) {
                    cmdAttach(matched);
                }

                else if(boost::iequals(cmd, "detach")) {
                    cmdDetach(matched);
                }

                else if(boost::iequals(cmd, "send")) {
                    cmdSend(matched);
                }

                else if(boost::iequals(cmd, "damage")) {
                    cmdDamage(matched);
                }

                else if(boost::iequals(cmd, "echoaround")) {
                    cmdEchoaround(matched);
                }

                else if(boost::iequals(cmd, "asound")) {
                    cmdAsound(matched);
                }

                else if(boost::iequals(cmd, "recho")) {
                    cmdRecho(matched);
                }

                else {
                    if(!executeCommand(ent, subCmd)) {
                        scriptError("Unknown command: " + cmd);
                    }
                }
            }
            currentLine++;

        }
        setState(DgState::DONE);
        return retVal;
    }

    double DgScript::cmdWait(std::unordered_map<std::string, std::string> &matched) {
        auto arg = matched["args"];
        if(arg.empty()) {
            // TODO: add more info here...
            scriptError("wait command requires an argument");
            return 0.0;
        }

        if(boost::istarts_with(arg, "until ")) {
            // valid forms of time are 14:30 and 1430 both of which mean the same thing:
            // we should wait until the game time in time_info is 14:30. if the current time
            // is 14:31, we should wait until tomorrow at 14:30.
            // All times are in 24-hour format, meaning the max is 23:59 and it wraps to 00:00
            // The actual double value we return is in real seconds until then, which we can
            // calculate using the constexpr in gametime.h - timeAccelFactor, which is the
            // multiplier for how fast time passes in the game world. IE: if timeAccelFactor
            // is 60.0, that means that for every real second, 60 seconds pass in game.

            auto time = arg.substr(6);
            boost::trim(time);
            auto colon = time.find(':');

            if(colon == std::string::npos) {
                // no colon, so we assume it's a 4-digit number
                if(time.length() != 4) {
                    scriptError("wait until time must be in the format HH:MM or HHMM");
                    return 0.0;
                }
                auto hour = std::stoi(time.substr(0, 2));
                auto minute = std::stoi(time.substr(2, 2));
                if(hour < 0 || hour > 23 || minute < 0 || minute > 59) {
                    scriptError("wait until time must be in the format HH:MM or HHMM");
                    return 0.0;
                }
                auto now = time_info.hours * 100 + time_info.minutes;
                auto then = hour * 100 + minute;
                if(now >= then) {
                    then += 2400;
                }
                return (then - now) * 60.0 / timeAccelFactor;
            }
            else {
                // colon, so we assume it's in HH:MM format
                if(time.length() != 5) {
                    scriptError("wait until time must be in the format HH:MM or HHMM");
                    return 0.0;
                }
                auto hour = std::stoi(time.substr(0, colon));
                auto minute = std::stoi(time.substr(colon + 1));
                if(hour < 0 || hour > 23 || minute < 0 || minute > 59) {
                    scriptError("wait until time must be in the format HH:MM or HHMM");
                    return 0.0;
                }
                auto now = time_info.hours * 100 + time_info.minutes;
                auto then = hour * 100 + minute;
                if(now >= then) {
                    then += 2400;
                }
                return (then - now) * 60.0 / timeAccelFactor;
            }
        } else {
            // the second possible format of the arguments are %ld %c in sscanf language.
            // IE: An integer followed by a character t or s. If the character is t, then
            // it's in mud hours. If s, it's in real seconds.

            long int time;
            char unit;
            if(sscanf(arg.c_str(), "%ld %c", &time, &unit)) {
                if(unit == 't') {
                    return time * 60.0 / timeAccelFactor;
                } else if(unit == 's') {
                    return time;
                } else {
                    return 0.1;
                }
            } else {
                return 0.1;
            }
        }

    }

    void DgScript::cmdLoad(std::unordered_map<std::string, std::string> &matched) {
        auto arg = matched["args"];
        if(arg.empty()) {
            scriptError("load command requires an argument");
            return;
        }

        // split arg into two sections by the first space.
        auto space = arg.find(' ');
        if(space == std::string::npos) {
            scriptError("load command requires a variable and a target");
            return;
        }

        auto varName = arg.substr(0, space);
        auto targetName = arg.substr(space + 1);
        boost::trim(varName);
        boost::trim(targetName);

        MoveParams params;
        params.moveType = MoveType::Traverse;
        params.traverseType = TraverseType::System;
        params.force = true;

        if(!(boost::iequals(varName, "obj") || boost::iequals(varName, "mob"))) {
            scriptError("load command requires a variable of type obj or mob");
            return;
        }

        if(boost::iequals(varName, "obj")) varName = "item";
        else if(boost::iequals(varName, "mob")) varName = "npc";

        Location l;

        if(auto r = registry.try_get<Room>(ent); r) {
            // No matter what, we're loading into a room.
            l.locationType = LocationType::Area;
            l.x = r->id;
            l.data = r->obj.getObject();
        } else {
            // ent is an Object. Items go in their inventory;
            // NPCs appear in the same location as the object.
            if(boost::iequals(varName, "item")) {
                l.locationType = LocationType::Inventory;
                l.data = ent;
            } else {
                if(auto lo = registry.try_get<Location>(ent); lo) {
                    l = *lo;
                } else {
                    scriptError("load command requires a valid location");
                    return;
                }
            }
        }

        params.dest = l;

        auto protoName = fmt::format("{}:{}", varName, targetName);
        auto proto = prototypes.find(protoName);
        if(proto == prototypes.end()) {
            scriptError(fmt::format("unable to find prototype for {}", protoName));
            return;
        }

        auto obj = proto->second->spawn();
        auto [res, err] = moveTo(obj, params);
        if(!res) {
            scriptError(fmt::format("unable to load {}: {}", protoName, err.value()));
            deleteObject(obj);
            return;
        }

    }

    void DgScript::cmdGlobal(std::unordered_map<std::string, std::string> &matched) {
        // copy the given script var matched["args"] to the global context if it exists.
        // This should be easy.
        auto arg = matched["args"];
        if(arg.empty()) {
            scriptError("global command requires an argument");
            return;
        }

        auto var = getVar(arg);

        if(!var.has_value()) {
            scriptError("global command requires a valid variable");
            return;
        }

        globalDgVars.setVar(arg, var.value());
        globalDgDirty = true;

    }

    void DgScript::cmdContext(std::unordered_map<std::string, std::string> &matched) {
        // Change this->context to arg, if arg is a valid number.
        // In practice, nothing in the old game seems to use this?


    }

    void DgScript::cmdRemote(std::unordered_map<std::string, std::string> &matched) {
        // syntax: remote <variable> <target>
        // Where <variable> is a name of a script var, and <target> is a DgRef like a
        // player or room. The value of the variable will be copied to the target's
        // DgScripts vars if both exist.
        // This is often used in the scripts like: remote tomato %actor.id%

        auto arg = matched["args"];
        if(arg.empty()) {
            scriptError("remote command requires an argument");
            return;
        }

        // split arg into two sections by the first space.
        auto space = arg.find(' ');
        if(space == std::string::npos) {
            scriptError("remote command requires a variable and a target");
            return;
        }

        auto varName = arg.substr(0, space);
        auto targetName = arg.substr(space + 1);
        boost::trim(varName);
        boost::trim(targetName);

        auto parse = parseDgObjectRef(targetName);
        if(!parse) {
            scriptError("remote command requires a valid target");
            return;
        }

        auto var = getVar(varName);
        if(!var.has_value()) {
            scriptError("remote command requires a valid variable");
            return;
        }

        auto &e = parse.value();
        entt::entity target = entt::null;

        if(e.locationType == LocationType::Absolute) {
            target = e.data;
        }
        else if(e.locationType == LocationType::Area) {
            target = e.getRoom();
        }
        if(registry.valid(target)) {
            auto &dg = registry.get_or_emplace<DgScripts>(target, target);
            dg.setVar(varName, var.value());
            setDirty(e.data);
        } else {
            scriptError("remote command requires a valid target");
        }
    }

    void DgScript::cmdRdelete(std::unordered_map<std::string, std::string> &matched) {
        // like cmdRemote, but will delete a variable from the target.
        auto arg = matched["args"];
        if(arg.empty()) {
            scriptError("rdelete command requires an argument");
            return;
        }

        // split arg into two sections by the first space.
        auto space = arg.find(' ');
        if(space == std::string::npos) {
            scriptError("rdelete command requires a variable and a target");
            return;
        }

        auto varName = arg.substr(0, space);
        auto targetName = arg.substr(space + 1);
        boost::trim(varName);
        boost::trim(targetName);

        auto parse = parseDgObjectRef(targetName);
        if(!parse) {
            scriptError("rdelete command requires a valid target");
            return;
        }

        auto &e = parse.value();
        entt::entity target = entt::null;

        if(e.locationType == LocationType::Absolute) {
            target = e.data;
        }
        else if(e.locationType == LocationType::Area) {
            target = e.getRoom();
        }
        if(registry.valid(target)) {
            auto &dg = registry.get_or_emplace<DgScripts>(target, target);
            dg.clearVar(varName);
            setDirty(e.data);
        } else {
            scriptError("rdelete command requires a valid target");
        }
    }

    void DgScript::cmdSet(std::unordered_map<std::string, std::string> &matched) {
        // syntax: <name> <value>

        auto arg = matched["args"];
        if(arg.empty()) {
            scriptError("set command requires an argument");
            return;
        }

        std::string varName, varValue;
        auto space = arg.find(' ');
        if(space == std::string::npos) {
            varName = arg;
            varValue = "1";
        } else {
            varName = arg.substr(0, space);
            varValue = arg.substr(space + 1);
        }

        boost::trim(varName);
        boost::trim(varValue);

        setVar(varName, varValue);
    }

    void DgScript::cmdUnset(std::unordered_map<std::string, std::string> &matched) {
        // syntax: <name>

        auto arg = matched["args"];
        if(arg.empty()) {
            scriptError("unset command requires an argument");
            return;
        }

        clearVar(arg);
    }

    void DgScript::cmdAttach(std::unordered_map<std::string, std::string> &matched) {
        // syntax: <scriptId> <target>
        // Where <scriptId> is the id of a script, and <target> is a DgRef like a player or room.
        // We basically are gonna need to locate the entity, retrieve its DgScripts component, and call
        // addScript(<scriptId>) on it.

        auto arg = matched["args"];
        if(arg.empty()) {
            scriptError("attach command requires an argument");
            return;
        }

        // split arg into two sections by the first space.
        auto space = arg.find(' ');
        if(space == std::string::npos) {
            scriptError("attach command requires a script id and a target");
            return;
        }

        auto scriptId = arg.substr(0, space);
        auto targetName = arg.substr(space + 1);
        boost::trim(scriptId);
        boost::trim(targetName);

        auto parse = parseDgObjectRef(targetName);
        if(!parse) {
            scriptError("attach command requires a valid target");
            return;
        }

        // coerce scriptId to a std::size_t and check if it exists in dgScripts map...
        std::size_t id = 0;
        try {
            id = std::stoull(scriptId);
        } catch(std::exception &e) {
            scriptError("attach command requires a valid script id");
            return;
        }

        if(!dgScripts.count(id)) {
            scriptError("attach command requires a valid script id");
            return;
        }

        auto &e = parse.value();
        entt::entity target = entt::null;

        if(e.locationType == LocationType::Absolute) {
            target = e.data;
        }
        else if(e.locationType == LocationType::Area) {
            target = e.getRoom();
        }
        if(registry.valid(target)) {
            auto &dg = registry.get_or_emplace<DgScripts>(target, target);
            dg.addScript(id);
            setDirty(e.data);
        } else {
            scriptError("attach command requires a valid target");
        }

    }

    void DgScript::cmdDetach(std::unordered_map<std::string, std::string> &matched) {
        // the opposite of cmdAttach. same process, but call removeScript()

        auto arg = matched["args"];
        if(arg.empty()) {
            scriptError("detach command requires an argument");
            return;
        }

        // split arg into two sections by the first space.
        auto space = arg.find(' ');
        if(space == std::string::npos) {
            scriptError("detach command requires a script id and a target");
            return;
        }

        auto scriptId = arg.substr(0, space);
        auto targetName = arg.substr(space + 1);
        boost::trim(scriptId);
        boost::trim(targetName);

        auto parse = parseDgObjectRef(targetName);
        if(!parse) {
            scriptError("detach command requires a valid target");
            return;
        }

        // coerce scriptId to a std::size_t and check if it exists in dgScripts map...
        std::size_t id = 0;
        try {
            id = std::stoull(scriptId);
        } catch(std::exception &e) {
            scriptError("detach command requires a valid script id");
            return;
        }

        if(!dgScripts.count(id)) {
            scriptError("detach command requires a valid script id");
            return;
        }

        auto &e = parse.value();
        entt::entity target = entt::null;

        if(e.locationType == LocationType::Absolute) {
            target = e.data;
        }
        else if(e.locationType == LocationType::Area) {
            target = e.getRoom();
        }
        if(registry.valid(target)) {
            auto &dg = registry.get_or_emplace<DgScripts>(target, target);
            dg.removeScript(id);
            setDirty(e.data);
        } else {
            scriptError("detach command requires a valid target");
        }

    }

    void DgScript::cmdSend(std::unordered_map<std::string, std::string> &matched) {
        // Syntax: <target> <message>
        // <target> could be an ObjectId or a Name.
        auto arg = matched["args"];
        if(arg.empty()) {
            scriptError("send command requires an argument");
            return;
        }

        auto space = arg.find(' ');
        if(space == std::string::npos) {
            scriptError("send command requires a target and a message");
            return;
        }

        auto targetName = arg.substr(0, space);
        auto message = arg.substr(space + 1);
        boost::trim(targetName);
        boost::trim(message);

        Search search(ent);
        search.useAll(false).useId(true);

        if(auto ro = registry.try_get<Room>(ent); ro) {
            // This trigger is running as a Room.
            Location l;
            l.data = ro->obj.getObject();
            l.locationType = LocationType::Area;
            l.x = ro->id;
            search.in(l);
        } else {
            auto loc = registry.try_get<Location>(ent);
            if(loc) {
                search.in(*loc);
            }
        }
        for(auto &e : search.find(targetName)) {
            sendLine(e, message);
        }
    }

    void DgScript::cmdDamage(std::unordered_map<std::string, std::string> &matched) {
        // todo: this inflicts damage. currently unimplemented...
    }

    void DgScript::cmdEchoaround(std::unordered_map<std::string, std::string> &matched) {
// Syntax: <target> <message>
        // <target> could be an ObjectId or a Name.
        auto arg = matched["args"];
        if(arg.empty()) {
            scriptError("send command requires an argument");
            return;
        }

        auto space = arg.find(' ');
        if(space == std::string::npos) {
            scriptError("send command requires a target and a message");
            return;
        }

        auto targetName = arg.substr(0, space);
        auto message = arg.substr(space + 1);
        boost::trim(targetName);
        boost::trim(message);

        Search search(ent);
        search.useAll(false).useId(true);

        auto found = search.find(targetName);
        if(found.empty()) return;
        auto targ = found[0];

        auto loc = registry.try_get<Location>(targ);
        if(!loc) return;
        if(loc->locationType == LocationType::Area) {
            auto &area = registry.get<Area>(loc->data);
            auto &room = area.data[loc->x];
            auto rcon = registry.try_get<RoomContents>(room);
            if(!rcon) return;
            for(auto &e : rcon->data) {
                if(e == targ) continue;
                sendLine(e, message);
            }
        }

    }

    void DgScript::cmdAsound(std::unordered_map<std::string, std::string> &matched) {
        // sends <args> to everything in all surrounding rooms of the trigger.
        auto args = matched["args"];
        if(args.empty()) return;

        entt::entity room = entt::null;
        if(auto rm = registry.try_get<Room>(ent); rm) {
            room = ent;
        } else {
            auto loc = registry.try_get<Location>(ent);
            if(!loc) return;
            if(loc->locationType == LocationType::Area) {
                auto &area = registry.get<Area>(loc->data);
                room = area.data[loc->x];
            }
        }
        if(!registry.valid(room)) return;
        if(auto ex = registry.try_get<Exits>(room); ex) {
            for(auto &[d, dest] : ex->data) {
                if(!registry.valid(dest.data)) continue;
                if(dest.locationType == LocationType::Area) {
                    auto &area = registry.get<Area>(dest.data);
                    auto neighbor = area.data[dest.x];
                    auto rcon = registry.try_get<RoomContents>(neighbor);
                    if(!rcon) continue;
                    for(auto &e : rcon->data) {
                        sendLine(e, args);
                    }
                }
            }
        }
    }

    void DgScript::cmdRecho(std::unordered_map<std::string, std::string> &matched) {
        // UNIMPLEMENTED.
    }


    bool DgScript::truthy(const std::string& arg) {
        // A truthy string is one which is not empty or "0".
        // We might need to be more sophisticated later but for now...
        return !arg.empty() && arg != "0";
    }

    std::string DgScript::maybeNegate(const std::string &arg) {
        if(boost::starts_with(arg, "!")) {
            return truthy(maybeNegate(arg.substr(1))) ? "0" : "1";
        }
        return arg;
    }

    bool DgScript::executeIf(const std::string &arg) {
        auto result = truthy(maybeNegate(evaluate(arg)));
        return result;
    }

    std::string DgScript::evaluate(const std::string &arg) {
        auto trimmed = boost::trim_copy(arg);
        if(boost::istarts_with(trimmed, "(")) {
            auto m = matchingParen(trimmed, 0);
            if(m != std::string::npos) {
                return evaluate(trimmed.substr(1, m-1));
            }
        }
        else {
            auto evaluated = varSubstitute(trimmed);
            auto lorResult = evaluateLhsOpRhs(evaluated);
            if(!lorResult.empty()) return lorResult;
            else return evaluated;
        }
        return "";
    }

    static std::vector<std::string> ops = {
            "||", "&&", "==", "!=", "<=", ">=", "<", ">", "/=", "+", "-", "*", "/", "%"
    };

    std::string DgScript::evaluateLhsOpRhs(const std::string &arg) {
        // Detect if we have an operator in the string.
        // If so, split it into lhs, op, rhs and call evaluateOp().
        // If there are no operators, return an empty string.

        for(auto& op : ops) {
            auto pos = arg.find(op);
            if(pos != std::string::npos) {
                auto lhs = boost::trim_copy(arg.substr(0, pos));
                auto rhs = boost::trim_copy(arg.substr(pos + op.size()));
                return evaluateOp(lhs, op, rhs);
            }
        }
        return "";
    }

    static std::string evaluateNum(double lhs, const std::string& op, double rhs) {
        double result;

        if(op == "||") result = lhs || rhs;
        else if(op == "&&") result = lhs && rhs;
        else if(op == "==") result = lhs == rhs;
        else if(op == "!=") result = lhs != rhs;
        else if(op == "<=") result = lhs <= rhs;
        else if(op == ">=") result = lhs >= rhs;
        else if(op == "<") result = lhs < rhs;
        else if(op == ">") result = lhs > rhs;
        else if(op == "/=") result = lhs != rhs;
        else if(op == "+") result = lhs + rhs;
        else if(op == "-") result = lhs - rhs;
        else if(op == "*") result = lhs * rhs;
        else if(op == "/") result = lhs / rhs;
        else if(op == "%") result = fmod(lhs, rhs);

        // If the result is an integer, we should return it as a string of an integer.
        // IE: if it's 2.0 or 1.0, return "2" or "1".
        // Otherwise, we return the double as a string.

        if(result == floor(result)) {
            return boost::lexical_cast<std::string>(static_cast<int64_t>(result));
        }
        else {
            return boost::lexical_cast<std::string>(result);
        }

    }

    std::string DgScript::evaluateOp(const std::string &lhs, const std::string &op, const std::string &rhs) {
        // This one is a little tricky because we have to handle all the different types of operators while
        // also handling the fact that the operands might be strings or numbers.

        // First, let's try coercing both sides to doubles. If that works, we can do numeric comparisons or operations.
        double lhd, rhd;
        try {
            lhd = boost::lexical_cast<double>(lhs);
            rhd = boost::lexical_cast<double>(rhs);
            return evaluateNum(lhd, op, rhd);
        }
        catch(boost::bad_lexical_cast& e) {
            // do nothing
        }

        // If we reached this point, then we'll need to just do our best with string comparisons.

        if(op == "||") return truthy(lhs) || truthy(rhs) ? "1" : "0";
        else if(op == "&&") return truthy(lhs) && truthy(rhs) ? "1" : "0";
        else if(op == "==") return boost::iequals(lhs, rhs) ? "1" : "0";
        else if(op == "!=") return !boost::iequals(lhs, rhs) ? "1" : "0";
        else if(op == "<=") return lhs <= rhs ? "1" : "0";
        else if(op == ">=") return lhs >= rhs ? "1" : "0";
        else if(op == "<") return lhs < rhs ? "1" : "0";
        else if(op == ">") return lhs > rhs ? "1" : "0";
        else if(op == "/=") return boost::icontains(lhs, rhs) ? "1" : "0";
        else if(op == "+") return lhs + rhs;
        else if(op == "-") return lhs + rhs;
        else if(op == "*") return lhs + rhs;
        else if(op == "/") return boost::icontains(lhs, rhs) ? "1" : "0";
        else if(op == "%") return lhs + rhs;
        else return "0";

    }


    std::size_t DgScript::findElseEnd(bool matchElseIf, bool matchElse) {
        if(depth.empty()) {
            throw DgScriptException("findElseEnd() called unexpectedly.");
        }
        auto &d = depth.back();
        if(d.first != DgNest::IF) {
            throw DgScriptException("findElseEnd() called unexpectedly.");
        }

        auto i = d.second + 1;
        auto total = prototype->lines.size();

        while(i < total) {
            auto line = prototype->getLine(i);

            if(line.empty() || boost::istarts_with(line, "*")) {

            }

            else if(matchElseIf && (boost::istarts_with(line, "elseif ") && executeIf(line.substr(7)))) {
                return i + 1;
            }

            else if(matchElse && (boost::iequals(line, "else") || boost::istarts_with(line, "else "))) {
                return i + 1;
            }

            else if(boost::iequals(line, "end") || boost::istarts_with(line, "end ")) {
                return i;
            }

            else if(boost::istarts_with(line, "if ")) {
                depth.emplace_back(DgNest::IF, i);
                i = findEnd() + 1;
                depth.pop_back();
                continue;
            }

            else if(boost::istarts_with(line, "switch ")) {
                depth.emplace_back(DgNest::SWITCH, i);
                i = findDone() + 1;
                depth.pop_back();
                continue;
            }

            else if(boost::istarts_with(line, "while ")) {
                depth.emplace_back(DgNest::WHILE, i);
                i = findDone() + 1;
                depth.pop_back();
                continue;
            }

            else if(boost::iequals(line, "default") || boost::istarts_with(line, "default ")) {
                throw DgScriptException("default found outside of switch block.");
            }

            else if(boost::iequals(line, "done") || boost::istarts_with(line, "done ")) {
                throw DgScriptException("done found outside of switch or while block.");
            }

            else if(boost::iequals(line, "case") || boost::istarts_with(line, "case ")) {
                throw DgScriptException("case found outside of switch block.");
            }
            i++;
        }
        throw DgScriptException("if has no corresponding end");

    }

    std::size_t DgScript::findEnd() {
        return findElseEnd(false, false);
    }

    std::size_t DgScript::findDone() {
        if(depth.empty()) {
            throw DgScriptException("findDone() called unexpectedly.");
        }
        auto &d = depth.back();
        if(d.first != DgNest::SWITCH && d.first != DgNest::WHILE) {
            throw DgScriptException("findDone() called unexpectedly.");
        }

        auto i = d.second + 1;
        auto total = prototype->lines.size();

        while(i < total) {
            auto line = prototype->getLine(i);

            if(line.empty() || boost::istarts_with(line, "*")) {

            }

            else if(boost::istarts_with(line, "if ")) {
                depth.emplace_back(DgNest::IF, i);
                i = findEnd() + 1;
                depth.pop_back();
                continue;
            }

            else if(boost::istarts_with(line, "switch ")) {
                depth.emplace_back(DgNest::SWITCH, i);
                i = findDone() + 1;
                depth.pop_back();
                continue;
            }

            else if(boost::istarts_with(line, "while ")) {
                depth.emplace_back(DgNest::WHILE, i);
                i = findDone() + 1;
                depth.pop_back();
                continue;
            }

            else if(boost::istarts_with(line, "elseif ")) {
                throw DgScriptException("elseif found outside of if block.");
            }

            else if(boost::iequals(line, "else") || boost::istarts_with(line, "else ")) {
                throw DgScriptException("else found outside of if block.");
            }

            else if(boost::iequals(line, "end") || boost::istarts_with(line, "end ")) {
                throw DgScriptException("end found outside of if block.");
            }

            else if(boost::iequals(line, "default") || boost::istarts_with(line, "default ")) {
                throw DgScriptException("default found outside of switch block.");
            }

            else if(boost::iequals(line, "done") || boost::istarts_with(line, "done ")) {
                return i;
            }
            i++;

        }
        throw DgScriptException("Unexpected end of script.");

    }

    std::size_t DgScript::findCase(const std::string &arg) {
        if(depth.empty()) {
            throw DgScriptException("findCase() called unexpectedly.");
        }

        auto &d = depth.back();
        if(d.first != DgNest::SWITCH) {
            throw DgScriptException("findCase() called unexpectedly.");
        }

        auto res = evaluate(arg);

        auto i = d.second + 1;
        auto total = prototype->lines.size();

        while(i < total) {
            auto line = prototype->getLine(i);

            if(line.empty() || boost::istarts_with(line, "*")) {

            }

            else if(boost::istarts_with(line, "case ") && truthy(evaluateOp(res, "==", line.substr(5)))) {
                return i + 1;
            }

            else if(boost::iequals(line, "default") || boost::istarts_with(line, "default ")) {
                return i + 1;
            }

            else if(boost::iequals(line, "done") || boost::istarts_with(line, "done ")) {
                return i;
            }

            else if(boost::istarts_with(line, "if ")) {
                depth.emplace_back(DgNest::IF, i);
                i = findEnd() + 1;
                depth.pop_back();
                continue;
            }

            else if(boost::istarts_with(line, "switch ")) {
                depth.emplace_back(DgNest::SWITCH, i);
                i = findDone() + 1;
                depth.pop_back();
                continue;
            }

            else if(boost::istarts_with(line, "while ")) {
                depth.emplace_back(DgNest::WHILE, i);
                i = findDone() + 1;
                depth.pop_back();
                continue;
            }

            else if(boost::istarts_with(line, "elseif ")) {
                throw DgScriptException("elseif found outside of if block.");
            }

            else if(boost::iequals(line, "else") || boost::istarts_with(line, "else ")) {
                throw DgScriptException("else found outside of if block.");
            }

            else if(boost::iequals(line, "end") || boost::istarts_with(line, "end ")) {
                throw DgScriptException("end found outside of if block.");
            }

            i++;
        }
        throw DgScriptException("switch without corresponding done.");
    }

    std::string DgScript::varSubstitute(const std::string &arg) {
        std::string out;
        auto i = 0;
        bool escaped = false;
        std::size_t m = std::string::npos;
        std::string recursed;

        // This function will iterate through the given string, scanning for
        // sections which are enclosed by percent signs like %this%. Upon finding one,
        // it will recursively call itself and then evaluateVar() on the results,
        // and the output of evaluateVar() is then appended to an output string.
        // Sections which are not enclosed by percent signs are simply appended directly
        // to out, character by character.

        while(i < arg.size()) {
            if(escaped) {
                i++;
                escaped = false;
                continue;
            }

            switch(arg[i]) {
                case '\\':
                    escaped = true;
                    break;
                case '%':
                    m = matchingPerc(arg, i);
                    if(m != std::string::npos) {
                        recursed = varSubstitute(arg.substr(i + 1, m - i - 1));
                        out += evaluateVar(recursed);
                        i = m + 1;
                        continue;
                    }
                    break;
                default:
                    out += arg[i];
            }
            i++;
        }
        return out;
    }

    struct DgMember {
        std::string member;
        bool call{false};
        std::string arg;
        std::size_t resume_index{std::string::npos};
    };

    DgMember getNextMember(std::string data, std::size_t start = 0) {
        DgMember currentMember;
        std::size_t match;

        for(std::size_t i = start; i < data.size(); ++i) {
            switch(data[i]) {
                case '.':
                    currentMember.resume_index = i + 1;
                    return currentMember;
                case '(':
                    match = matchingParen(data, i);
                    if(match != std::string::npos) {
                        currentMember.arg = data.substr(i + 1, match - i - 1);
                        currentMember.call = true;
                        i = match + 1;
                        continue;
                    }
                    // if no match, fall through to default case
                default:
                    if(!currentMember.call) {
                        currentMember.member += data[i];
                    }
            }
        }

        return currentMember;  // end of the data reached, resume_index will be npos
    }

    std::string DgScript::evaluateVar(const std::string& arg) {
        std::vector<DgMember> members;
        std::size_t startIndex = 0;
        std::variant<std::string, Location, DgFunc> lastResult = "";

        while(true) {
            members.emplace_back(getNextMember(arg, startIndex));

            auto &b = members.back();

            if(members.size() == 1) {
                // for the very first member, this could be a script variable or
                // a special function like "time" or "random" or etc.

                // REFER TO dg_variables.cpp line 280 for this shit...
                if(boost::iequals(b.member, "self")) {
                    Location l;
                    l.locationType = LocationType::Inventory;
                    l.data = ent;
                }
                else if(boost::iequals(b.member, "global")) {
                    // set DgFunc for DgGlobal...
                    lastResult = dgGlobal;
                }
                else if(boost::iequals(b.member, "ctime")) {
                    lastResult = std::to_string(time(nullptr));
                }
                else if(boost::iequals(b.member, "people")) {
                    // set lastresult to number of people in location of ent.
                    // if ent is room, then number of people in room.
                    // remember items aren't people. only characters.
                    // TODO: this shit.
                }
                else if(boost::iequals(b.member, "time")) {
                    // set DgFunc for DgTime...
                    // it should be able to handle hour, day, month, and year.
                    // that's for the in-game time.
                    lastResult = dgTime;
                }
                else if(boost::iequals(b.member, "findmob")) {
                    // dg_variables.cpp line 421 check it out later...
                    lastResult = dgFindmob;
                }
                else if(boost::iequals(b.member, "findobj")) {
                    // dg_variables.cpp line 441 check it out later...
                    lastResult = dgFindobj;
                }
                else if(boost::iequals(b.member, "random")) {
                    // reference dg_variables.cpp line 457...
                    lastResult = dgRandom;
                }
                else if(boost::iequals(b.member, "purge") || boost::iequals(b.member, "force") ||
                boost::iequals(b.member, "teleport") || boost::iequals(b.member, "damage") || boost::iequals(b.member, "send")
                || boost::iequals(b.member, "echo") || boost::iequals(b.member, "echoaround") || boost::iequals(b.member, "zoneecho") ||
                        boost::iequals(b.member, "asound") || boost::iequals(b.member, "at") || boost::iequals(b.member, "transform") ||
                        boost::iequals(b.member, "recho") || boost::iequals(b.member, "load")) {
                    lastResult = b.member;
                }
                else {
                    // okay so it wasn't a special function. Then next we'll check
                    // through vars.
                    auto v = getVar(b.member);
                    if(v.has_value()) {
                        auto &va = v.value();
                        if(va.index() == 0) {
                            lastResult = std::get<std::string>(va);
                        } else {
                            auto l = std::get<Location>(va);
                            lastResult = l;
                        }
                    } else {
                        // nothing found at all? Well, guess we're getting an empty string...
                        lastResult = "";
                    }
                }
            } else {
                // if this isn't the first loop, then we are going to attempt to check member traits of whatever is
                // on lastResult. If lastResult is an entity (character, item, or room), then we'll call the right
                // handler for that. if lastResult is a DgFunc, we'll call that. If it's a string, then, we've reached
                // a dead end and will just return whatever we've got. Strings don't have properties.

                if(lastResult.index() == 0) {
                    // it's a string... we are sol.
                    lastResult = "";
                }
                else if(lastResult.index() == 1) {
                    // it's an entity...
                    auto &l = std::get<Location>(lastResult);
                    entt::entity target = entt::null;
                    if(l.locationType == LocationType::Absolute) target = l.data;
                    else if(l.locationType == LocationType::Area) target = l.getRoom();

                    if(registry.valid(target)) {
                        auto &dg = registry.get_or_emplace<DgScripts>(target, target);
                        auto res = dg.evalVar(shared_from_this(), b.member, b.call, b.arg);
                        if(res.index() == 0) {
                            lastResult = std::get<std::string>(res);
                        } else {
                            lastResult = std::get<Location>(res);
                        }
                    } else {
                        lastResult = "";
                    }
                }
                else if(lastResult.index() == 2) {
                    // it's a DgFunc.
                    auto &f = std::get<DgFunc>(lastResult);
                    auto res = f(shared_from_this(), b.member, b.call, b.arg);
                    if(res.index() == 0) {
                        lastResult = std::get<std::string>(res);
                    } else {
                        auto l = std::get<Location>(res);
                        lastResult = l;
                    }
                }

            }

            if(members.back().resume_index == std::string::npos) {
                break;
            } else {
                startIndex = members.back().resume_index;
            }
        }

        if(lastResult.index() == 0) {
            return std::get<std::string>(lastResult);
        } else if(lastResult.index() == 1) {
            // lastResult is an entity... we can at least get its name or something.
            auto l = std::get<Location>(lastResult);
            if(l.locationType == LocationType::Absolute)
                return getName(l.data);
            else if(l.locationType == LocationType::Area) {
                auto r = l.getRoom();
                return getName(r);
            } else return getName(l.data);
        } else {
            // lastResult is a DgFunc... let's just call it and return whatever it returns.
            auto &f = std::get<DgFunc>(lastResult);
            auto res = f(shared_from_this(), "", false, "");
            if(res.index() == 0) {
                return std::get<std::string>(res);
            } else {
                auto l = std::get<Location>(res);
                if(l.locationType == LocationType::Absolute)
                    return getName(l.data);
                else if(l.locationType == LocationType::Area) {
                    auto r = l.getRoom();
                    return getName(r);
                } else return getName(l.data);
            }
        }
    }

    OpResult<> DgScripts::addScript(std::size_t id) {
        auto find = dgScripts.find(id);
        if(find == dgScripts.end()) {
            return {false, "Script not found."};
        }

        for(auto &s : scripts) {
            if(s->prototype->id == id) {
                return {false, "Script already exists."};
            }
        }

        auto script = std::make_shared<DgScript>(ent, find->second);
        dgScriptInstances.insert(script);
        scripts.push_back(script);
        return {true, ""};
    }

    OpResult<> DgScripts::removeScript(std::size_t id) {
        auto found = std::find_if(scripts.begin(), scripts.end(), [id](auto &s) {
            return s->prototype->id == id;
        });
        if(found == scripts.end()) {
            return {false, "Script not found."};
        }

        auto script = *found;
        script->setState(DgState::PURGED);
        dgScriptInstances.erase(script);
        scripts.erase(found);
        return {true, ""};
    }

    DgVariant dgGlobal(const std::shared_ptr<DgScript>& script, const std::string& member, bool call, const std::string& arg) {
        if(member.empty()) {
            return "";
        }

        auto var = globalDgVars.getVar(member);
        if(var.has_value()) {
            return var.value();
        } else {
            return "";
        }
    }

    DgVariant dgTime(const std::shared_ptr<DgScript>& script, const std::string& member, bool call, const std::string& arg) {
        if(boost::iequals(member, "hour")) {
            return std::to_string(time_info.hours);
        }
        else if(boost::iequals(member, "day")) {
            return std::to_string(time_info.day);
        }
        else if(boost::iequals(member, "month")) {
            return std::to_string(time_info.month);
        }
        else if(boost::iequals(member, "year")) {
            return std::to_string(time_info.year);
        }
        return "";
    }

    DgVariant dgFindmob(const std::shared_ptr<DgScript>& script, const std::string& member, bool call, const std::string& arg) {
        return "";
    }

    DgVariant dgFindobj(const std::shared_ptr<DgScript>& script, const std::string& member, bool call, const std::string& arg) {
        return "";
    }

    DgVariant dgRandom(const std::shared_ptr<DgScript>& script, const std::string& member, bool call, const std::string& arg) {
        return "";
    }

    static std::unordered_map<std::string, std::string> evalCharAliases = {
            {"alias", "name"},
            {"class", "sensei"},
            {"con", "constitution"},
            {"str", "strength"},
            {"cha", "speed"},
            {"spd", "speed"},
            {"dex", "dexterity"},
            {"int", "intelligence"},
            {"wis", "wisdom"},
            {"exp", "experience"},
            {"align", "alignment"},
            {"hitp", "health"},
            {"maxhitp", "maxhealth"},
            {"maxmana", "maxki"},
            {"mana", "ki"},
            {"maxmove", "maxstamina"},
            {"move", "stamina"},
            {"zenni", "gold"}
    };

    static std::unordered_map<std::string, affect::AffectId> evalCharAffects = {
            {"dead", affect::SPIRIT},
            {"flying", affect::FLYING}
    };

    static std::unordered_map<std::string, pflags::PFlagId> evalCharPlayerFlags = {
            {"is_killer", pflags::KILLER},
            {"is_thief", pflags::THIEF}
    };

    static DgVariant evalCharacter(entt::entity ent, const std::shared_ptr<DgScript> &script, const std::string &member, bool call,
                                   const std::string &arg, double val, bool goodVal) {
        // refer to dg_variables.cpp line 534 onwards...

        auto checkMember = member;
        auto find = evalCharAliases.find(checkMember);
        if(find != evalCharAliases.end()) {
            checkMember = find->second;
        }

        if(checkMember == "name") {
            return getName(ent);
        }

        if(auto isStat = cstat::characterStatNames.find(checkMember); isStat != cstat::characterStatNames.end()) {
            auto statId = isStat->second;
            auto statFound = cstat::characterStats.find(statId);
            if(statFound == cstat::characterStats.end()) {
                return "";
            }
            // Hooray this'll be easy.
            if(call && goodVal) {
                statFound->second->modBase(ent, val);
            }
            std::unordered_map<std::size_t, double> cache;
            return trimNum(statFound->second->getEffective(ent, cache));
        }

        if(auto isAff = evalCharAffects.find(checkMember); isAff != evalCharAffects.end()) {
            if(auto ca = registry.try_get<CharacterAffects>(ent); ca) {
                return ca->data.test(isAff->second) ? "1" : "0";
            }
            return "0";
        }

        if(auto isPlr = evalCharPlayerFlags.find(checkMember); isPlr != evalCharPlayerFlags.end()) {
            if(registry.any_of<NPC>(ent)) return "0";

            if(!arg.empty()) {
                if(boost::iequals(arg, "on")) {
                    auto &plr = registry.get_or_emplace<PlayerFlags>(ent);
                    plr.data.set(isPlr->second);
                } else if(boost::iequals(arg, "off")) {
                    auto &plr = registry.get_or_emplace<PlayerFlags>(ent);
                    plr.data.reset(isPlr->second);
                }
            }
            if(auto cp = registry.try_get<PlayerFlags>(ent); cp) {
                return cp->data.test(isPlr->second) ? "1" : "0";
            }
            return "0";
        }

        if(checkMember == "affect") {
            if(arg.empty()) return "0";
            // arg is gonna be a flag name...
            auto aff = affect::getAffectId(arg);
            if(!aff.has_value()) return "0";
            if(auto ca = registry.try_get<CharacterAffects>(ent); ca) {
                return ca->data.test(aff.value()) ? "1" : "0";
            }
            return "0";
        }

        if(checkMember == "bank") {
            // TODO: Implement...
        }

        if(checkMember == "canbeseen") {
            // TODO: Implement...
        }

        if(checkMember == "carry") {
            // TODO: Implement...
        }

        if(checkMember == "clan") {
            // TODO: Implement...
        }

        if(checkMember == "sensei") {
            auto &sen = registry.get_or_emplace<Sensei>(ent);
            return sensei::senseis[sen.data]->getName();
        }

        if(checkMember == "race") {
            auto &ra = registry.get_or_emplace<Race>(ent);
            return race::races[ra.data]->getName();
        }

        if(checkMember == "death") {
            // TODO: Implement... get deathtimer...
        }

        if(checkMember == "drag") {
            // TODO: Implement...
        }

        if(checkMember == "drunk") {
            // TODO: Implement...
        }

        if(checkMember == "eq") {
            // TODO: Implement...
        }

        if(checkMember == "fighting") {
            // TODO: Implement...
        }

        if(checkMember == "follower") {
            // TODO: Implement...
        }

        if(checkMember == "gold") {
            if(call && goodVal) {
                auto &gold = registry.get_or_emplace<Money>(ent);
                gold.data += val;
                if(gold.data < 0) gold.data = 0;
            }
            if(auto gold = registry.try_get<Money>(ent); gold) {
                return std::to_string(gold->data);
            }
        }

        if(checkMember == "has_item") {
            if(arg.empty()) return "0";
            auto search = Search(ent).useId(true).setType(SearchType::Items).equipment(ent).inventory(ent);
            return search.find(arg).empty() ? "0" : "1";
        }

        if(checkMember == "hisher") {
            // TODO: Implement...
        }

        if(checkMember == "heshe") {
            // TODO: Implement...
        }

        if(checkMember == "himher") {
            // TODO: Implement...
        }

        if(checkMember == "hunger") {
            // TODO: Implement...
        }

        if(checkMember == "id") {
            auto &o = registry.get<ObjectId>(ent);
            return o.toString();
        }

        if(checkMember == "is_pc") {
            return registry.any_of<Player>(ent) ? "1" : "0";
        }

        if(checkMember == "inventory") {
            if(!goodVal) return "";
            if(auto inv = registry.try_get<Inventory>(ent); inv) {
                // we are going to search every item in inv->data for one with an ItemVnum component with data == val.
                for(auto &item : inv->data) {
                    if(auto vnum = registry.try_get<ItemVnum>(item); vnum) {
                        if(vnum->data == val) {
                            Location l;
                            l.locationType == LocationType::Absolute;
                            l.data = item;
                            return l;
                        }
                    }
                }
            }
        }

        if(checkMember == "master") {
            // TODO: Implement...
        }

        if(checkMember == "pos") {
            if(!arg.empty()) {
                // can set to Sleeping, Resting, Sitting, Fighting, or Standing...
                // TODO: Implement...
            }

            auto &pos = registry.get_or_emplace<Position>(ent);
            return position::positions[pos.data]->getName();
        }

        if(checkMember == "prac") {
            // TODO: implement practices...
        }

        if(checkMember == "plr") {
            if(arg.empty()) return "0";
            if(!registry.any_of<Player>(ent)) return "0";

            auto plrId = pflags::getPlayerFlagId(arg);
            if(!plrId.has_value()) return "0";
            if(auto plr = registry.try_get<PlayerFlags>(ent); plr) {
                return plr->data.test(plrId.value()) ? "1" : "0";
            }
            return "0";
        }

        if(checkMember == "pref") {
            if(arg.empty()) return "0";
            if(!registry.any_of<Player>(ent)) return "0";

            auto prfId = pref::getPreferenceFlagId(arg);
            if(!prfId.has_value()) return "0";
            if(auto prf = registry.try_get<PreferenceFlags>(ent); prf) {
                return prf->data.test(prfId.value()) ? "1" : "0";
            }
            return "0";
        }

        if(checkMember == "room") {
            if(auto loc = registry.try_get<Location>(ent); loc) {
                return *loc;
            }
            return "";
        }

        if(checkMember == "rpp") {
            // TODO: Implement..
        }

        if(checkMember == "sex") {
            auto &se = registry.get_or_emplace<Sex>(ent);
            return sex::sexes[se.data]->getName();
        }

        if(checkMember == "size") {
            std::size_t si = size::MEDIUM;
            if(auto sz = registry.try_get<Size>(ent); sz) {
                si = sz->data;
            } else {
                auto &race = registry.get_or_emplace<Race>(ent);
                si = race::races[race.data]->getSize();
            }
            return size::sizes[si]->getName();
        }

        if(checkMember == "skill") {
            // TODO: implement...
        }

        if(checkMember == "skillset") {
            // TODO: implement...
        }

        if(checkMember == "saving_fortitude") {
            // TODO: implement...
        }

        if(checkMember == "saving_reflex") {
            // TODO: implement...
        }

        if(checkMember == "saving_will") {
            // TODO: implement...
        }

        if(checkMember == "thirst") {
            // TODO: implement...
        }

        if(checkMember == "tnl") {
            // TODO: implement...
        }

        if(checkMember == "vnum") {
            if(auto v = registry.try_get<NPCVnum>(ent); v) {
                return std::to_string(v->data);
            }
            return "-1";
        }

        if(checkMember == "varexists") {
            auto &s = registry.get_or_emplace<DgScripts>(ent, ent);
            return s.hasVar(arg) ? "1" : "0";
        }

        if(checkMember == "weight") {
            // TODO: implement...
        }

        auto &s = registry.get_or_emplace<DgScripts>(ent, ent);
        auto var = s.getVar(checkMember);

        if(var.has_value()) return var.value();

        return "";

    }

    static DgVariant evalRoom(entt::entity ent, const std::shared_ptr<DgScript> &script, const std::string &member, bool call,
                              const std::string &arg, double val, bool goodVal) {
        return "";
    }

    static DgVariant evalItem(entt::entity ent, const std::shared_ptr<DgScript> &script, const std::string &member, bool call,
                              const std::string &arg, double val, bool goodVal) {

        auto checkMember = member;

        return "";

    }

    DgVariant DgScripts::evalVar(const std::shared_ptr<DgScript> &script, const std::string &member, bool call,
                                 const std::string &arg) {

        if(member.empty()) {
            return getName(ent);
        }

        double val = 0;
        bool goodVal = false;
        if(!arg.empty()) {
            // coerce arg into double val if possible and set goodVal true if it is possible.
            try {
                val = std::stod(arg);
                goodVal = true;
            } catch(...) {
                goodVal = false;
            }
        }
        std::string checkMember = boost::to_lower_copy(member);
        boost::trim(checkMember);


        if(registry.any_of<Room>(ent)) return evalRoom(ent, script, member, call, arg, val, goodVal);
        else if(registry.any_of<Character>(ent)) return evalCharacter(ent, script, member, call, arg, val, goodVal);
        else if(registry.any_of<Item>(ent)) return evalItem(ent, script, member, call, arg, val, goodVal);
        else return "";

    }

}