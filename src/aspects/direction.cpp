#include "dbatk/aspects/direction.h"

namespace dbat::dir {

    const std::unordered_map<std::string, std::string> exitAliases = {
            {"n", "north"},
            {"s", "south"},
            {"e", "east"},
            {"w", "west"},
            {"u", "up"},
            {"d", "down"},
            {"nw", "northwest"},
            {"ne", "northeast"},
            {"sw", "southwest"},
            {"se", "southeast"},
            {"in", "inside"},
            {"out", "outside"}
    };

    const std::vector<Direction> directions = {
            {North,     "north",     "n",  South,     "south",     "s",  GridPoint(0, 1, 0)},
            {East,      "east",      "e",  West,      "west",      "w",  GridPoint(1, 0, 0)},
            {South,     "south",     "s",  North,     "north",     "n",  GridPoint(0, -1, 0)},
            {West,      "west",      "w",  East,      "east",      "e",  GridPoint(-1, 0, 0)},
            {Up,        "up",        "u",  Down,      "down",      "d",  GridPoint(0, 0, 1)},
            {Down,      "down",      "d",  Up,        "up",        "u",  GridPoint(0, 0, -1)},
            {Northwest, "northwest", "nw", Southeast, "southeast", "se", GridPoint(-1, 1, 0)},
            {Northeast, "northeast", "ne", Southwest, "southwest", "sw", GridPoint(1, 1, 0)},
            {Southeast, "southeast", "se", Northwest, "northwest", "nw", GridPoint(1, -1, 0)},
            {Southwest, "southwest", "sw", Northeast, "northeast", "ne", GridPoint(-1, -1, 0)},
            {Inside,    "inside",    "in", Outside,   "outside",   "o",  GridPoint(0, 0, 0)},
            {Outside,   "outside",   "o",  Inside,    "inside",    "in", GridPoint(0, 0, 0)},
    };

    std::optional<DirectionId> parseDirection(const std::string& txt, bool enableInOut) {
        std::string check = boost::to_lower_copy(txt);
        boost::trim(check);

        auto begin = directions.begin();
        auto end = directions.end();
        if(!enableInOut) {
            end -= 2;
        }

        auto result = std::find_if(begin, end, [&check](const Direction& dir) {
            return dir.getName() == check || dir.getAbbr() == check || boost::istarts_with(dir.getName(), check);
        });
        if(result != end) {
            return result->getId();
        } else {
            return std::nullopt;
        }

    }

}