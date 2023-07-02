#include "dbatk/aspects/celFlags.h"

namespace dbat::celflags {

    const std::vector<std::shared_ptr<CelestialFlag>> celestialFlags = {
            std::make_shared<SimpleCelestialFlag>(ETHERSTREAM, "ETHERSTREAM"),
            std::make_shared<SimpleCelestialFlag>(HASMOON, "HASMOON"),
    };
}