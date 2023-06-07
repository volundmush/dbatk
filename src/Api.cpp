#include "kaizermud/Api.h"
#include "boost/algorithm/string.hpp"
#include "fmt/format.h"
#include "fmt/args.h"
#include "dbatk/Terrain.h"
#include "kaizermud/Color.h"

namespace dbat {

    static const std::string compassTemplate = "      |@C {N} @n|\n"
                                               "| @C{NW: >2}@n| |@Y{U: ^3}@n| |@C{NE: <3}@n|\n"
                                               "| @C{W: >2}@n| |@m{I: ^3}@n| |@C{E: <3}@n|\n"
                                               "| @C{SW: >2}@n| |@Y{D:^3}@n| |@C{SE: <3}@n|\n"
                                               "      |@C {S} @n|";

    static const std::string roomHeader = "O----------------------------------------------------------------------O";
    static const std::string roomSubheader = "------------------------------------------------------------------------";

    static std::unordered_map<std::string, std::string> exitMap = {
            {"north",     "N"},
            {"south",     "S"},
            {"east",      "E"},
            {"west",      "W"},
            {"up",        "U"},
            {"down",      "D"},
            {"northeast", "NE"},
            {"northwest", "NW"},
            {"southeast", "SE"},
            {"southwest", "SW"},
            {"inside",    "I"},
            {"outside",   "I"},
    };

    static std::vector<std::string> generateCompass(entt::entity ent, entt::entity looker) {
        std::vector<entt::entity> exits;

        auto rev = kaizer::getReverseRelation(ent, "exit");
        if (rev.has_value()) exits = rev.value().get();

        fmt::dynamic_format_arg_store<fmt::format_context> args;
        std::unordered_map<std::string, std::string> exArgs;
        for(auto &[name, key] : exitMap) {
            exArgs[key] = " ";
        }

        for (auto &[name, key]: exitMap) {
            // check if exits contains a matching name, and if so, we'll create an arg for it using the key.
            // If it doesn't contain a match, then we set the arg to an empty space.
            auto it = std::find_if(exits.begin(), exits.end(), [&](auto &e) {
                return boost::iequals(kaizer::getDisplayName(e, looker), name);
            });
            if (it != exits.end()) {
                if(name == "outside") {
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

            terrain::Terrain *ter = dynamic_cast<terrain::Terrain *>(kaizer::getAspect(room, "terrain"));
            if(ter) {
                curMap[curY][curX] = ter->getMapTile();
            } else {
                curMap[curY][curX] = "?";
            }

            auto rev = kaizer::getReverseRelation(room, "exit");
            std::vector<entt::entity> exits;
            if(rev.has_value()) exits = rev.value().get();

            // Now we must iterate over all exits and scan those which correlate with cardinal directions: N, S, E, W and diagonals.
            for(auto &exit : exits) {
                auto name = kaizer::getDisplayName(exit, looker);
                auto dest = kaizer::getRelation(exit, "destination");
                if(!kaizer::registry.valid(dest)) continue;

                if(boost::iequals(name, "north")) {
                    if((curY + 1) <= maxY) scan(dest, curX, curY + 1);
                }
                else if (boost::iequals(name, "south")) {
                    if((curY - 1) >= minY) scan(dest, curX, curY - 1);
                }
                else if(boost::iequals(name, "east")) {
                    if((curX + 1) <= maxX) scan(dest, curX + 1, curY);
                }
                else if(boost::iequals(name, "west")) {
                    if((curX - 1) >= minX) scan(dest, curX - 1, curY);
                }
                else if(boost::iequals(name, "northeast")) {
                    if((curX + 1) <= maxX && (curY + 1) <= maxY) scan(dest, curX + 1, curY + 1);
                }
                else if(boost::iequals(name, "northwest")) {
                    if((curX - 1) >= minX && (curY + 1) <= maxY) scan(dest, curX - 1, curY + 1);
                }
                else if(boost::iequals(name, "southeast")) {
                    if((curX + 1) <= maxX && (curY - 1) >= minY) scan(dest, curX + 1, curY - 1);
                }
                else if(boost::iequals(name, "southwest")) {
                    if((curX - 1) >= minX && (curY - 1) >= minY) scan(dest, curX - 1, curY - 1);
                }
                // No other exit types are respected... yet...
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

    static std::vector<terrain::Terrain*> terrains;

    static std::vector<std::string> mapLegend;

    std::vector<std::string> generateMapLegend(entt::entity ent, entt::entity looker) {
        if(terrains.empty()) {
            // Populate terrains.
            for(auto &[slot, asp] : kaizer::aspectRegistry["terrain"]) {
                auto ter = dynamic_cast<terrain::Terrain *>(asp.get());
                if(ter) terrains.push_back(ter);
            }
            // Now sort terrains by their getSortPriority() value.
            std::sort(terrains.begin(), terrains.end(), [](terrain::Terrain *a, terrain::Terrain *b) {
                return a->getSortPriority() < b->getSortPriority();
            });
        }
        if(mapLegend.empty()) {
            std::vector<std::string> legend;
            for(auto t : terrains) {
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

    std::string roomRenderAppearance(entt::entity ent, entt::entity looker) {
        std::vector<std::string> lines;
        lines.emplace_back(roomHeader);
        lines.emplace_back(fmt::format("Location: {}", kaizer::getDisplayName(ent, looker)));
        lines.emplace_back("Placeholder");
        lines.emplace_back(roomHeader);
        auto desc = kaizer::getString(ent, "look_description");
        if (desc.has_value()) {
            lines.emplace_back(desc.value());
            lines.emplace_back(roomSubheader);
        }
        lines.emplace_back("       Compass        AutoMap                  Map Key");
        lines.emplace_back("@R -----------------   ---------    -------------------------------------");
        auto automap = generateAutomap(ent, looker);
        auto compass = generateCompass(ent, looker);
        auto legend = generateMapLegend(ent, looker);
        // Now we need to know the maximum amount of lines for each of these three things.
        auto maxLines = std::max({automap.size(), compass.size(), legend.size()});
        for(int i = 0; i < maxLines; i++) {
            std::string compassLine = i < compass.size() ? compass[i] : "";
            std::string automapLine = i < automap.size() ? automap[i] : "";
            std::string legendLine = i < legend.size() ? legend[i] : "";
            // now we need to define sep1. sep1 is a sequence of 20-(length of compassLine minus colorcode)...
            auto sep1 = std::string(20 - kaizer::stripAnsi(compassLine).size(), ' ');
            // sep2 is a sequence of 14-(length of automapLine minus colorcode)...
            auto sep2 = std::string(14 - kaizer::stripAnsi(automapLine).size(), ' ');
            lines.emplace_back(" " + compassLine + sep1 + automapLine + sep2 + legendLine);
        }

        return boost::algorithm::join(lines, "@n\n");
    }

    std::string roomGetDisplayName(entt::entity ent, entt::entity looker) {
        auto name = kaizer::getString(ent, "short_description");
        if(name.has_value()) return std::string(name.value());
        return fmt::format("Unnamed Room {}", kaizer::getID(ent));
    }

    void registerDBAPI() {
        kaizer::renderAppearance.setOverride("room", roomRenderAppearance);
        kaizer::getDisplayName.setOverride("room", roomGetDisplayName);
    }

}