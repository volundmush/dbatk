#pragma once
#include "boost/algorithm/string.hpp"

namespace dbat::sensei {

    // Sensei Stuff
    struct Sensei {
        [[nodiscard]] virtual std::size_t getID() = 0;
        [[nodiscard]] virtual std::string_view getName() = 0;
        // Abbreviation is usually the first two letters of the name, but can be overridden.
        [[nodiscard]] virtual std::string_view getAbbreviation() {
            if(_abbr.empty()) 
                _abbr = getName().substr(0, 2);
            return _abbr;
        }
        [[nodiscard]] virtual std::string_view getStyleName() = 0;
        [[nodiscard]] virtual kaizer::ObjectID getStartRoom() = 0;
        [[nodiscard]] virtual kaizer::ObjectID getLocation() = 0;
        [[nodiscard]] virtual kaizer::OpResult<> canSet(entt::entity ent);
        std::string _abbr;
    };

    struct Untrained : Sensei {
        [[nodiscard]] std::size_t getID() override {return 0;}
        [[nodiscard]] std::string_view getName() override {return "Untrained";}
        [[nodiscard]] std::string_view getStyleName() override {return "Like a Bum";}
        [[nodiscard]] kaizer::ObjectID getStartRoom() override {return 300;}
        [[nodiscard]] kaizer::ObjectID getLocation() override {return 300;}
    };

    struct Roshi : Sensei {
        [[nodiscard]] std::size_t getID() override {return 1;}
        [[nodiscard]] std::string_view getName() override {return "Roshi";}
        [[nodiscard]] std::string_view getStyleName() override {return "Kame Arts";}
        [[nodiscard]] kaizer::ObjectID getStartRoom() override {return 1130;}
        [[nodiscard]] kaizer::ObjectID getLocation() override {return 1131;}
    };

    struct Piccolo : Sensei {
        [[nodiscard]] std::size_t getID() override {return 2;}
        [[nodiscard]] std::string_view getName() override {return "Piccolo";}
        [[nodiscard]] std::string_view getStyleName() override {return "Demon Taijutsu";}
        [[nodiscard]] kaizer::ObjectID getStartRoom() override {return 1659;}
        [[nodiscard]] kaizer::ObjectID getLocation() override {return 1662;}
    };

    struct Krane : Sensei {
        [[nodiscard]] std::size_t getID() override {return 3;}
        [[nodiscard]] std::string_view getName() override {return "Krane";}
        [[nodiscard]] std::string_view getStyleName() override {return "Crane Arts";}
        [[nodiscard]] kaizer::ObjectID getStartRoom() override {return 13009;}
        [[nodiscard]] kaizer::ObjectID getLocation() override {return 13012;}
    };

    struct Nail : Sensei {
        [[nodiscard]] std::size_t getID() override {return 4;}
        [[nodiscard]] std::string_view getName() override {return "Nail";}
        [[nodiscard]] std::string_view getStyleName() override {return "Tranquil Palm";}
        [[nodiscard]] kaizer::ObjectID getStartRoom() override {return 11683;}
        [[nodiscard]] kaizer::ObjectID getLocation() override {return 11683;}
    };

    struct Bardock : Sensei {
        [[nodiscard]] std::size_t getID() override {return 5;}
        [[nodiscard]] std::string_view getName() override {return "Bardock";}
        [[nodiscard]] std::string_view getStyleName() override {return "Brutal Beast";}
        [[nodiscard]] kaizer::ObjectID getStartRoom() override {return 2268;}
        [[nodiscard]] kaizer::ObjectID getLocation() override {return 2267;}
    };

    struct Ginyu : Sensei {
        [[nodiscard]] std::size_t getID() override {return 6;}
        [[nodiscard]] std::string_view getName() override {return "Ginyu";}
        [[nodiscard]] std::string_view getStyleName() override {return "Flaunted Style";}
        [[nodiscard]] kaizer::ObjectID getStartRoom() override {return 4289;}
        [[nodiscard]] kaizer::ObjectID getLocation() override {return 4290;}
    };

    struct Frieza : Sensei {
        [[nodiscard]] std::size_t getID() override {return 7;}
        [[nodiscard]] std::string_view getName() override {return "Frieza";}
        [[nodiscard]] std::string_view getStyleName() override {return "Frozen Fist";}
        [[nodiscard]] kaizer::ObjectID getStartRoom() override {return 4282;}
        [[nodiscard]] kaizer::ObjectID getLocation() override {return 4283;}
    };

    struct Tapion : Sensei {
        [[nodiscard]] std::size_t getID() override {return 8;}
        [[nodiscard]] std::string_view getName() override {return "Tapion";}
        [[nodiscard]] std::string_view getStyleName() override {return "Shadow Grappling";}
        [[nodiscard]] kaizer::ObjectID getStartRoom() override {return 8231;}
        [[nodiscard]] kaizer::ObjectID getLocation() override {return 8233;}
    };

    struct Sixteen : Sensei {
        [[nodiscard]] std::size_t getID() override {return 9;}
        [[nodiscard]] std::string_view getName() override {return "Sixteen";}
        [[nodiscard]] std::string_view getAbbreviation() override {return "16";}
        [[nodiscard]] std::string_view getStyleName() override {return "Iron Hand";}
        [[nodiscard]] kaizer::ObjectID getStartRoom() override {return 1713;}
        [[nodiscard]] kaizer::ObjectID getLocation() override {return 1714;}
        [[nodiscard]] kaizer::OpResult<> canSet(entt::entity ent) override;
    };

    struct Dabura : Sensei {
        [[nodiscard]] std::size_t getID() override {return 10;}
        [[nodiscard]] std::string_view getName() override {return "Dabura";}
        [[nodiscard]] std::string_view getStyleName() override {return "Devil Dance";}
        [[nodiscard]] kaizer::ObjectID getStartRoom() override {return 6487;}
        [[nodiscard]] kaizer::ObjectID getLocation() override {return 6486;}
        [[nodiscard]] kaizer::OpResult<> canSet(entt::entity ent) override;
    };

    struct Kibito : Sensei {
        [[nodiscard]] std::size_t getID() override {return 11;}
        [[nodiscard]] std::string_view getName() override {return "Kibito";}
        [[nodiscard]] std::string_view getStyleName() override {return "Gentle Fist";}
        [[nodiscard]] kaizer::ObjectID getStartRoom() override {return 12098;}
        [[nodiscard]] kaizer::ObjectID getLocation() override {return 12098;}
    };

    struct Jinto : Sensei {
        [[nodiscard]] std::size_t getID() override {return 12;}
        [[nodiscard]] std::string_view getName() override {return "Jinto";}
        [[nodiscard]] std::string_view getStyleName() override {return "Star's Radiance";}
        [[nodiscard]] kaizer::ObjectID getStartRoom() override {return 3499;}
        [[nodiscard]] kaizer::ObjectID getLocation() override {return 3499;}
        [[nodiscard]] kaizer::OpResult<> canSet(entt::entity ent) override;
    };

    struct Tsuna : Sensei {
        [[nodiscard]] std::size_t getID() override {return 13;}
        [[nodiscard]] std::string_view getName() override {return "Tsuna";}
        [[nodiscard]] std::string_view getStyleName() override {return "Sacred Tsunami";}
        [[nodiscard]] kaizer::ObjectID getStartRoom() override {return 15009;}
        [[nodiscard]] kaizer::ObjectID getLocation() override {return 15009;}
        [[nodiscard]] kaizer::OpResult<> canSet(entt::entity ent) override;
    };

    struct Kurzak : Sensei {
        [[nodiscard]] std::size_t getID() override {return 14;}
        [[nodiscard]] std::string_view getName() override {return "Kurzak";}
        [[nodiscard]] std::string_view getStyleName() override {return "Adaptive Taijutsu";}
        [[nodiscard]] kaizer::ObjectID getStartRoom() override {return 16100;}
        [[nodiscard]] kaizer::ObjectID getLocation() override {return 16100;}
        [[nodiscard]] kaizer::OpResult<> canSet(entt::entity ent) override;
    };

    extern std::vector<std::shared_ptr<Sensei>> senseiRegistry;

    void registerSensei(const std::shared_ptr<Sensei>& entry);

    void registerSenseis();
}