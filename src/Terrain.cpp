#include "dbatk/Terrain.h"

namespace dbat::terrain {

    void registerTerrainTypes() {
        auto city = std::make_shared<City>();
        registerAspect(city);
        registerAspect(std::make_shared<Inside>());
        registerAspect(std::make_shared<Plain>());
        registerAspect(std::make_shared<Forest>());
        registerAspect(std::make_shared<Mountains>());
        registerAspect(std::make_shared<Hills>());
        registerAspect(std::make_shared<Sky>());
        registerAspect(std::make_shared<Water>());
        registerAspect(std::make_shared<UnderWater>());
        registerAspect(std::make_shared<Shop>());
        registerAspect(std::make_shared<Important>());
        registerAspect(std::make_shared<Desert>());
        registerAspect(std::make_shared<Shallows>());
        registerAspect(std::make_shared<Lava>());
        registerAspect(std::make_shared<Space>());

        kaizer::aspectSlotDefaults["terrain"] = city;
    }
}