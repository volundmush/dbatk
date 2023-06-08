#pragma once
#include "kaizermud/base.h"

namespace dbat::components {
    struct Room {
        std::uint8_t terrain{0};
    };

    struct Character {
        std::uint8_t race{0};
        std::uint8_t sensei{0};
        std::uint8_t position{8};
        std::uint8_t sex{0};
    };
}