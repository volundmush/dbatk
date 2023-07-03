#pragma once
#include "dbatk/aspects/flags.h"

namespace dbat::con {
    enum ContainerFlagId : uint8_t {
        CLOSEABLE = 0,
        PICKPROOF = 1,
        CLOSED = 2,
        LOCKED = 3
    };

    constexpr std::size_t numContFlags = 4;
}