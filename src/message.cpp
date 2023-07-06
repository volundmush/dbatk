#include "dbatk/message.h"
#include "dbatk/grammar.h"
#include "dbatk/api.h"
#include <boost/regex.hpp>

namespace dbat {

    static boost::regex msgRegex(R"(\$(?<func>\w+)\()");

    static std::string parseYou(const MsgFormat &msg, const std::string& arg, entt::entity receiver) {
        entt::entity target = registry.valid(msg.actor) ? msg.actor : receiver;
        if(!arg.empty()) {
            auto lower = boost::to_lower_copy(arg);
            auto find = msg.variables.find(lower);
            if(find != msg.variables.end()) {
                target = find->second;
            } else {
                return "NOTFOUND";
            }
        }
        if(target == receiver) return "you";
        return getDisplayName(target, receiver);
    }

    static std::string parseConj(const MsgFormat &msg, const std::string& arg, entt::entity receiver) {
        if(arg.empty()) return "";
        auto lower = boost::to_lower_copy(arg);

        auto candidates = grammar::getConjugateCandidates(lower);
        auto out = msg.actor == receiver ? candidates.first : candidates.second;
        return out;
    }

    static std::string parsePronoun(const MsgFormat &msg, const std::string& arg, entt::entity receiver, grammar::PronounType pType) {
        entt::entity target = registry.valid(msg.actor) ? msg.actor : receiver;
        if(!arg.empty()) {
            auto lower = boost::to_lower_copy(arg);
            auto find = msg.variables.find(lower);
            if(find != msg.variables.end()) {
                target = find->second;
            } else {
                return "NOTFOUND";
            }
        }

        sex::SexId id = sex::NEUTER;

        if(auto s = registry.try_get<Sex>(target) ; s) {
            id = s->data;
        }

        auto person = target == receiver ? grammar::VerbPerson::FirstPerson : grammar::VerbPerson::ThirdPerson;

        auto out = grammar::getPronoun(id, person, pType);
        return out;
    }

    static std::string parseSubj(const MsgFormat &msg, const std::string& arg, entt::entity receiver) {
        return parsePronoun(msg, arg, receiver, grammar::PronounType::Subjective);
    }

    static std::string parseObj(const MsgFormat &msg, const std::string& arg, entt::entity receiver) {
        return parsePronoun(msg, arg, receiver, grammar::PronounType::Objective);

    }

    static std::string parsePoss(const MsgFormat &msg, const std::string& arg, entt::entity receiver) {
        return parsePronoun(msg, arg, receiver, grammar::PronounType::PossessivePronoun);
    }

    static std::string parseRefl(const MsgFormat &msg, const std::string& arg, entt::entity receiver) {
        return parsePronoun(msg, arg, receiver, grammar::PronounType::Reflexive);
    }

    static std::string parseAn(const MsgFormat &msg, const std::string& arg, entt::entity receiver) {
        return grammar::an(arg);
    }

    static std::string parseLower(const MsgFormat &msg, const std::string& arg, entt::entity receiver) {
        return boost::to_lower_copy(arg);
    }

    static std::string parseUpper(const MsgFormat &msg, const std::string& arg, entt::entity receiver) {
        return boost::to_upper_copy(arg);
    }

    static std::unordered_map<std::string, std::function<std::string(const MsgFormat&, const std::string&, entt::entity)>> msgFuncs = {
        {"you", parseYou},
        {"obj", parseObj},
        {"conj", parseConj},
        {"subj", parseSubj},
        {"poss", parsePoss},
        {"refl", parseRefl},
        {"an", parseAn},
        {"lower", parseLower},
        {"upper", parseUpper}
    };

    MsgFormat& MsgFormat::str(const std::string& m) {
        this->msg = m;
        return *this;
    }

    MsgFormat& MsgFormat::exclude(entt::entity ent) {
        toExclude.insert(ent);
        return *this;
    }

    MsgFormat& MsgFormat::direct(entt::entity recipient) {
        auto &loc = recipients.emplace_back();
        loc.locationType = LocationType::Absolute;
        loc.data = recipient;
        return *this;
    }

    MsgFormat& MsgFormat::region(entt::entity reg) {
        regions.emplace_back(reg);
        return *this;
    }

    MsgFormat& MsgFormat::in(dbat::Location loc) {
        recipients.emplace_back(loc);
        return *this;
    }

    MsgFormat& MsgFormat::set(const std::string& name, entt::entity ent) {
        variables[name] = ent;
        return *this;
    }

    void MsgFormat::send() {
        std::set<entt::entity> seen;

        for(auto &l : recipients) {
            if(l.locationType == LocationType::Absolute) {
                // r.second is our direct target to send to.
                if(toExclude.contains(l.data)) continue;
                if(seen.contains(l.data)) continue;
                sendTo(l.data);
                seen.insert(l.data);
            }
            else if(l.locationType == LocationType::Area) {
                // r.second is a room entity, we should retrieve RoomContents and iterate.
                auto r = l.getRoom();
                if(auto rcon = registry.try_get<RoomContents>(r); rcon) {
                    for(auto &e : rcon->data) {
                        if(toExclude.contains(e)) continue;
                        if(seen.contains(e)) continue;
                        sendTo(e);
                        seen.insert(e);
                    }
                }
            } else if(l.locationType == LocationType::Expanse) {

            }
        }
        for(auto &reg : regions) {
            for(auto e : getContents(reg)) {
                if(toExclude.contains(e)) continue;
                if(seen.contains(e)) continue;
                sendTo(e);
                seen.insert(e);
            }
        }

    }

    std::string MsgFormat::callFunc(const std::string &func, const std::string &arg, entt::entity recipient,
                                    bool capitalized) {
        auto lower = boost::to_lower_copy(func);
        // handle recursive parsing...
        auto parseArg = parseMessage(arg, recipient);
        auto find = msgFuncs.find(lower);
        if(find != msgFuncs.end()) {
            auto res = find->second(*this, parseArg, recipient);
            if(capitalized) return grammar::capitalize(res);
            return res;
        }
        return "NOTFOUND";
    }

    static std::size_t findMatchingParen(const std::string &txt, std::size_t start) {
        std::size_t depth = 0;
        for(std::size_t i = start; i < txt.size(); i++) {
            if(txt[i] == '(') {
                depth++;
            } else if(txt[i] == ')') {
                depth--;
                if(depth == 0) {
                    return i;
                }
            }
        }
        return std::string::npos;
    }

    std::string MsgFormat::parseMessage(const std::string &txt, entt::entity recipient) {
        // the txt variable holds a message to send, encoded with template language.
        // An example is: "$you() $capitalize($conj(punch)) $you(target) in $poss(target) face!"

        // We must recursively scan it...

        // Our logic will be thus: we shall scan the string for the first msgRegex (such as "$you(" ) and
        // locate the matching closing paren. We then append up to just before the $, and
        // We will then call the function with the contents of the parens to handle replacement, and append the result.
        // Once we stop finding msgRegex matches, we will append the rest of the string and return.

        std::string out = txt;

        // First, let's scan for the first msgRegex match.
        boost::smatch match;

        while(true) {
            // We will keep looping until there are no matches, but we must scan anew each time...
            if(!boost::regex_search(out, match, msgRegex)) {
                // We didn't find a match. Let's just append the rest of the string and return.
                return out;
            }

            // Since there is a match, we will find the matching paren and then call out.replace with the result.
            // Note that <func> is matched in the msgRegex so that's the function name to call.
            auto current = match.position();
            auto end = findMatchingParen(out, current);
            if(end == std::string::npos) {
                // We didn't find a matching paren. Let's just append the rest of the string and return.
                out.append(out.substr(current));
                return out;
            }

            // Save the section between parens to a variable...
            auto arg = out.substr(current + match.length(), end - current - match.length());
            // check if first character of the function call is capitalized..
            auto funcName = match[1].str();
            auto capitalized = std::isupper(funcName[0]);
            auto called = callFunc(funcName, arg, recipient, capitalized);

            // we want to replace the whole function call $blah(arg) with what's in called.
            out.replace(current, end - current + 1, called);

            // Now the loop will run again...
        }
    }

    MsgFormat::MsgFormat(entt::entity ent, const std::string &msg) {
        actor = ent;
        this->msg = msg;
        if(actor != entt::null)
            direct(actor);
    }

    void MsgFormat::sendTo(entt::entity recipient) {
        if(!registry.any_of<SessionHolder>(recipient)) return;
        if(filter && !filter(recipient)) return;

        // the .msg variable holds a message to send, encoded with template language.
        // An example is: "$you() $capitalize($conj(punch)) $you(target) in $poss(target) face!"
        // We will call the parseMessage() function to parse this message and return a string.
        // it will recursively move through all of those and spit out the end result.

        auto out = parseMessage(msg, recipient);

        sendLine(recipient, out);
    }

}