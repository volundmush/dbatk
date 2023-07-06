#include "dbatk/aspects/terrain.h"

namespace dbat::terrain {

    struct Inside : Terrain {
        [[nodiscard]] std::string getName() override {return "Inside";}
        [[nodiscard]] std::string getMapTile() override {return "@wI@n";}
        [[nodiscard]] std::size_t getId() override {return INSIDE;};
        [[nodiscard]] std::size_t getSortPriority() override {return 1;};
    };

    struct City : Terrain {
        [[nodiscard]] std::string getName() override {return "City";}
        [[nodiscard]] std::string getMapTile() override {return "@WC@n";}
        [[nodiscard]] std::size_t getId() override {return CITY;};
        [[nodiscard]] std::size_t getSortPriority() override {return 0;};
    };

    struct Plain : Terrain {
        [[nodiscard]] std::string getName() override {return "Plain";}
        [[nodiscard]] std::string getMapTile() override {return "@GP@n";}
        [[nodiscard]] std::size_t getId() override {return PLAIN;};
    };

    struct Forest : Terrain {
        [[nodiscard]] std::string getName() override {return "Forest";}
        [[nodiscard]] std::string getMapTile() override {return "@gF@n";}
        [[nodiscard]] std::size_t getId() override {return FOREST;};
    };

    struct Hills : Terrain {
        [[nodiscard]] std::string getName() override {return "Hills";}
        [[nodiscard]] std::string getMapTile() override {return "@yH@n";}
        [[nodiscard]] std::size_t getId() override {return HILLS;};
        [[nodiscard]] std::size_t getSortPriority() override {return 5;};
    };

    struct Mountains : Terrain {
        [[nodiscard]] std::string getName() override {return "Mountains";}
        [[nodiscard]] std::string getMapTile() override {return "@DM@n";}
        [[nodiscard]] std::size_t getId() override {return MOUNTAINS;};
        [[nodiscard]] std::size_t getSortPriority() override {return 4;};
    };

    struct Shallows : Terrain {
        [[nodiscard]] std::string getName() override {return "Shallows";}
        [[nodiscard]] std::string getMapTile() override {return "@c~@n";}
        [[nodiscard]] std::size_t getId() override {return SHALLOWS;};
        [[nodiscard]] std::size_t getSortPriority() override {return 12;};
    };

    struct Water : Terrain {
        [[nodiscard]] std::string getName() override {return "Water";}
        [[nodiscard]] std::string getMapTile() override {return "@BW@n";}
        [[nodiscard]] std::size_t getId() override {return WATER;};
    };

    struct Sky : Terrain {
        [[nodiscard]] std::string getName() override {return "Sky";}
        [[nodiscard]] std::string getMapTile() override {return "@CS@n";}
        [[nodiscard]] std::size_t getId() override {return SKY;};
    };

    struct UnderWater : Terrain {
        [[nodiscard]] std::string getName() override {return "Underwater";}
        [[nodiscard]] std::string getMapTile() override {return "@bU@n";}
        [[nodiscard]] std::size_t getId() override {return UNDERWATER;};
    };

    struct Shop : Terrain {
        [[nodiscard]] std::string getName() override {return "Shop";}
        [[nodiscard]] std::string getMapTile() override {return "@m$@n";}
        [[nodiscard]] std::size_t getId() override {return SHOP;};
    };

    struct Important : Terrain {
        [[nodiscard]] std::string getName() override {return "Important";}
        [[nodiscard]] std::string getMapTile() override {return "@M#@n";}
        [[nodiscard]] std::size_t getId() override {return IMPORTANT;};
    };

    struct Desert : Terrain {
        [[nodiscard]] std::string getName() override {return "Desert";}
        [[nodiscard]] std::string getMapTile() override {return "@YD@n";}
        [[nodiscard]] std::size_t getId() override {return DESERT;};
    };

    struct Space : Terrain {
        [[nodiscard]] std::string getName() override {return "Space";}
        [[nodiscard]] std::string getMapTile() override {return "@BS@n";}
        [[nodiscard]] std::size_t getId() override {return SPACE;};
        [[nodiscard]] bool displayOnLegend() override {return false;};
        [[nodiscard]] std::size_t getSortPriority() override {return 14;};
    };

    struct Lava : Terrain {
        [[nodiscard]] std::string getName() override {return "Lava";}
        [[nodiscard]] std::string getMapTile() override {return "@4 @n";}
        [[nodiscard]] std::size_t getId() override {return LAVA;};
        [[nodiscard]] std::size_t getSortPriority() override {return 13;};
    };

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