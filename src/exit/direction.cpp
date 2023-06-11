#include "dbatk/exit/direction.h"

namespace dbat::dir {

    const std::vector<Direction> directions = {
            {NORTH, "north", "n", SOUTH, "south", "s"},
            {EAST, "east", "e", WEST, "west", "w"},
            {SOUTH, "south", "s", NORTH, "north", "n"},
            {WEST, "west", "w", EAST, "east", "e"},
            {UP, "up", "u", DOWN, "down", "d"},
            {DOWN, "down", "d", UP, "up", "u"},
            {NORTHWEST, "northwest", "nw", SOUTHEAST, "southeast", "se"},
            {NORTHEAST, "northeast", "ne", SOUTHWEST, "southwest", "sw"},
            {SOUTHEAST, "southeast", "se", NORTHWEST, "northwest", "nw"},
            {SOUTHWEST, "southwest", "sw", NORTHEAST, "northeast", "ne"},
            {INSIDE, "inside", "in", OUTSIDE, "outside", "o"},
            {OUTSIDE, "outside", "o", INSIDE, "inside", "in"},
    };

}