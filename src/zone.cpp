#include "dbatk/zone.h"
#include "dbatk/database.h"
#include "dbatk/api.h"
#include "dbatk/components.h"
#include "dbatk/config.h"
#include "fmt/format.h"

namespace dbat {

    nlohmann::json reset_com::serialize() {
        nlohmann::json j;
        std::string cmd;
        cmd.push_back(command);
        j["command"] = cmd;
        if(if_flag) j["if_flag"] = if_flag;
        if(arg1) j["arg1"] = arg1;
        if(arg2) j["arg2"] = arg2;
        if(arg3) j["arg3"] = arg3;
        if(arg4) j["arg4"] = arg4;
        if(arg5) j["arg5"] = arg5;
        if(!sarg1.empty()) j["sarg1"] = sarg1;
        if(!sarg2.empty()) j["sarg2"] = sarg2;

        return j;
    }

    reset_com::reset_com(const nlohmann::json &j) {
        if(j.contains("command")) {
            std::string cmd = j["command"];
            command = cmd[0];
        }
        if(j.contains("if_flag")) if_flag = j["if_flag"];
        if(j.contains("arg1")) arg1 = j["arg1"];
        if(j.contains("arg2")) arg2 = j["arg2"];
        if(j.contains("arg3")) arg3 = j["arg3"];
        if(j.contains("arg4")) arg4 = j["arg4"];
        if(j.contains("arg5")) arg5 = j["arg5"];
        if(j.contains("sarg1")) sarg1 = j["sarg1"];
        if(j.contains("sarg2")) sarg2 = j["sarg2"];
    }

    nlohmann::json Zone::serialize() {
        nlohmann::json j;
        if(!name.empty()) j["name"] = name;
        if(!builders.empty()) j["builders"] = builders;
        if(lifespan) j["lifespan"] = lifespan;
        if(reset_mode) j["reset_mode"] = reset_mode;
        if(!min_level) j["min_level"] = min_level;
        if(!max_level) j["max_level"] = max_level;
        j["bot"] = bot;
        j["top"] = top;

        if(flags.any()) j["flags"] = bitsetToJson(flags);

        for(auto& cmd : cmds) {
            j["cmds"].push_back(cmd.serialize());
        }

        return j;
    }

    Zone::Zone(const nlohmann::json &j) {
        if(j.contains("name")) name = j["name"];
        if(j.contains("builders")) builders = j["builders"];
        if(j.contains("lifespan")) lifespan = j["lifespan"];
        if(j.contains("reset_mode")) reset_mode = j["reset_mode"];
        if(j.contains("min_level")) min_level = j["min_level"];
        if(j.contains("max_level")) max_level = j["max_level"];
        if(j.contains("bot")) bot = j["bot"];
        if(j.contains("top")) top = j["top"];
        if(j.contains("flags")) jsonToBitset(j["flags"], flags);

        if(j.contains("cmds")) {
            for(auto& cmd : j["cmds"]) {
                cmds.emplace_back(cmd);
            }
        }
    }

    std::map<std::size_t, Zone> zones;

    void Zone::onHeartbeat(double deltaTime) {
        age -= deltaTime;
        if(age <= 0.0) {
            reset();
            age = lifespan * 60.0;
        }
    }

    void Zone::reset() {
        //logger->info("Resetting zone {}: {}", id, name);
        int cmd_no = 0;
        bool last_cmd = false;
        entt::entity mob = entt::null;
        entt::entity obj = entt::null;
        entt::entity tmob = entt::null, tobj = entt::null;

        RoomId rvnum, rrnum;
        bool mob_load = false;
        bool obj_load = false;

        std::unordered_map<std::string, std::list<entt::entity>> spawns;
        int line = 0;
        for(auto &c : cmds) {
            // comment
            line++;
            if(c.command == '*') {
                last_cmd = false;
                continue;
            }
            // Read a Mobile.
            else if(c.command == 'M') {
                auto protoName = fmt::format("npc:{}", c.arg1);
                auto proto = prototypes.find(protoName);
                if(proto == prototypes.end()) {
                    logger->error("Line {}: Unable to find prototype for npc:{}", line, c.arg1);
                    tobj = entt::null;
                    continue;
                }
                auto count = proto->second->instanceCount;
                if(proto->second->instanceCount >= c.arg2) {
                    last_cmd = false;
                    tobj = entt::null;
                    continue;
                }
                auto rand = randomNumber(1, 100);
                if(!(rand >= c.arg5)) {
                    last_cmd = false;
                    tobj = entt::null;
                    continue;
                }
                if(c.arg4 > 0) {
                    auto view = registry.view<NPCVnum, LegacyLoadRoom>();
                    int counter = 0;
                    for(auto e : view) {
                        auto &vnum = view.get<NPCVnum>(e);
                        auto &loadRoom = view.get<LegacyLoadRoom>(e);
                        if(vnum.data == c.arg1 && loadRoom.id == c.arg3) {
                            counter++;
                            if(counter >= c.arg4) break;
                        }
                        if(counter >= c.arg4) break;
                    }
                    if(counter >= c.arg4) {
                        last_cmd = false;
                        tobj = entt::null;
                        continue;
                    }
                }
                // All checks have passed; let's spawn it.
                mob = proto->second->spawn();
                auto &vnum = registry.get_or_emplace<NPCVnum>(mob);
                vnum.data = c.arg1;
                auto &loadRoom = registry.emplace<LegacyLoadRoom>(mob);
                loadRoom.id = c.arg3;
                tmob = mob;
                tobj = entt::null;
                last_cmd = true;
                mob_load = true;
                MoveParams params;
                params.traverseType = TraverseType::System;
                params.moveType = MoveType::Traverse;
                params.force = true;
                params.quiet = true;
                params.dest = legacyRooms[c.arg3];
                moveTo(mob, params);
                spawns[protoName].emplace_back(mob);
            }
            // Read an Object
            else if(c.command == 'O') {
                auto protoName = fmt::format("item:{}", c.arg1);
                auto proto = prototypes.find(protoName);
                if(proto == prototypes.end()) {
                    logger->error("Line {}: Unable to find prototype for item:{}", line, c.arg1);
                    tmob = entt::null;
                    continue;
                }
                auto count = proto->second->instanceCount;
                if(count >= c.arg2) {
                    tmob = entt::null;
                    last_cmd = false;
                    continue;
                }
                auto rand = randomNumber(1, 100);
                if(!(rand >= c.arg5)) {
                    last_cmd = false;
                    tmob = entt::null;
                    continue;
                }
                if(c.arg4 > 0) {
                    auto view = registry.view<ItemVnum, LegacyLoadRoom>();
                    int counter = 0;
                    for(auto e : view) {
                        auto &vnum = view.get<ItemVnum>(e);
                        auto &loadRoom = view.get<LegacyLoadRoom>(e);
                        if(vnum.data == c.arg1 && loadRoom.id == c.arg3) {
                            counter++;
                            if(counter >= c.arg4) break;
                        }
                        if(counter >= c.arg4) break;
                    }
                    if(counter >= c.arg4) {
                        last_cmd = false;
                        tmob = entt::null;
                        continue;
                    }
                }
                // All checks have passed; let's spawn it.
                obj = proto->second->spawn();
                auto &vnum = registry.get_or_emplace<ItemVnum>(obj);
                vnum.data = c.arg1;
                auto &loadRoom = registry.emplace<LegacyLoadRoom>(obj);
                loadRoom.id = c.arg3;
                tobj = obj;
                tmob = entt::null;
                last_cmd = true;
                obj_load = true;
                MoveParams params;
                params.traverseType = TraverseType::System;
                params.moveType = MoveType::Traverse;
                params.force = true;
                params.quiet = true;
                params.dest = legacyRooms[c.arg3];
                moveTo(obj, params);
                spawns[protoName].emplace_back(obj);
            }

            // Spawn a c.arg1 to the last c.arg3 object...
            else if(c.command == 'P') {
                auto protoName = fmt::format("item:{}", c.arg1);
                auto proto = prototypes.find(protoName);
                if(proto == prototypes.end()) {
                    logger->error("Unable to find prototype for item:{}", c.arg1);
                    tmob = entt::null;
                    continue;
                }
                auto count = proto->second->instanceCount;
                if(count >= c.arg2) {
                    last_cmd = false;
                    tmob = entt::null;
                    continue;
                }
                auto rand = randomNumber(1, 100);
                if(!(rand >= c.arg5)) {
                    last_cmd = false;
                    tmob = entt::null;
                    continue;
                }
                auto checkName = fmt::format("item:{}", c.arg3);
                auto check = spawns.find(checkName);
                if(check == spawns.end()) {
                    last_cmd = false;
                    tmob = entt::null;
                    continue;
                }
                auto found = check->second.back();

                // All checks have passed; let's spawn it.
                obj = proto->second->spawn();
                auto &vnum = registry.get_or_emplace<ItemVnum>(obj);
                vnum.data = c.arg1;
                tobj = obj;
                tmob = entt::null;
                last_cmd = true;
                MoveParams params;
                params.traverseType = TraverseType::System;
                params.moveType = MoveType::Traverse;
                params.force = true;
                params.quiet = true;
                Location dest;
                dest.data = found;
                dest.locationType = LocationType::Inventory;
                params.dest = dest;
                moveTo(obj, params);
                spawns[protoName].emplace_back(obj);
            }

            // load a c.arg1 item and give it to mob.
            else if(c.command == 'G') {
                auto protoName = fmt::format("item:{}", c.arg1);
                auto proto = prototypes.find(protoName);
                if(proto == prototypes.end()) {
                    logger->error("Unable to find prototype for item:{}", c.arg1);
                    tmob = entt::null;
                    continue;
                }
                auto count = proto->second->instanceCount;
                if(count >= c.arg2) {
                    last_cmd = false;
                    tmob = entt::null;
                    continue;
                }
                auto rand = randomNumber(1, 100);
                if(!(rand >= c.arg5)) {
                    last_cmd = false;
                    tmob = entt::null;
                    continue;
                }
                if(!registry.valid(mob)) {
                    last_cmd = false;
                    tmob = entt::null;
                    continue;
                }
                // All checks have passed; let's spawn it.
                obj = proto->second->spawn();
                auto &vnum = registry.get_or_emplace<ItemVnum>(obj);
                vnum.data = c.arg1;
                tobj = obj;
                tmob = entt::null;
                last_cmd = true;
                MoveParams params;
                params.traverseType = TraverseType::System;
                params.moveType = MoveType::Traverse;
                params.force = true;
                params.quiet = true;
                Location dest;
                dest.data = mob;
                dest.locationType = LocationType::Inventory;
                params.dest = dest;
                moveTo(obj, params);
                spawns[protoName].emplace_back(obj);
            }

            // spawn a c.arg1 item to last mob's equipment, slot c.arg3
            else if(c.command == 'E') {
                auto protoName = fmt::format("item:{}", c.arg1);
                auto proto = prototypes.find(protoName);
                if(proto == prototypes.end()) {
                    logger->error("Unable to find prototype for item:{}", c.arg1);
                    tmob = entt::null;
                    continue;
                }
                auto count = proto->second->instanceCount;
                if(count >= c.arg2) {
                    last_cmd = false;
                    tmob = entt::null;
                    continue;
                }
                auto rand = randomNumber(1, 100);
                if(!(rand >= c.arg5)) {
                    last_cmd = false;
                    tmob = entt::null;
                    continue;
                }
                if(!registry.valid(mob)) {
                    last_cmd = false;
                    tmob = entt::null;
                    continue;
                }

                // TODO: Check if c.arg3 is a valid equipment slot...

                // All checks have passed; let's spawn it.
                obj = proto->second->spawn();
                auto &vnum = registry.get_or_emplace<ItemVnum>(obj);
                vnum.data = c.arg1;
                tobj = obj;
                tmob = entt::null;
                last_cmd = true;
                MoveParams params;
                params.traverseType = TraverseType::System;
                params.moveType = MoveType::Traverse;
                params.force = true;
                params.quiet = true;
                Location dest;
                dest.data = mob;
                dest.locationType = LocationType::Equipment;
                dest.x = c.arg3;
                params.dest = dest;
                moveTo(obj, params);
                spawns[protoName].emplace_back(obj);
            }

            // manipulate a door.
            else if(c.command == 'D') {
                auto loc = legacyRooms.find(c.arg1);
                if(loc == legacyRooms.end()) {
                    logger->error("Unable to find room {} for door manipulation.", c.arg1);
                    last_cmd = false;
                    tmob = entt::null;
                    tobj = entt::null;
                    continue;
                }
                auto dest = loc->second;
                if(dest.locationType != LocationType::Area) {
                    logger->error("Unable to find room {} for door manipulation.", c.arg1);
                    last_cmd = false;
                    tmob = entt::null;
                    tobj = entt::null;
                    continue;
                }
                if(c.arg2 < 0 || c.arg2 >= dir::numAllDirs) {
                    logger->error("Invalid direction {} for door manipulation.", c.arg2);
                    last_cmd = false;
                    tmob = entt::null;
                    tobj = entt::null;
                    continue;
                }

                auto &area = registry.get<Area>(dest.data);
                auto room = area.data[c.arg1];
                auto &doors = registry.get_or_emplace<Doors>(room);
                auto &door = doors.data[static_cast<dir::DirectionId>(c.arg2)];
                switch(c.arg3) {
                    case 0:
                        door.flags.reset(eflags::LOCKED);
                        door.flags.reset(eflags::CLOSED);
                        break;
                    case 1:
                        door.flags.set(eflags::CLOSED);
                        door.flags.reset(eflags::LOCKED);
                        break;
                    case 2:
                        door.flags.set(eflags::LOCKED);
                        door.flags.set(eflags::CLOSED);
                        break;
                }
                last_cmd = true;
                tmob = entt::null;
                tobj = entt::null;
            }

            // assign a Trigger (DgScript) to a mob or object or room...
            else if(c.command == 'T') {
                if(c.arg1 == 0 && registry.valid(tmob)) {
                    // add trigger c.arg2 to tmob...
                } else if(c.arg1 == 1 && registry.valid(tobj)) {
                    // add trigger c.arg2 to tobj...
                } else if(c.arg1 == 2) {
                    // attach script c.arg2 to room c.arg3...
                }
            }

            else if(c.command == 'V') {
                if(c.arg1 == 0 && registry.valid(tmob)) {
                    // add trigger c.arg2 to tmob...
                } else if(c.arg1 == 1 && registry.valid(tobj)) {
                    // add trigger c.arg2 to tobj...
                } else if(c.arg1 == 2) {
                    // attach script c.arg2 to room c.arg3...
                }
            }

            else {
                logger->error("Unknown command {} in dg_script.", c.command);
                last_cmd = false;
                tmob = entt::null;
                tobj = entt::null;
                continue;
            }

        }

    }
}