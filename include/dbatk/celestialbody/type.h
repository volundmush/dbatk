#pragma once

#include "dbatk/base.h"

namespace dbat::celtype {
    enum CelestialTypeID : std::size_t {
        PLANET = 0,
        ASTEROID = 1,
        STAR = 2
    };
}