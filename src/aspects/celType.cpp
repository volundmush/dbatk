#include "dbatk/aspects/celType.h"

namespace dbat::celtype {
    const std::vector<std::shared_ptr<CelType>> celTypes = {
            std::make_shared<SimpleCelType>(PLANET, "PLANET"),
            std::make_shared<SimpleCelType>(MOON, "MOON"),
            std::make_shared<SimpleCelType>(ASTEROID, "ASTEROID"),
            std::make_shared<SimpleCelType>(STAR, "STAR"),
            std::make_shared<SimpleCelType>(STATION, "STATION")
    };

    std::optional<CelestialTypeId> getCelTypeId(const std::string& name, bool exact) {
        auto found = partialMatch(name, celTypes.begin(), celTypes.end(), exact, [](const auto& flag) { return flag->getName(); });
        if(found != celTypes.end()) {
            return static_cast<CelestialTypeId>((*found)->getId());
        }
        return std::nullopt;
    }
}