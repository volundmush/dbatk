#include "dbatk/commands/admin.h"
#include "dbatk/api.h"
#include "dbatk/components.h"
#include "dbatk/search.h"

namespace dbat::cmd {

    void AdmTeleport::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        // lsargs is who or what's being teleported and rsargs is the destination...
        auto lsargs = input["lsargs"];
        auto rsargs = input["rsargs"];

        if (lsargs.empty() || rsargs.empty()) {
            sendText(ent, "Usage: @teleport <target>=<destination>");
            return;
        }

        auto lsearch = Search(ent).useId(true).useSelf(true);
        auto loc = registry.try_get<Location>(ent);
        if(loc) {
            if(loc->locationType == LocationType::Area) {
                auto &area = registry.get<Area>(loc->data);
                auto &room = area.data[loc->x];
                lsearch.room(room);
            }
        }
        lsearch.in(ent);

        auto target = lsearch.find(lsargs);
        if (target.empty()) {
            sendText(ent, "No target found.");
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
            sendText(ent, "No destination found.");
            return;
        }

        MoveParams params;
        params.dest = validDestination(destination[0], pos);
        if(!params.dest) {
            sendText(ent, "Invalid destination.");
            return;
        }
        params.moveType = MoveType::Traverse;
        params.traverseType = TraverseType::System;
        params.force = true;
        params.mover = ent;

        auto [res, err] = moveTo(target[0], params);

        if (res) {
            sendText(ent, "Teleported.");
        } else {
            sendText(ent, fmt::format("Teleport failed: {}", err.value()));
        }

    }

    void AdmGoto::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto args = input["args"];

        if(args.empty()) {
            sendText(ent, "Usage: goto <destination>");
            return;
        }

    }

    void registerAdminCommands() {

    }

}