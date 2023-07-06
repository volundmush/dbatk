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

    extern const std::vector<std::shared_ptr<Position>> positions;

    std::optional<PositionId> getPositionId(const std::string& name, bool exact = true);

}