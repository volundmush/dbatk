#pragma once
#include "dbatk/base.h"

namespace dbat::position {

    enum PositionID : std::size_t {
        DEAD = 0,
        MORTALLY_WOUNDED = 1,
        INCAPACITATED = 2,
        STUNNED = 3,
        SLEEPING = 4,
        RESTING = 5,
        SITTING = 6,
        FIGHTING = 7,
        STANDING = 8
    };

    struct Position {
        [[nodiscard]] virtual std::size_t getID() = 0;
        [[nodiscard]] virtual std::string_view getName() = 0;
    };

    struct Dead : Position {
        [[nodiscard]] std::size_t getID() override {return DEAD;};
        [[nodiscard]] std::string_view getName() override {return "Dead";}
    };

    struct MortallyWounded : Position {
        [[nodiscard]] std::size_t getID() override {return MORTALLY_WOUNDED;};
        [[nodiscard]] std::string_view getName() override {return "Mortally Wounded";}
    };

    struct Incapacitated : Position {
        [[nodiscard]] std::size_t getID() override {return INCAPACITATED;};
        [[nodiscard]] std::string_view getName() override {return "Incapacitated";}
    };

    struct Stunned : Position {
        [[nodiscard]] std::size_t getID() override {return STUNNED;};
        [[nodiscard]] std::string_view getName() override {return "Stunned";}
    };

    struct Sleeping : Position {
        [[nodiscard]] std::size_t getID() override {return SLEEPING;};
        [[nodiscard]] std::string_view getName() override {return "Sleeping";}
    };

    struct Resting : Position {
        [[nodiscard]] std::size_t getID() override {return RESTING;};
        [[nodiscard]] std::string_view getName() override {return "Resting";}
    };

    struct Sitting : Position {
        [[nodiscard]] std::size_t getID() override {return SITTING;};
        [[nodiscard]] std::string_view getName() override {return "Sitting";}
    };

    struct Fighting : Position {
        [[nodiscard]] std::size_t getID() override {return FIGHTING;};
        [[nodiscard]] std::string_view getName() override {return "Fighting";}
    };

    struct Standing : Position {
        [[nodiscard]] std::size_t getID() override {return STANDING;};
        [[nodiscard]] std::string_view getName() override {return "Standing";}
    };

    extern const std::vector<std::shared_ptr<Position>> positions;

}