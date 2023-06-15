#pragma once
#include "dbatk/base.h"

namespace dbat::wear {
    enum WearID : std::size_t {
        FINGER = 0,  // Can be worn on finger
        NECK = 1,    // Can be worn around neck
        BODY = 2,    // Can be worn on body
        HEAD = 3,    // Can be worn on head
        LEGS = 4,    // Can be worn on legs
        FEET = 5,    // Can be worn on feet
        HANDS = 6,   // Can be worn on hands
        ARMS = 7,    // Can be worn on arms
        SHIELD = 8,  // Can be used as a shield
        ABOUT = 9,  // Can be worn about body
        WAIST = 10,  // Can be worn around waist
        WRIST = 11,  // Can be worn on wrist
        WIELD = 12,  // Can be wielded
        HOLD = 13,   // Can be held
        PACK = 14,   // Can be worn as a backpack
        EAR = 15,    // Can be worn as an earring
        SHOULDER = 16,     // Can be worn as wings
        EYE = 17     // Can be worn as a mask
    };

    constexpr std::size_t NUM_WEAR_FLAGS = 18;

}