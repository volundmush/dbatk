#pragma once
#include "dbatk/base.h"

namespace dbat::dir {
    enum DirectionId : uint8_t {
        North = 0,
        East = 1,
        South = 2,
        West = 3,
        Up = 4,
        Down = 5,
        Northwest = 6,
        Northeast = 7,
        Southeast = 8,
        Southwest = 9,
        Inside = 10,
        Outside = 11
    };

    constexpr std::size_t numMainDirs = 10;
    constexpr std::size_t numAllDirs = 12;

    class Direction {
    public:
        Direction(DirectionId id, std::string name, std::string abbr, DirectionId revID, std::string revName, std::string revAbbr, GridPoint coordinates) :
            id(id), name(std::move(name)), abbr(std::move(abbr)), revID(revID), revName(std::move(revName)), revAbbr(std::move(revAbbr)), coordinates(coordinates) {}
        [[nodiscard]] DirectionId getId() const {return id; }
        [[nodiscard]] std::string getName() const {return name;};
        [[nodiscard]] std::string getAbbr() const {return abbr;};
        [[nodiscard]] std::string getRevAbbr() const {return revAbbr;};
        [[nodiscard]] std::string getRevName() const {return revName;};
        [[nodiscard]] DirectionId getRevID() const {return revID;};
        [[nodiscard]] GridPoint offset() const { return coordinates;};
    protected:
        DirectionId id, revID;
        std::string name, revName, abbr, revAbbr;
        GridPoint coordinates;
    };

    extern const std::vector<Direction> directions;
}