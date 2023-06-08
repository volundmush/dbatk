#include "dbatk/Terrain.h"

namespace dbat::terrain {

    std::vector<std::shared_ptr<Terrain>> terrainRegistry;

    void registerTerrainType(const std::shared_ptr<Terrain>& terrainType) {
        while(terrainRegistry.size() < terrainType->getID() + 1) {
            terrainRegistry.emplace_back(nullptr);
        }
        terrainRegistry[terrainType->getID()] = terrainType;
    }
    
    void registerTerrainTypes() {
        registerTerrainType(std::make_shared<Space>());
        registerTerrainType(std::make_shared<City>());
        registerTerrainType(std::make_shared<Inside>());
        registerTerrainType(std::make_shared<Plain>());
        registerTerrainType(std::make_shared<Forest>());
        registerTerrainType(std::make_shared<Mountains>());
        registerTerrainType(std::make_shared<Hills>());
        registerTerrainType(std::make_shared<Sky>());
        registerTerrainType(std::make_shared<Water>());
        registerTerrainType(std::make_shared<UnderWater>());
        registerTerrainType(std::make_shared<Shop>());
        registerTerrainType(std::make_shared<Important>());
        registerTerrainType(std::make_shared<Desert>());
        registerTerrainType(std::make_shared<Shallows>());
        registerTerrainType(std::make_shared<Lava>());
    }
}