#include "dbatk/commands/admin.h"
#include "dbatk/api.h"
#include "dbatk/components.h"
#include "dbatk/search.h"
#include "dbatk/zone.h"
#include "dbatk/operations/information.h"

namespace dbat::cmd {

    void AdmTeleport::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        // lsargs is who or what's being teleported and rsargs is the destination...
        auto lsargs = input["lsargs"];
        auto rsargs = input["rsargs"];

        if (lsargs.empty() || rsargs.empty()) {
            sendLine(ent, "Usage: .teleport <target>=<destination>");
            return;
        }

        auto lsearch = Search(ent).useId(true).useSelf(true);
        auto loc = registry.try_get<Location>(ent);
        if(loc) {
            lsearch.in(*loc);
        }
        lsearch.inventory(ent);


        auto target = lsearch.find(lsargs);
        if (target.empty()) {
            sendLine(ent, "No target found.");
            return;
        }

        auto rsearch = Search(ent).useId(true);
        // We must split off an optional /section from the end of rsargs.
        auto slash = rsargs.find('/');
        std::string pos;
        if (slash != std::string::npos) {
            pos = rsargs.substr(slash + 1);
            rsargs = rsargs.substr(0, slash);
        }
        boost::trim(rsargs);
        boost::trim(pos);

        auto destination = rsearch.find(rsargs);
        if (destination.empty()) {
            sendLine(ent, "No destination found.");
            return;
        }

        MoveParams params;
        params.dest = validDestination(destination[0], pos);
        if(!params.dest) {
            sendLine(ent, "Invalid destination.");
            return;
        }
        params.moveType = MoveType::Traverse;
        params.traverseType = TraverseType::System;
        params.force = true;
        params.mover = ent;

        auto [res, err] = moveTo(target[0], params);

        if (res) {
            sendLine(ent, "Teleported.");
        } else {
            sendLine(ent, fmt::format("Teleport failed: {}", err.value()));
        }

    }

    void AdmGoto::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto args = input["args"];

        if(args.empty()) {
            sendLine(ent, "Usage: goto <destination>");
            return;
        }

        // parse args to an int64_t..
        int64_t id;
        try {
            id = std::stoll(args);
        } catch(std::invalid_argument &e) {
            sendLine(ent, "Invalid destination.");
            return;
        } catch(std::out_of_range &e) {
            sendLine(ent, "Invalid destination.");
            return;
        }

        auto find = legacyRooms.find(id);
        if(find == legacyRooms.end()) {
            sendLine(ent, "Invalid destination.");
            return;
        }

        MoveParams params;
        params.dest = find->second;
        params.moveType = MoveType::Traverse;
        params.traverseType = TraverseType::System;
        params.force = true;
        params.mover = ent;

        auto [res, err] = moveTo(ent, params);

        if (res) {
            sendLine(ent, "Teleported.");
        } else {
            sendLine(ent, fmt::format("Teleport failed: {}", err.value()));
        }

    }

    void AdmAt::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto loc = registry.try_get<Location>(ent);

        if(!loc) {
            sendLine(ent, "You must be in a room to use this command.");
            return;
        }

        auto copyLoc = *loc;

        auto args = input["args"];

        if(args.empty()) {
            sendLine(ent, "Usage: at <location> <command>");
            return;
        }

        // Let's parse args with a whitespace split...
        auto space = args.find(' ');
        if(space == std::string::npos) {
            sendLine(ent, "Usage: at <location> <command>");
            return;
        }

        auto location = args.substr(0, space);
        auto command = args.substr(space + 1);
        boost::trim(location);
        boost::trim(command);
        if(command.empty()) {
            sendLine(ent, "Usage: at <location> <command>");
            return;
        }

        // parse args to an int64_t..
        int64_t id;
        try {
            id = std::stoll(location);
        } catch(std::invalid_argument &e) {
            sendLine(ent, "Invalid destination.");
            return;
        } catch(std::out_of_range &e) {
            sendLine(ent, "Invalid destination.");
            return;
        }

        auto find = legacyRooms.find(id);
        if(find == legacyRooms.end()) {
            sendLine(ent, "Invalid destination.");
            return;
        }

        MoveParams params;
        params.dest = find->second;
        params.moveType = MoveType::Traverse;
        params.traverseType = TraverseType::System;
        params.force = true;
        params.mover = ent;
        params.quiet = true;

        if(auto [res, err] = moveTo(ent, params); res) {
            // We're in the room now, so we can execute the command.
            executeCommand(ent, command);
            // and now we move back.
            params.dest = copyLoc;
            moveTo(ent, params);
        } else {
            sendLine(ent, fmt::format("At failed: {}", err.value()));
        }

    }



    void AdmList::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto args = input["args"];

        if(args.empty()) {
            sendLine(ent, "Usage: .list <command>");
            return;
        }

        if(boost::iequals(args, "stats")) {
            std::size_t items, npcs;
            items = registry.view<Item>().size();
            npcs = registry.view<NPC>().size();
        }

        else if(boost::iequals(args, "population")) {

            std::unordered_map<std::size_t, std::pair<std::string, std::size_t>> items, npcs;

            std::list<std::pair<std::string, std::size_t>> npcList, itemList;

            auto v1 = registry.view<NPCVnum>();
            for(auto e : v1) {
                auto &vn = registry.get<NPCVnum>(e);
                auto &v = npcs[vn.data];
                if(v.first.empty()) {
                    v.first = fmt::format("{} ({})", getName(e), vn.data);
                }
                v.second++;
            }

            // We want to sort the npcs by count so the highest count is first in npcList...
            for(auto &p : npcs) {
                npcList.push_back(p.second);
            }
            npcList.sort([](const std::pair<std::string, std::size_t> &a, const std::pair<std::string, std::size_t> &b) {
                return a.second > b.second;
            });


            auto v2 = registry.view<ItemVnum>();
            for(auto e : v2) {
                auto &vn = registry.get<ItemVnum>(e);
                auto &v = items[vn.data];
                if(v.first.empty()) {
                    v.first = fmt::format("{} ({})", getName(e), vn.data);
                }
                v.second++;
            }

            // We want to sort the items by count so the highest count is first in itemList...
            for(auto &p : items) {
                itemList.push_back(p.second);
            }
            itemList.sort([](const std::pair<std::string, std::size_t> &a, const std::pair<std::string, std::size_t> &b) {
                return a.second > b.second;
            });

            std::vector<std::string> lines;

            lines.emplace_back(fmt::format("NPCs: {}", npcList.size()));
            for(auto &p : npcList) {
                lines.emplace_back(fmt::format("  {:>4} - {}", p.second, p.first));
            }

            lines.emplace_back(fmt::format("Items: {}", itemList.size()));
            for(auto &p : itemList) {
                lines.emplace_back(fmt::format("  {:>4} - {}", p.second, p.first));
            }

            sendLine(ent, boost::join(lines, "\n"));

        } else {
            sendLine(ent, "Invalid command.");
            return;
        }
    }

    void AdmZone::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto sw = boost::to_lower_copy(input["switches"]);

        std::set<std::string> switches;
        boost::split(switches, sw, boost::is_any_of("/"));

        if(!switches.empty()) {
            if(switches.contains("list")) {
                sendLine(ent, op::listZones());
                return;
            } else {
                sendLine(ent, "Invalid switch.");
                return;
            }
        }
    }

    void AdmCheat::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto args = input["args"];

        if(args.empty()) {
            sendLine(ent, "Usage: .cheat <mode>");
            return;
        }

        auto admId = aflags::getAdminFlagId(args, false);
        if(!admId) {
            sendLine(ent, "Invalid mode.");
            return;
        }

        auto &flag = aflags::adminFlags[admId.value()];

        auto &af = registry.get_or_emplace<AdminFlags>(ent);
        auto result = af.data.test(flag->getId()) ? "Disabled" : "Enabled";
        sendLine(ent, fmt::format("{} Cheat Mode: {}", result, flag->getName()));
        af.data.flip(flag->getId());

    }

    void AdmExamine::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto args = input["args"];

        if(args.empty()) {
            sendLine(ent, "Usage: .examine <target>");
            return;
        }

        Search sr(ent);
        sr.useId(true);
        if(auto loc = registry.try_get<Location>(ent); loc) {
            sr.in(*loc);
        }

        auto found = sr.find(args);
        if(found.empty()) {
            sendLine(ent, "No such target.");
            return;
        }

        sendLine(ent, op::renderExamine(found.front(), ent));

    }

    void registerAdminCommands() {
        registerCommand(std::make_shared<AdmTeleport>());
        registerCommand(std::make_shared<AdmGoto>());
        registerCommand(std::make_shared<AdmAt>());
        registerCommand(std::make_shared<AdmList>());
        registerCommand(std::make_shared<AdmZone>());
        registerCommand(std::make_shared<AdmCheat>());
        registerCommand(std::make_shared<AdmExamine>());

    }

}