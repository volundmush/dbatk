#pragma once
#include "dbatk/Aspects.h"
#include "boost/algorithm/string.hpp"

namespace dbat::terrain {

    struct Terrain : dbat::aspect::DBATAspect {
        [[nodiscard]] std::string_view getSlot() override {return "terrain";}
        [[nodiscard]] virtual std::string_view getName() = 0;
        [[nodiscard]] virtual std::string_view getMapTile() = 0;
        [[nodiscard]] virtual int getSortPriority() {return 0;};
        [[nodiscard]] virtual bool displayOnLegend() {return true;};
        [[nodiscard]] std::string_view getKey() override {
            if(_key.empty())
                _key = boost::to_lower_copy(std::string(getName()));
            return _key;
        }
        std::string _key;
    };

    struct City : Terrain {
        [[nodiscard]] std::string_view getName() override {return "City";}
        [[nodiscard]] std::string_view getMapTile() override {return "@WC@n";}
        [[nodiscard]] int getSortPriority() override {return 0;};
    };

    struct Inside : Terrain {
        [[nodiscard]] std::string_view getName() override {return "Inside";}
        [[nodiscard]] std::string_view getMapTile() override {return "@wI@n";}
        [[nodiscard]] int getSortPriority() override {return 1;};
    };

    struct Plain : Terrain {
        [[nodiscard]] std::string_view getName() override {return "Plain";}
        [[nodiscard]] std::string_view getMapTile() override {return "@GP@n";}
        [[nodiscard]] int getSortPriority() override {return 2;};
    };

    struct Forest : Terrain {
        [[nodiscard]] std::string_view getName() override {return "Forest";}
        [[nodiscard]] std::string_view getMapTile() override {return "@gF@n";}
        [[nodiscard]] int getSortPriority() override {return 3;};
    };

    struct Mountains : Terrain {
        [[nodiscard]] std::string_view getName() override {return "Mountains";}
        [[nodiscard]] std::string_view getMapTile() override {return "@DM@n";}
        [[nodiscard]] int getSortPriority() override {return 4;};
    };

    struct Hills : Terrain {
        [[nodiscard]] std::string_view getName() override {return "Hills";}
        [[nodiscard]] std::string_view getMapTile() override {return "@yH@n";}
        [[nodiscard]] int getSortPriority() override {return 5;};
    };

    struct Sky : Terrain {
        [[nodiscard]] std::string_view getName() override {return "Sky";}
        [[nodiscard]] std::string_view getMapTile() override {return "@CS@n";}
        [[nodiscard]] int getSortPriority() override {return 6;};
    };

    struct Water : Terrain {
        [[nodiscard]] std::string_view getName() override {return "Water";}
        [[nodiscard]] std::string_view getMapTile() override {return "@BW@n";}
        [[nodiscard]] int getSortPriority() override {return 7;};
    };

    struct UnderWater : Terrain {
        [[nodiscard]] std::string_view getName() override {return "Underwater";}
        [[nodiscard]] std::string_view getMapTile() override {return "@bU@n";}
        [[nodiscard]] int getSortPriority() override {return 8;};
    };

    struct Shop : Terrain {
        [[nodiscard]] std::string_view getName() override {return "Shop";}
        [[nodiscard]] std::string_view getMapTile() override {return "@m$@n";}
        [[nodiscard]] int getSortPriority() override {return 9;};
    };

    struct Important : Terrain {
        [[nodiscard]] std::string_view getName() override {return "Important";}
        [[nodiscard]] std::string_view getMapTile() override {return "@M#@n";}
        [[nodiscard]] int getSortPriority() override {return 10;};
    };

    struct Desert : Terrain {
        [[nodiscard]] std::string_view getName() override {return "Desert";}
        [[nodiscard]] std::string_view getMapTile() override {return "@YD@n";}
        [[nodiscard]] int getSortPriority() override {return 11;};
    };

    struct Shallows : Terrain {
        [[nodiscard]] std::string_view getName() override {return "Shallows";}
        [[nodiscard]] std::string_view getMapTile() override {return "@c~@n";}
        [[nodiscard]] int getSortPriority() override {return 12;};
    };

    struct Lava : Terrain {
        [[nodiscard]] std::string_view getName() override {return "Lava";}
        [[nodiscard]] std::string_view getMapTile() override {return "@4 @n";}
        [[nodiscard]] int getSortPriority() override {return 13;};
    };

    struct Space : Terrain {
        [[nodiscard]] std::string_view getName() override {return "Space";}
        [[nodiscard]] std::string_view getMapTile() override {return "@BS@n";}
        [[nodiscard]] int getSortPriority() override {return 14;};
        [[nodiscard]] bool displayOnLegend() override {return false;};
    };

    void registerTerrainTypes();

}