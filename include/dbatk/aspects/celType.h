#pragma once

#include "dbatk/base.h"

namespace dbat::celtype {
    enum CelestialTypeId : uint8_t {
        PLANET = 0,
        MOON = 1,
        ASTEROID = 2,
        STAR = 3,
        STATION = 4
    };
}