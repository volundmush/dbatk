#pragma once
#include "dbatk/base.h"

namespace dbat::liq {
    enum LiquidId : uint8_t {
        WATER = 0,
        BEER = 1,
        WINE = 2,
        ALE = 3,
        DARKALE = 4,
        WHISKY = 5,
        LEMONADE = 6,
        FIREBREATH = 7,
        LOCALSPECIALTY = 8,
        SLIME = 9,
        MILK = 10,
        TEA = 11,
        COFFEE = 12,
        BLOOD = 13,
        SALTWATER = 14,
        CLEARWATER = 15
    };

    constexpr std::size_t numLiquids = 16;
}