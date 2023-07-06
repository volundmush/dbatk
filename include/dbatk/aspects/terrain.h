#pragma once
#include "dbatk/base.h"

namespace dbat::terrain {

    enum TerrainId : uint8_t {
        INSIDE = 0,
        CITY = 1,
        PLAIN = 2,
        FOREST = 3,
        HILLS = 4,
        MOUNTAINS = 5,
        SHALLOWS = 6,
        WATER = 7,
        SKY = 8,
        UNDERWATER = 9,
        SHOP = 10,
        IMPORTANT = 11,
        DESERT = 12,
        SPACE = 13,
        LAVA = 14
    };

    constexpr std::size_t NUM_TERRAIN_TYPES = 15;

    struct Terrain {
        [[nodiscard]] virtual std::size_t getId() = 0;
        [[nodiscard]] virtual std::string getName() = 0;
        [[nodiscard]] virtual std::string getMapTile() = 0;
        [[nodiscard]] virtual std::size_t getSortPriority() {return getId();};
        [[nodiscard]] virtual bool displayOnLegend() {return true;};
    };

    extern std::vector<std::shared_ptr<Terrain>> terrainRegistry;

}