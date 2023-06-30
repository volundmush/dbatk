#include "dbatk/operations/information.h"
#include "dbatk/components.h"
#include "dbatk/api.h"
#include "fmt/args.h"
#include "dbatk/color.h"

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

    std::string renderRoomAppearance(entt::entity room, entt::entity looker) {
        std::vector<std::string> lines;
        lines.emplace_back(roomHeader);
        lines.emplace_back(fmt::format("Location: {}", getDisplayName(room, looker)));
        lines.emplace_back("Placeholder");
        lines.emplace_back(roomHeader);
        auto ldesc = getLookDescription(room);
        if(!ldesc.empty()) {
            lines.emplace_back(ldesc);
            lines.emplace_back(roomSubheader);
        }
        lines.emplace_back("       Compass        AutoMap                  Map Key");
        lines.emplace_back("@R -----------------   ---------    -------------------------------------");
        auto automap = generateAutomap(room, looker);
        auto compass = generateCompass(room, looker);
        auto legend = generateMapLegend(room, looker);
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
            lines.emplace_back(" " + compassLine + sep1 + automapLine + sep2 + legendLine);
        }
        lines.emplace_back(roomSubheader);
        auto rcon = registry.try_get<RoomContents>(room);
        if(rcon) {
            for(auto e : rcon->data) {
                if(e == looker) continue;
                lines.emplace_back(getRoomLine(e, looker));
            }
        }

        return boost::algorithm::join(lines, "@n\n");
    }
}