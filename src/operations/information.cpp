#include "dbatk/operations/information.h"
#include "dbatk/components.h"
#include "dbatk/api.h"
#include "fmt/args.h"
#include "dbatk/color.h"
#include "dbatk/zone.h"
#include "dbatk/dgscript.h"
#include "dbatk/session.h"
#include "fmt/format.h"

namespace dbat::op {

    static const std::string compassTemplate = "      |@C {N} @n|\n"
                                               "| @C{NW: >2}@n| |@Y{U: ^3}@n| |@C{NE: <3}@n|\n"
                                               "| @C{W: >2}@n| |@m{I: ^3}@n| |@C{E: <3}@n|\n"
                                               "| @C{SW: >2}@n| |@Y{D:^3}@n| |@C{SE: <3}@n|\n"
                                               "      |@C {S} @n|";

    static const std::string roomHeader = "O----------------------------------------------------------------------O";
    static const std::string roomSubheader = "------------------------------------------------------------------------";
    static std::unordered_map<dir::DirectionId, std::string> exitMap = {
            {dir::North,     "N"},
            {dir::South,     "S"},
            {dir::East,      "E"},
            {dir::West,      "W"},
            {dir::Up,        "U"},
            {dir::Down,      "D"},
            {dir::Northeast, "NE"},
            {dir::Northwest, "NW"},
            {dir::Southeast, "SE"},
            {dir::Southwest, "SW"},
            {dir::Inside,    "I"},
            {dir::Outside,   "I"},
    };

    static std::vector<std::string> generateCompass(entt::entity ent, entt::entity looker) {
        auto excomp = registry.try_get<Exits>(ent);

        fmt::dynamic_format_arg_store<fmt::format_context> args;
        std::unordered_map<std::string, std::string> exArgs;
        for(auto &[name, key] : exitMap) {
            exArgs[key] = " ";
        }
        if(excomp)
            for (auto &[id, key]: exitMap) {
                // check if exits contains a matching name, and if so, we'll create an arg for it using the key.
                // If it doesn't contain a match, then we set the arg to an empty space.
                if (excomp->data.contains(id)) {
                    if(id == dir::Outside) {
                        exArgs["I"] = "OUT";
                    } else {
                        exArgs[key] = key;
                    }
                }
            }

        for(auto &[key, value]: exArgs) {
            args.push_back(fmt::arg(key.c_str(), value));
        }

        auto compass = fmt::vformat(compassTemplate, args);
        // Now split compass into lines and return the vector.
        std::vector<std::string> lines;
        boost::split(lines, compass, boost::is_any_of("\n"));
        return lines;
    };

    std::vector<std::string> generateAutomap(entt::entity ent, entt::entity looker, int minY = -2, int maxY = 2,
                                             int minX = -4, int maxX = 4) {
        std::set<entt::entity> visited;

        // Curmap's outer map is Y and inner map is X. I know that's backwards but Y is rows and X is columns so...
        std::map<int, std::map<int, std::string>> curMap;

        // First, we must fill out every room in the map with a blank space.
        for (int y = minY;y <=maxY;y++) {
            for (int x = minX; x <= maxX; x++) {
                curMap[y][x] = " ";
            }
        }

        std::function<void(entt::entity, int, int)> scan = [&](entt::entity room, int curX, int curY) {
            if (visited.contains(room)) return;
            visited.insert(room);
            auto &t = registry.get_or_emplace<Terrain>(room);
            curMap[curY][curX] = terrain::terrainRegistry[t.data]->getMapTile();

            auto excomp = registry.try_get<Exits>(room);

            // Now we must iterate over all exits and scan those which correlate with cardinal directions: N, S, E, W and diagonals.
            if(excomp)
                for(auto &[dir, dest] : excomp->data) {
                    if(dest.locationType != LocationType::Area) continue;
                    entt::entity area = dest.data;
                    if(area == entt::null) {
                        auto &r = registry.get_or_emplace<Room>(room);
                        area = r.obj.getObject();
                    }
                    auto &ar = registry.get_or_emplace<Area>(area);
                    auto &targetRoom = ar.data[dest.x];

                    switch(dir) {
                        case dir::North:
                            if((curY + 1) <= maxY) scan(targetRoom, curX, curY + 1);
                            break;
                        case dir::South:
                            if((curY - 1) >= minY) scan(targetRoom, curX, curY - 1);
                            break;
                        case dir::East:
                            if((curX + 1) <= maxX) scan(targetRoom, curX + 1, curY);
                            break;
                        case dir::West:
                            if((curX - 1) >= minX) scan(targetRoom, curX - 1, curY);
                            break;
                        case dir::Northeast:
                            if((curX + 1) <= maxX && (curY + 1) <= maxY) scan(targetRoom, curX + 1, curY + 1);
                            break;
                        case dir::Northwest:
                            if((curX - 1) >= minX && (curY + 1) <= maxY) scan(targetRoom, curX - 1, curY + 1);
                            break;
                        case dir::Southeast:
                            if((curX + 1) <= maxX && (curY - 1) >= minY) scan(targetRoom, curX + 1, curY - 1);
                            break;
                        case dir::Southwest:
                            if((curX - 1) >= minX && (curY - 1) >= minY) scan(targetRoom, curX - 1, curY - 1);
                            break;
                        default:
                            // No other exit types are respected... yet...
                            break;
                    }
                }


        };
        scan(ent, 0, 0);
        curMap[0][0] = "@RX@n";

        // Now we must convert the map into a vector of strings. The highest Y value in the map must be the first row,
        // and the lowest Y value must be the last row. The lowest X value is the first column, and the highest X value
        // is the last. Standard cartesian grid stuff, but our weird map-in-a-map means the storage order is reversed.
        std::vector<std::string> lines;
        for (int y = maxY; y >= minY; y--) {
            std::string line;
            for (int x = minX; x <= maxX; x++) {
                line += curMap[y][x];
            }
            lines.emplace_back(line);
        }
        return lines;
    }


    static std::vector<std::string> mapLegend;

    static std::vector<std::string> generateMapLegend(entt::entity ent, entt::entity looker) {

        if(mapLegend.empty()) {
            std::vector<std::string> legend;
            for(auto t : terrain::terrainRegistry) {
                if(t)
                    if(t->displayOnLegend()) legend.emplace_back(fmt::format("{}: @W{}@n", t->getMapTile(), t->getName()));
            }
            legend.emplace_back("@RX@n: @WYou@n");

            for (int i = 0; i < legend.size(); i += 3) {
                std::string line;
                line += legend[i];
                if (i + 1 < legend.size()) {
                    line += ", " + legend[i + 1];
                }
                if (i + 2 < legend.size()) {
                    line += ", " + legend[i + 2];
                }
                mapLegend.push_back(line);
            }

        }
        return mapLegend;
    }

    bool renderAbsoluteLocation(RenderInfo &info) {
        // this is for looking at a thing directly. It should show the LookDescription and other relevant details,
        // depending on what info.data IS.
        auto startLines = info.output.size();
        auto ldesc = getLookDescription(info.loc.data);
        if(!ldesc.empty()) {
            info.output.emplace_back(ldesc);
        }

        if(registry.any_of<Player>(info.loc.data)) {
            // We are rendering a player.
        }

        if(registry.any_of<NPC>(info.loc.data)) {
            // We are rendering an NPC.
        }

        if(registry.any_of<Character>(info.loc.data)) {
            // all character-related stuff goes here...

            if(registry.any_of<Equipment>(info.loc.data)) {
                RenderInfo eq = info;
                eq.loc.locationType = LocationType::Equipment;
                if(renderEquipmentLocation(eq)) {
                    info.output.emplace_back("\n");
                    info.output.emplace_back("\n");
                }
            }

            if(info.matrix && registry.any_of<Inventory>(info.loc.data)) {
                info.output.emplace_back("Matrix inventory view:");
                RenderInfo inv = info;
                inv.loc.locationType = LocationType::Inventory;
                if(renderInventoryLocation(inv)) {
                    info.output.emplace_back("\n");
                    info.output.emplace_back("\n");
                }
            }

        }
        return startLines == info.output.size();

    }

    bool renderInventoryLocation(RenderInfo &info) {
        auto startLines = info.output.size();
        if(auto inv = registry.try_get<Inventory>(info.loc.data); inv) {
            if(!inv->data.empty()) {
                for(auto &item : inv->data) {
                    info.output.emplace_back(renderItemListing(item, info.looker, info.matrix));
                }
            } else {
                info.output.emplace_back("Nothing.");
            }
        }
        return startLines == info.output.size();
    }

    bool renderEquipmentLocation(RenderInfo &info) {
        auto startLines = info.output.size();
        if(auto eq = registry.try_get<Equipment>(info.loc.data); eq) {
            if(eq->data.empty()) return false;
            for(auto &[slotId, item] : eq->data) {
                // TODO: handle slot displays...
                info.output.emplace_back(renderItemListing(item, info.looker, info.matrix));
            }
        }
        return startLines == info.output.size();
    }

    bool renderAreaLocation(RenderInfo &info) {
        auto startLines = info.output.size();
        auto room = info.loc.getRoom();
        if(room == entt::null) {
            logger->error("Room is null in renderAreaLocation");
            info.output.emplace_back("You are nowhere. This is bad. You should talk to admin.");
            return startLines == info.output.size();
        }
        info.output.emplace_back(roomHeader);
        auto &r = registry.get<Room>(room);
        auto ancestors = getAncestors(r.obj.getObject());
        if(info.matrix) {
            std::vector<std::string> anames;
            for(auto a : ancestors) {
                anames.emplace_back(renderDisplayName(a, info.looker, info.matrix));
            }
            anames.emplace_back(renderDisplayName(room, info.looker, info.matrix));
            info.output.emplace_back(fmt::format("Location: {}", fmt::join(anames, " -> ")));
            std::vector<std::string> msec;
            if(auto ter = registry.try_get<Terrain>(room); ter) {
                auto &t = terrain::terrainRegistry[ter->data];
                msec.emplace_back(fmt::format("Terrain: [{:>2} - {}]", t->getId(), t->getName()));
            }
            if(auto rflag = registry.try_get<RoomFlags>(room); rflag) {
                std::vector<std::string> flags;
                for(const auto &r : rflags::roomFlags) {
                    if(rflag->data.test(r->getId())) {
                        flags.emplace_back(r->getName());
                    }
                }
                if(!flags.empty()) {
                    msec.emplace_back(fmt::format("Room Flags: [@G{}@n]", fmt::join(flags, ", ")));
                }
            }
            if(!msec.empty()) {
                info.output.emplace_back(fmt::format("{}", fmt::join(msec, " ")));
            }
        } else {
            info.output.emplace_back(fmt::format("Location: {}", renderDisplayName(room, info.looker, info.matrix)));
        }
        info.output.emplace_back(roomHeader);
        auto ldesc = getLookDescription(room);
        if(!ldesc.empty()) {
            info.output.emplace_back(ldesc);
            info.output.emplace_back(roomSubheader);
        }
        info.output.emplace_back("       Compass        AutoMap                  Map Key");
        info.output.emplace_back("@R -----------------   ---------    -------------------------------------");
        auto automap = generateAutomap(room, info.looker);
        auto compass = generateCompass(room, info.looker);
        auto legend = generateMapLegend(room, info.looker);
        // Now we need to know the maximum amount of lines for each of these three things.
        auto maxLines = std::max({automap.size(), compass.size(), legend.size()});
        for(int i = 0; i < maxLines; i++) {
            std::string compassLine = i < compass.size() ? compass[i] : "";
            std::string automapLine = i < automap.size() ? automap[i] : "";
            std::string legendLine = i < legend.size() ? legend[i] : "";
            // now we need to define sep1. sep1 is a sequence of 20-(length of compassLine minus colorcode)...
            auto sep1 = std::string(20 - stripAnsi(compassLine).size(), ' ');
            // sep2 is a sequence of 14-(length of automapLine minus colorcode)...
            auto sep2 = std::string(14 - stripAnsi(automapLine).size(), ' ');
            info.output.emplace_back(fmt::format(" {}{}{}{}{}", compassLine, sep1, automapLine, sep2, legendLine));
        }
        info.output.emplace_back(roomSubheader);
        auto rcon = registry.try_get<RoomContents>(room);
        if(rcon) {
            for(auto e : rcon->data) {
                if(e == info.looker) continue;
                if(!(info.matrix || canDetect(info.looker, e, 0))) continue;
                info.output.emplace_back(renderRoomLine(e, info.looker, info.matrix));
            }
        }
        return startLines == info.output.size();

    }

    bool renderExpanseLocation(RenderInfo &info) {
        // This should be very similar to renderAreaLocation in presentation, but there are some
        // major differences. Expanses are coordinate based, but players don't see the coordinates.
        auto startLines = info.output.size();
        GridPoint gp(info.loc.x, info.loc.y, info.loc.z);
        // TOOD: this
        return startLines == info.output.size();
    }

    bool renderMapLocation(RenderInfo &info) {
        auto startLines = info.output.size();
        // TOOD: this
        return startLines == info.output.size();
    }

    bool renderSpaceLocation(RenderInfo &info) {
        auto startLines = info.output.size();
        // TOOD: this
        return startLines == info.output.size();
    }

    std::string renderLocation(const Location& loc, entt::entity looker) {
        std::vector<std::string> output;
        RenderInfo info{.output=output};
        info.looker = looker;
        info.loc = loc;
        if(auto af = registry.try_get<AdminFlags>(looker); af) {
            info.matrix = af->data.test(aflags::MATRIX);
        }

        switch(info.loc.locationType) {
            case LocationType::Absolute:
                renderAbsoluteLocation(info);
                break;
            case LocationType::Inventory:
                renderInventoryLocation(info);
                break;
            case LocationType::Equipment:
                renderEquipmentLocation(info);
                break;
            case LocationType::Area:
                renderAreaLocation(info);
                break;
            case LocationType::Expanse:
                renderExpanseLocation(info);
                break;
            case LocationType::Map:
                renderMapLocation(info);
                break;
            case LocationType::Space:
                renderSpaceLocation(info);
                break;
        }

        return boost::algorithm::join(output, "@n\n");
    }

    std::string listZones() {
        std::vector<std::string> lines;
        for(auto &[id, z] : zones) {
            lines.emplace_back(fmt::format("{:>4} {}@n Range: {:>5}-{:>5}", id, z.name, z.bot, z.top));
        }
        return boost::join(lines, "\n");
    }

    std::string renderPrefixData(entt::entity ent) {
        std::vector<std::string> prefixes;

        if(auto room = registry.try_get<Room>(ent); room) {
            prefixes.emplace_back(fmt::format("@G[#{}/{}]@n", room->obj.index, room->id));
        } else if (auto oid = registry.try_get<ObjectId>(ent); oid){
            // if it's not a room, it should have an ObjectId.
            prefixes.emplace_back(fmt::format("@G[#{}]@n", oid->index));
        }

        if(auto dg = registry.try_get<DgScripts>(ent); dg) {
            // we want a nice green section that looks like [DG:1,2,3] for any script ids.
            std::vector<std::string> scriptIds;
            for(const auto& s : dg->scripts) {
                scriptIds.emplace_back(std::to_string(s->prototype->id));
            }
            if(!scriptIds.empty()) {
                prefixes.emplace_back(fmt::format("@G[DG:{}]@n", fmt::join(scriptIds, ",")));
            }
        }

        if(auto proto = registry.try_get<Prototype>(ent); proto) {
            prefixes.emplace_back(fmt::format("@G[P:{}]@n", proto->data));
        }

        if(auto key = registry.try_get<Keywords>(ent); key) {
            if(!key->data.empty()) {
                prefixes.emplace_back(fmt::format("({})@n", key->data));
            }
        }

        if(!prefixes.empty()) {
            return fmt::format("{}", fmt::join(prefixes, " "));
        } else {
            return "";
        }
    }

    std::string renderRoomLine(entt::entity ent, entt::entity looker, bool matrix) {
        auto name = getRoomLine(ent, looker);
        if(matrix) {
            auto prefixes = renderPrefixData(ent);
            if(!prefixes.empty()) {
                name = fmt::format("{} {}", prefixes, name);
            }
        }
        return name;
    }

    std::string renderItemListing(entt::entity ent, entt::entity looker, bool matrix) {
        auto name = getDisplayName(ent, looker, matrix);
        if(matrix) {
            auto prefixes = renderPrefixData(ent);
            if(!prefixes.empty()) {
                name = fmt::format("{} {}", prefixes, name);
            }
        }
        // TODO: Add suffixes for item details...
        return name;
    }

    std::string renderDisplayName(entt::entity ent, entt::entity looker, bool matrix) {
        auto name = getDisplayName(ent, looker, matrix);
        if(matrix) {
            auto prefixes = renderPrefixData(ent);
            if(!prefixes.empty()) {
                name = fmt::format("{} {}", prefixes, name);
            }
        }
        return name;
    }

    std::string renderExamine(entt::entity ent, entt::entity looker) {
        std::vector<std::string> lines;
        bool matrix = false;
        if(auto af = registry.try_get<AdminFlags>(looker); af) {
            matrix = af->data.test(aflags::MATRIX);
        }

        if(auto obj = registry.try_get<ObjectId>(ent); obj) {
            lines.emplace_back(fmt::format("Object: {}", obj->toString()));
        }

        if(auto room = registry.try_get<Room>(ent); room) {
            auto o = room->obj.getObject();
            lines.emplace_back(fmt::format("Room {} of Area: {}", room->id, renderDisplayName(o, looker, matrix)));
        }

        if(auto proto = registry.try_get<Prototype>(ent); proto) {
            lines.emplace_back(fmt::format("Prototype: {}", proto->data));
        }

        if(auto str = registry.try_get<Name>(ent); str) {
            lines.emplace_back(fmt::format("Name: {}", str->data, str->clean));
            if(!boost::equals(str->data, str->clean)) {
                lines.emplace_back(fmt::format("  (Clean): {}", str->clean));
            }
        }

        if(auto str = registry.try_get<Keywords>(ent); str) {
            lines.emplace_back(fmt::format("Keywords: {}", str->data));
        }

        if(auto str = registry.try_get<RoomDescription>(ent); str) {
            lines.emplace_back(fmt::format("RoomDescription: {}", str->data));
            if(!boost::equals(str->data, str->clean)) {
                lines.emplace_back(fmt::format("  (Clean): {}", str->clean));
            }
        }

        if(auto str = registry.try_get<LookDescription>(ent); str) {
            lines.emplace_back(fmt::format("LookDescription: {}", str->data));
            if(!boost::equals(str->data, str->clean)) {
                lines.emplace_back(fmt::format("  (Clean): {}", str->clean));
            }
        }

        if(auto sess = registry.try_get<SessionHolder>(ent); sess) {
            if(sess->data) {
                lines.emplace_back(fmt::format("Session: {} Mode {}, {} connections", sess->data->getId().index, sess->sessionMode, sess->data->getNumClients()));
            }
        }

        if(auto par = registry.try_get<Parent>(ent); par) {
            if(registry.valid(par->data)) {
                lines.emplace_back(fmt::format("Parent: {}", renderDisplayName(par->data, looker, matrix)));
            }
        }
        if(auto children = registry.try_get<Children>(ent); children) {
            if(!children->data.empty()) {
                std::vector<std::string> childNames;
                for(auto child : children->data) {
                    if(registry.valid(child)) {
                        childNames.emplace_back(renderDisplayName(child, looker, matrix));
                    }
                }
                lines.emplace_back(fmt::format("Children: {}", fmt::join(childNames, ", ")));
            }
        }

        if(auto own = registry.try_get<Owner>(ent); own) {
            if(registry.valid(own->data)) {
                lines.emplace_back(fmt::format("Parent: {}", renderDisplayName(own->data, looker, matrix)));
            }
        }
        if(auto ass = registry.try_get<Assets>(ent); ass) {
            if(!ass->data.empty()) {
                std::vector<std::string> assNames;
                for(auto a : ass->data) {
                    if(registry.valid(a)) {
                        assNames.emplace_back(renderDisplayName(a, looker, matrix));
                    }
                }
                lines.emplace_back(fmt::format("Assets: {}", fmt::join(assNames, ", ")));
            }
        }

        if(auto loc = registry.try_get<Location>(ent); loc) {
            if(registry.valid(loc->data)) {
                lines.emplace_back(fmt::format("Location: {}", renderDisplayName(loc->data, looker, matrix)));
                if(loc->locationType == LocationType::Absolute) {
                    lines.emplace_back("  (Absolute)");
                }
                else if(loc->locationType == LocationType::Inventory) {
                    lines.emplace_back("  (Inventory)");
                }
                else if(loc->locationType == LocationType::Equipment) {
                    lines.emplace_back(fmt::format("  (Equipment: Slot {})", loc->x));
                }
                else if(loc->locationType == LocationType::Area) {
                    auto room = loc->getRoom();
                    lines.emplace_back(fmt::format("  (Area Room {}: {})", loc->x, renderDisplayName(room, looker, matrix)));
                }
                else if(loc->locationType == LocationType::Expanse) {
                    lines.emplace_back(fmt::format("  (Expanse Room: {},{},{})", loc->x, loc->y, loc->z));
                }
                else if(loc->locationType == LocationType::Map) {
                    lines.emplace_back(fmt::format("  (Map Room: {},{},{})", loc->x, loc->y, loc->z));
                }
                else if(loc->locationType == LocationType::Space) {
                    lines.emplace_back(fmt::format("  (Space Coordinates: {},{},{})", loc->x, loc->y, loc->z));
                }
            }
        }

        if(auto inv = registry.try_get<Inventory>(ent); inv) {
            if(!inv->data.empty()) {
                lines.emplace_back(fmt::format("Inventory: {} Items", inv->data.size()));
                for(auto e : inv->data) {
                    lines.emplace_back(fmt::format("  {}", renderItemListing(e, looker, matrix)));
                }
            }
        }

        if(auto eq = registry.try_get<Equipment>(ent) ; eq) {
            if(!eq->data.empty()) {
                lines.emplace_back(fmt::format("Equipment: {} Items", eq->data.size()));
                for(auto [slot, e] : eq->data) {
                    lines.emplace_back(fmt::format("  {} - {}", slot, renderItemListing(e, looker, matrix)));
                }
            }
        }

        if(auto ar = registry.try_get<Area>(ent); ar) {
            lines.emplace_back(fmt::format("Area: {} Rooms", ar->data.size()));
            for(auto [id, r] : ar->data) {
                lines.emplace_back(fmt::format("  {} - {}", id, renderDisplayName(r, looker, matrix)));
            }
        }

        if(auto rc = registry.try_get<RoomContents>(ent); rc) {
            if(!rc->data.empty()) {
                lines.emplace_back(fmt::format("RoomContents: {} Objects", rc->data.size()));
                for(auto e : rc->data) {
                    lines.emplace_back(fmt::format("  {}", renderRoomLine(e, looker, matrix)));
                }
            }
        }

        if(auto ex = registry.try_get<Expanse>(ent); ex) {
            lines.emplace_back(fmt::format("Expanse: {} Points of Interest", ex->poi.size()));
            lines.emplace_back(fmt::format("  Bounds: X[{},{}], Y[{},{}], Z[{},{}]", ex->minX, ex->maxX, ex->minY, ex->maxY, ex->minZ, ex->maxZ));
            for(auto &[gp, e] : ex->poi) {
                lines.emplace_back(fmt::format("  ({},{},{}) - {}", gp.x,gp.y,gp.z, renderDisplayName(e, looker, matrix)));
            }
        }

        if(auto ma = registry.try_get<Map>(ent); ma) {
            lines.emplace_back(fmt::format("Map: {} Points of Interest", ma->poi.size()));
            lines.emplace_back(fmt::format("  Bounds: X[{},{}], Y[{},{}], Z[{},{}]", ma->minX, ma->maxX, ma->minY, ma->maxY, ma->minZ, ma->maxZ));
            for(auto &[gp, e] : ma->poi) {
                lines.emplace_back(fmt::format("  ({},{},{}) - {}", gp.x,gp.y,gp.z, renderDisplayName(e, looker, matrix)));
            }
        }

        if(auto gc = registry.try_get<GridContents>(ent); gc) {
            lines.emplace_back(fmt::format("GridContents: {} Objects", gc->data.size()));
            for(auto &[gp, v] : gc->data) {
                if(!v.empty()) {
                    lines.emplace_back(fmt::format("  ({},{},{})", gp.x,gp.y, gp.z));
                    for(auto e : v) {
                        lines.emplace_back(fmt::format("    {}", renderRoomLine(e, looker, matrix)));
                    }
                }
            }
        }

        if(auto sp = registry.try_get<Space>(ent); sp) {
            lines.emplace_back(fmt::format("Space: {} Points of Interest", sp->poi.size()));
            lines.emplace_back(fmt::format("  Bounds: X[{},{}], Y[{},{}], Z[{},{}]", sp->minX, sp->maxX, sp->minY, sp->maxY, sp->minZ, sp->maxZ));
            for(auto &[gp, e] : sp->poi) {
                lines.emplace_back(fmt::format("  ({},{},{}) - {}", gp.x,gp.y,gp.z, renderDisplayName(e, looker, matrix)));
            }
        }

        if(auto sc = registry.try_get<SectorContents>(ent); sc) {
            lines.emplace_back(fmt::format("SectorContents: {} Objects", sc->data.size()));
            for(auto &[gp, v] : sc->data) {
                if(!v.empty()) {
                    lines.emplace_back(fmt::format("  ({},{},{})", gp.x,gp.y,gp.z));
                    for(auto e : v) {
                        lines.emplace_back(fmt::format("    {}", renderItemListing(e, looker, matrix)));
                    }
                }
            }
        }

        if(registry.any_of<Item>(ent)) {
            lines.emplace_back(fmt::format("Item: true"));
        }

        if(registry.any_of<Character>(ent)) {
            lines.emplace_back(fmt::format("Character: true"));
        }

        if(registry.any_of<NPC>(ent)) {
            lines.emplace_back(fmt::format("NPC: true"));
        }

        if(registry.any_of<Vehicle>(ent)) {
            lines.emplace_back(fmt::format("Vehicle: true"));
        }

        if(auto p = registry.try_get<Player>(ent); p) {
            lines.emplace_back(fmt::format("Player Character of Account: {}", p->accountId));
        }

        if(auto r = registry.try_get<Race>(ent); r) {
            lines.emplace_back(fmt::format("Race: {} - {}", static_cast<uint8_t>(r->data), race::races[r->data]->getName()));
        }

        if(auto s = registry.try_get<Sensei>(ent); s) {
            lines.emplace_back(fmt::format("Sensei: {} - {}", static_cast<uint8_t>(s->data), sensei::senseis[s->data]->getName()));
        }

        if(auto p = registry.try_get<Position>(ent); p) {
            lines.emplace_back(fmt::format("Position: {} - {}", static_cast<uint8_t>(p->data), position::positions[p->data]->getName()));
        }

        if(auto s = registry.try_get<Sex>(ent); s) {
            lines.emplace_back(fmt::format("Sex: {} - {}", static_cast<uint8_t>(s->data), sex::sexes[s->data]->getName()));
        }

        if(auto t = registry.try_get<Terrain>(ent); t) {
            lines.emplace_back(fmt::format("Terrain: {} - {}", static_cast<uint8_t>(t->data), terrain::terrainRegistry[t->data]->getName()));
        }

        if(auto af = registry.try_get<AdminFlags>(ent); af) {
            if(af->data.any()) {
                std::vector<std::string> flags;
                for(auto &f : aflags::adminFlags) {
                    if(af->data.test(f->getId())) {
                        flags.emplace_back(fmt::format("{} - {}", f->getId(), f->getName()));
                    }
                }
                lines.emplace_back(fmt::format("AdminFlags: {}", boost::algorithm::join(flags, ", ")));
            }
        }

        if(auto of = registry.try_get<ObjectFlags>(ent); of) {
            if(of->data.any()) {
                std::vector<std::string> flags;
                for(auto &f : oflags::objectFlags) {
                    if(of->data.test(f->getId())) {
                        flags.emplace_back(fmt::format("{} - {}", f->getId(), f->getName()));
                    }
                }
                lines.emplace_back(fmt::format("ObjectFlags: {}", boost::algorithm::join(flags, ", ")));
            }
        }

        if(auto rf = registry.try_get<RoomFlags>(ent); rf) {
            if(rf->data.any()) {
                std::vector<std::string> flags;
                for(auto &f : rflags::roomFlags) {
                    if(rf->data.test(f->getId())) {
                        flags.emplace_back(fmt::format("{} - {}", f->getId(), f->getName()));
                    }
                }
                lines.emplace_back(fmt::format("RoomFlags: {}", boost::algorithm::join(flags, ", ")));
            }
        }

        if(auto s = registry.try_get<Size>(ent); s) {
            lines.emplace_back(fmt::format("Size: {} - {}", static_cast<uint8_t>(s->data), size::sizes[s->data]->getName()));
        }

        if(auto c = registry.try_get<Cost>(ent); c) {
            lines.emplace_back(fmt::format("Cost: {}", c->data));
        }

        if(auto m = registry.try_get<Money>(ent); m) {
            lines.emplace_back(fmt::format("Money: {}", m->data));
        }

        if(auto lr = registry.try_get<LevelRequirement>(ent); lr) {
            lines.emplace_back(fmt::format("LevelRequirement: {}", lr->data));
        }

        if(auto w = registry.try_get<Weight>(ent); w) {
            lines.emplace_back(fmt::format("Weight: {}", w->data));
        }

        if(auto ex = registry.try_get<Exits>(ent); ex) {
            auto d = registry.try_get<Doors>(ent);
            if(!ex->data.empty()) {
                lines.emplace_back(fmt::format("Exits: {} Directions", ex->data.size()));
                for(auto &[di, l] : ex->data) {
                    lines.emplace_back(fmt::format("  {} - {}", dir::directions[di].getName(), l.roomString()));
                }
                // TODO: Add door support
            }
        }

        if(auto li = registry.try_get<LightData>(ent); li) {
            lines.emplace_back("LightData:");
            lines.emplace_back(fmt::format("  timeLeft: {}", li->timeLeft));
            lines.emplace_back(fmt::format("  hoursLeft: {}", li->hoursLeft));
        }

        if(auto wp = registry.try_get<WeaponData>(ent); wp) {
            lines.emplace_back("WeaponData:");
            lines.emplace_back(fmt::format("  skill: {}", wp->skill));
            lines.emplace_back(fmt::format("  damDice: {}", wp->damDice));
            lines.emplace_back(fmt::format("  damSize: {}", wp->damSize));
            lines.emplace_back(fmt::format("  damType: {}", wp->damType));
        }

        if(auto wo = registry.try_get<CharWornData>(ent); wo) {
            lines.emplace_back("CharWornData:");
            // TODO: Complete this.
        }

        if(auto fd = registry.try_get<FoodData>(ent); fd) {
            lines.emplace_back("FoodData:");
            lines.emplace_back(fmt::format("  nutrition: {}", fd->nutrition));
            lines.emplace_back(fmt::format("  maxNutrition: {}", fd->maxNutrition));
            lines.emplace_back(fmt::format("  poisoned: {}", fd->poisoned));
        }

        if(auto co = registry.try_get<ContainerData>(ent); co) {
            lines.emplace_back("ContainerData:");
            lines.emplace_back(fmt::format("  capacity: {}", co->capacity));
            if(co->key) {
                lines.emplace_back(fmt::format("  key: {}", co->key.value()));
            } else {
                lines.emplace_back("  key: None");
            }
            // TODO: container flags
        }

        if(auto dd = registry.try_get<DrinkData>(ent); dd) {
            lines.emplace_back("DrinkData:");
            lines.emplace_back(fmt::format("  capacity: {}", dd->capacity));
            lines.emplace_back(fmt::format("  current: {}", dd->current));
            lines.emplace_back(fmt::format("  poisoned: {}", dd->poisoned));
            lines.emplace_back(fmt::format("  liquid: {}", static_cast<uint8_t>(dd->liquid)));
            // TODO: liquids data
        }

        if(auto d = registry.try_get<Durability>(ent); d) {
            lines.emplace_back(fmt::format("Durability: {}/{}", d->durability, d->maxDurability));
        }

        if(auto i = registry.try_get<ItemFlags>(ent); i) {
            if(i->data.any()) {
                std::vector<std::string> flags;
                for(auto &f : iflags::itemFlags) {
                    if(i->data.test(f->getId())) {
                        flags.emplace_back(fmt::format("{} - {}", f->getId(), f->getName()));
                    }
                }
                lines.emplace_back(fmt::format("ItemFlags: {}", boost::algorithm::join(flags, ", ")));
            }
        }

        if(auto ia = registry.try_get<ItemAffects>(ent); ia) {
            if(ia->data.any()) {
                std::vector<std::string> affects;
                for(auto &a : affect::affectFlags) {
                    if(ia->data.test(a->getId())) {
                        affects.emplace_back(fmt::format("{} - {}", a->getId(), a->getName()));
                    }
                }
                lines.emplace_back(fmt::format("ItemAffects: {}", boost::algorithm::join(affects, ", ")));
            }
        }

        if(auto ca = registry.try_get<CharacterAffects>(ent); ca) {
            if(ca->data.any()) {
                std::vector<std::string> affects;
                for(auto &a : affect::affectFlags) {
                    if(ca->data.test(a->getId())) {
                        affects.emplace_back(fmt::format("{} - {}", a->getId(), a->getName()));
                    }
                }
                lines.emplace_back(fmt::format("CharacterAffects: {}", boost::algorithm::join(affects, ", ")));
            }
        }

        if(auto cs = registry.try_get<CharacterStats>(ent); cs) {
            lines.emplace_back("CharacterStats:");
            for(auto i = 0; i < cstat::numCharStats; i++) {
                auto stat = cstat::characterStats.find(static_cast<cstat::StatId>(i));
                if(stat == cstat::characterStats.end()) {
                    continue;
                }
                lines.emplace_back(fmt::format("  {}: {}", stat->second->getName(), cs->data[i]));
            }
        }

        if(auto nf = registry.try_get<NPCFlags>(ent); nf) {
            if(nf->data.any()) {
                std::vector<std::string> flags;
                for(auto &f : nflags::npcFlags) {
                    if(nf->data.test(f->getId())) {
                        flags.emplace_back(fmt::format("{} - {}", f->getId(), f->getName()));
                    }
                }
                lines.emplace_back(fmt::format("NPCFlags: {}", boost::algorithm::join(flags, ", ")));
            }
        }

        if(auto pf = registry.try_get<PlayerFlags>(ent); pf) {
            if(pf->data.any()) {
                std::vector<std::string> flags;
                for(auto &f : pflags::playerFlags) {
                    if(pf->data.test(f->getId())) {
                        flags.emplace_back(fmt::format("{} - {}", f->getId(), f->getName()));
                    }
                }
                lines.emplace_back(fmt::format("PlayerFlags: {}", boost::algorithm::join(flags, ", ")));
            }
        }

        if(auto pref = registry.try_get<PreferenceFlags>(ent); pref) {
            if(pref->data.any()) {
                std::vector<std::string> flags;
                for(auto &f : pref::preferenceFlags) {
                    if(pref->data.test(f->getId())) {
                        flags.emplace_back(fmt::format("{} - {}", f->getId(), f->getName()));
                    }
                }
                lines.emplace_back(fmt::format("PreferenceFlags: {}", boost::algorithm::join(flags, ", ")));
            }
        }

        if(auto g = registry.try_get<Gravity>(ent); g) {
            lines.emplace_back(fmt::format("Gravity: {}", g->data));
        }

        if(auto cel = registry.try_get<CelestialBody>(ent); cel) {
            lines.emplace_back("CelestialBody:");
            lines.emplace_back(fmt::format("  type: {}", celtype::celTypes[cel->type]->getName()));
            if(cel->flags.any()) {
                std::vector<std::string> flags;
                for(auto &f : celflags::celestialFlags) {
                    if(cel->flags.test(f->getId())) {
                        flags.emplace_back(fmt::format("{} - {}", f->getId(), f->getName()));
                    }
                }
                lines.emplace_back(fmt::format("  flags: {}", boost::algorithm::join(flags, ", ")));
            }
        }

        if(registry.any_of<Structure>(ent)) {
            lines.emplace_back("Structure: true");
        }

        if(auto ex = registry.try_get<ExtraDescriptions>(ent); ex) {
            if(!ex->data.empty()) {
                lines.emplace_back("ExtraDescriptions:");
                for(auto &e : ex->data) {
                    lines.emplace_back(fmt::format("  {}: {}", e.first, e.second));
                }
            }
        }

        if(auto im = registry.try_get<ItemModifiers>(ent); im) {
            if(!im->data.empty()) {
                lines.emplace_back("ItemModifiers:");
                for(auto &m : im->data) {
                    lines.emplace_back(fmt::format("  {}: {} - {}", m.location, m.specific, m.modifier));
                }
            }
        }

        if(auto sk = registry.try_get<Skills>(ent); sk) {
            // TODO: Skills...
        }

        if(registry.any_of<Dimension>(ent)) {
            lines.emplace_back("Dimension: true");
        }

        if(auto iv = registry.try_get<ItemVnum>(ent); iv) {
            lines.emplace_back(fmt::format("ItemVnum: {}", iv->data));
        }

        if(auto iv = registry.try_get<NPCVnum>(ent); iv) {
            lines.emplace_back(fmt::format("NPCVnum: {}", iv->data));
        }

        if(auto lr = registry.try_get<LegacyLoadRoom>(ent); lr) {
            lines.emplace_back(fmt::format("LegacyLoadRoom: {}", lr->id));
        }

        if(auto dg = registry.try_get<DgScripts>(ent); dg) {
            lines.emplace_back(fmt::format("DgScripts: {} Scripts", dg->scripts.size()));
            for(auto &sc : dg->scripts) {
                lines.emplace_back(fmt::format("  {} - {}", sc->prototype->id, sc->prototype->name));
            }
            if(!dg->vars.empty()) {
                lines.emplace_back("    DgVariables:");
                for(auto &[name, val] : dg->vars) {
                    std::string printVal = "Unknown...";
                    if(val.index() == 0) {
                        printVal = std::get<std::string>(val);
                    } else {
                        // it's an entt::entity ...
                        auto e = std::get<Location>(val);
                        // Which is either an ObjectId or a Room...
                        printVal = "entity";
                    }
                    lines.emplace_back(fmt::format("      {}: {}", name, printVal));
                }
            }
        }

        return boost::algorithm::join(lines, "\n");
    }

    std::string renderPrefix(entt::entity ent) {
        std::string prefix = "?";
        if(registry.any_of<NPC>(ent)) {
            prefix = "N";
        }
        else if(registry.any_of<Player>(ent)) {
            prefix = "P";
        }
        else if(registry.any_of<Vehicle>(ent)) {
            prefix = "V";
        }
        else if(registry.any_of<Item>(ent)) {
            prefix = "I";
        }
        else if(registry.any_of<Area>(ent)) {
            prefix = "A";
        }
        else if(registry.any_of<CelestialBody>(ent)) {
            prefix = "C";
        }
        else if(registry.any_of<Map>(ent)) {
            prefix = "M";
        }
        else if(registry.any_of<Expanse>(ent)) {
            prefix = "E";
        }
        else if(registry.any_of<Space>(ent)) {
            prefix = "S";
        }
        return prefix;
    }

    std::string renderWhere(const std::string& name, entt::entity looker) {
        std::vector<std::string> lines;
        lines.emplace_back(fmt::format("Looking for: *{}*", name));
        std::size_t counter = 0;
        bool matrix = false;
        if(auto a = registry.try_get<AdminFlags>(looker); a) {
            matrix = a->data.test(aflags::MATRIX);
        }
        auto v = registry.view<ObjectId, Location>();
        for(auto ent : v) {
            if(ent == looker) continue;
            auto n = stripAnsi(getName(ent));
            std::string keywords;
            if(auto k = registry.try_get<Keywords>(ent); k) {
                keywords = k->data;
            }
            if(!boost::algorithm::icontains(n, name) && !boost::algorithm::icontains(keywords, name)) {
                continue;
            }
            auto prefix = renderPrefix(ent);
            auto &loc = v.get<Location>(ent);
            auto &oid = v.get<ObjectId>(ent);
            n = renderDisplayName(ent, looker, matrix);
            if(loc.locationType == LocationType::Absolute) {
                // This shouldn't be possible...
                logger->error("Found an entity with an absolute location in the world! {}", oid.toString());
                continue;
            }
            else if(loc.locationType == LocationType::Inventory) {
                lines.emplace_back(fmt::format("{} {} - inventory of {}", prefix, n, renderDisplayName(loc.data, looker, matrix)));
            }
            else if(loc.locationType == LocationType::Equipment) {
                lines.emplace_back(fmt::format("{} {} - equipped in slot {} by {}", prefix, n, loc.x, renderDisplayName(loc.data, looker, matrix)));
            }
            else if(loc.locationType == LocationType::Area) {
                auto room = loc.getRoom();
                lines.emplace_back(fmt::format("{} {} - area {} - room {}", prefix, n, renderDisplayName(loc.data, looker, matrix), loc.x, renderDisplayName(room, looker, matrix)));
            }
            else if(loc.locationType == LocationType::Map) {
                lines.emplace_back(fmt::format("{} {} - map {} - ({},{},{})", prefix, n, renderDisplayName(loc.data, looker, matrix), loc.x, loc.y, loc.z));
            }
            else if(loc.locationType == LocationType::Expanse) {
                lines.emplace_back(fmt::format("{} {} - expanse {} - ({},{},{})", prefix, n, renderDisplayName(loc.data, looker, matrix), loc.x, loc.y, loc.z));
            }
            else if(loc.locationType == LocationType::Space) {
                lines.emplace_back(fmt::format("{} {} - space {} - ({},{},{})", prefix, n, renderDisplayName(loc.data, looker, matrix), loc.x, loc.y, loc.z));
            }
            else {
                logger->error("Found an entity with an unknown location type in the world! {} - {}", oid.toString(), static_cast<uint8_t>(loc.locationType));
                continue;
            }
            counter++;
        }

        if(counter) {
            lines.emplace_back(fmt::format("Found {} matches.", counter));
        }

        return boost::algorithm::join(lines, "\n");
    }

}