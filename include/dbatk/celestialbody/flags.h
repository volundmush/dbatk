#include "dbatk/flags.h"

namespace dbat::celflags {

    enum CelestialFlagID : std::size_t {
        ETHERSTREAM = 0,
        HASMOON = 1,
    };


    constexpr std::size_t NUM_CELESTIAL_FLAGS = 2;


}