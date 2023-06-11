#pragma once
#include "dbatk/base.h"

namespace dbat::terrain {

    enum TerrainID : std::size_t {
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
        [[nodiscard]] virtual std::size_t getID() = 0;
        [[nodiscard]] virtual std::string getName() = 0;
        [[nodiscard]] virtual std::string getMapTile() = 0;
        [[nodiscard]] virtual std::size_t getSortPriority() {return getID();};
        [[nodiscard]] virtual bool displayOnLegend() {return true;};
    };

    struct Inside : Terrain {
        [[nodiscard]] std::string getName() override {return "Inside";}
        [[nodiscard]] std::string getMapTile() override {return "@wI@n";}
        [[nodiscard]] std::size_t getID() override {return INSIDE;};
        [[nodiscard]] std::size_t getSortPriority() override {return 1;};
    };

    struct City : Terrain {
        [[nodiscard]] std::string getName() override {return "City";}
        [[nodiscard]] std::string getMapTile() override {return "@WC@n";}
        [[nodiscard]] std::size_t getID() override {return CITY;};
        [[nodiscard]] std::size_t getSortPriority() override {return 0;};
    };

    struct Plain : Terrain {
        [[nodiscard]] std::string getName() override {return "Plain";}
        [[nodiscard]] std::string getMapTile() override {return "@GP@n";}
        [[nodiscard]] std::size_t getID() override {return PLAIN;};
    };

    struct Forest : Terrain {
        [[nodiscard]] std::string getName() override {return "Forest";}
        [[nodiscard]] std::string getMapTile() override {return "@gF@n";}
        [[nodiscard]] std::size_t getID() override {return FOREST;};
    };

    struct Hills : Terrain {
        [[nodiscard]] std::string getName() override {return "Hills";}
        [[nodiscard]] std::string getMapTile() override {return "@yH@n";}
        [[nodiscard]] std::size_t getID() override {return HILLS;};
        [[nodiscard]] std::size_t getSortPriority() override {return 5;};
    };

    struct Mountains : Terrain {
        [[nodiscard]] std::string getName() override {return "Mountains";}
        [[nodiscard]] std::string getMapTile() override {return "@DM@n";}
        [[nodiscard]] std::size_t getID() override {return MOUNTAINS;};
        [[nodiscard]] std::size_t getSortPriority() override {return 4;};
    };

    struct Shallows : Terrain {
        [[nodiscard]] std::string getName() override {return "Shallows";}
        [[nodiscard]] std::string getMapTile() override {return "@c~@n";}
        [[nodiscard]] std::size_t getID() override {return SHALLOWS;};
        [[nodiscard]] std::size_t getSortPriority() override {return 12;};
    };

    struct Water : Terrain {
        [[nodiscard]] std::string getName() override {return "Water";}
        [[nodiscard]] std::string getMapTile() override {return "@BW@n";}
        [[nodiscard]] std::size_t getID() override {return WATER;};
    };

    struct Sky : Terrain {
        [[nodiscard]] std::string getName() override {return "Sky";}
        [[nodiscard]] std::string getMapTile() override {return "@CS@n";}
        [[nodiscard]] std::size_t getID() override {return SKY;};
    };

    struct UnderWater : Terrain {
        [[nodiscard]] std::string getName() override {return "Underwater";}
        [[nodiscard]] std::string getMapTile() override {return "@bU@n";}
        [[nodiscard]] std::size_t getID() override {return UNDERWATER;};
    };

    struct Shop : Terrain {
        [[nodiscard]] std::string getName() override {return "Shop";}
        [[nodiscard]] std::string getMapTile() override {return "@m$@n";}
        [[nodiscard]] std::size_t getID() override {return SHOP;};
    };

    struct Important : Terrain {
        [[nodiscard]] std::string getName() override {return "Important";}
        [[nodiscard]] std::string getMapTile() override {return "@M#@n";}
        [[nodiscard]] std::size_t getID() override {return IMPORTANT;};
    };

    struct Desert : Terrain {
        [[nodiscard]] std::string getName() override {return "Desert";}
        [[nodiscard]] std::string getMapTile() override {return "@YD@n";}
        [[nodiscard]] std::size_t getID() override {return DESERT;};
    };

    struct Space : Terrain {
        [[nodiscard]] std::string getName() override {return "Space";}
        [[nodiscard]] std::string getMapTile() override {return "@BS@n";}
        [[nodiscard]] std::size_t getID() override {return SPACE;};
        [[nodiscard]] bool displayOnLegend() override {return false;};
        [[nodiscard]] std::size_t getSortPriority() override {return 14;};
    };

    struct Lava : Terrain {
        [[nodiscard]] std::string getName() override {return "Lava";}
        [[nodiscard]] std::string getMapTile() override {return "@4 @n";}
        [[nodiscard]] std::size_t getID() override {return LAVA;};
        [[nodiscard]] std::size_t getSortPriority() override {return 13;};
    };

    extern std::vector<std::shared_ptr<Terrain>> terrainRegistry;

}