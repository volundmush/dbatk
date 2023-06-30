#pragma once
#include "dbatk/base.h"

namespace dbat::position {

    enum PositionId : uint8_t {
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
        [[nodiscard]] virtual std::size_t getId() = 0;
        [[nodiscard]] virtual std::string getName() = 0;
    };

    struct Dead : Position {
        [[nodiscard]] std::size_t getId() override {return DEAD;};
        [[nodiscard]] std::string getName() override {return "Dead";}
    };

    struct MortallyWounded : Position {
        [[nodiscard]] std::size_t getId() override {return MORTALLY_WOUNDED;};
        [[nodiscard]] std::string getName() override {return "Mortally Wounded";}
    };

    struct Incapacitated : Position {
        [[nodiscard]] std::size_t getId() override {return INCAPACITATED;};
        [[nodiscard]] std::string getName() override {return "Incapacitated";}
    };

    struct Stunned : Position {
        [[nodiscard]] std::size_t getId() override {return STUNNED;};
        [[nodiscard]] std::string getName() override {return "Stunned";}
    };

    struct Sleeping : Position {
        [[nodiscard]] std::size_t getId() override {return SLEEPING;};
        [[nodiscard]] std::string getName() override {return "Sleeping";}
    };

    struct Resting : Position {
        [[nodiscard]] std::size_t getId() override {return RESTING;};
        [[nodiscard]] std::string getName() override {return "Resting";}
    };

    struct Sitting : Position {
        [[nodiscard]] std::size_t getId() override {return SITTING;};
        [[nodiscard]] std::string getName() override {return "Sitting";}
    };

    struct Fighting : Position {
        [[nodiscard]] std::size_t getId() override {return FIGHTING;};
        [[nodiscard]] std::string getName() override {return "Fighting";}
    };

    struct Standing : Position {
        [[nodiscard]] std::size_t getId() override {return STANDING;};
        [[nodiscard]] std::string getName() override {return "Standing";}
    };

    extern const std::vector<std::shared_ptr<Position>> positions;

    std::optional<PositionId> getPositionId(const std::string& name, bool exact = true);

}