#include "dbatk/aspects/terrain.h"

namespace dbat::terrain {

    std::vector<std::shared_ptr<Terrain>> terrainRegistry = {
            std::make_shared<Inside>(),
            std::make_shared<City>(),
            std::make_shared<Plain>(),
            std::make_shared<Forest>(),
            std::make_shared<Hills>(),
            std::make_shared<Mountains>(),
            std::make_shared<Shallows>(),
            std::make_shared<Water>(),
            std::make_shared<Sky>(),
            std::make_shared<UnderWater>(),
            std::make_shared<Shop>(),
            std::make_shared<Important>(),
            std::make_shared<Desert>(),
            std::make_shared<Space>(),
            std::make_shared<Lava>()
    };
}