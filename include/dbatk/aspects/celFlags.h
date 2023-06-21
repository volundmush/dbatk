#include "dbatk/aspects/flags.h"

namespace dbat::celflags {

    enum CelestialFlagId : uint8_t {
        ETHERSTREAM = 0,
        HASMOON = 1,
    };


    constexpr std::size_t numCelestialFlags = 2;


}