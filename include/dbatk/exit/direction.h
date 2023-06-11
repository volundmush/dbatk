#pragma once
#include "dbatk/base.h"

namespace dbat::dir {
    enum DirNum : std::size_t {
        NORTH = 0,
        EAST = 1,
        SOUTH = 2,
        WEST = 3,
        UP = 4,
        DOWN = 5,
        NORTHWEST = 6,
        NORTHEAST = 7,
        SOUTHEAST = 8,
        SOUTHWEST = 9,
        INSIDE = 10,
        OUTSIDE = 11
    };

    class Direction {
    public:
        Direction(DirNum id, std::string name, std::string abbr, DirNum revid, std::string revName, std::string revAbbr) :
                id(id), name(std::move(name)), abbr(std::move(abbr)), revID(revid), revName(std::move(revName)), revAbbr(std::move(revAbbr)) {};
        [[nodiscard]] DirNum getID() const {return id; }
        [[nodiscard]] std::string getName() const {return name;};
        [[nodiscard]] std::string getAbbr() const {return abbr;};
        [[nodiscard]] std::string getRevAbbr() const {return revAbbr;};
        [[nodiscard]] std::string getRevName() const {return revName;};
        [[nodiscard]] DirNum getRevID() const {return revID;};
    protected:
        DirNum id, revID;
        std::string name, revName, abbr, revAbbr;
    };

    extern const std::vector<Direction> directions;
}