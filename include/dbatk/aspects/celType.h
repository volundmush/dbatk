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

    constexpr std::size_t numCelestialTypes = 5;

    class CelType : public BaseFlag {
    public:
        virtual void onLoad(entt::entity room) {};
        virtual void onSet(entt::entity room) {};
        virtual void onClear(entt::entity room) {};
    };

    std::optional<CelestialTypeId> getCelTypeId(const std::string& name, bool exact);
    extern const std::vector<std::shared_ptr<CelType>> celTypes;
}