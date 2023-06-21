#pragma once
#include "dbatk/base.h"

namespace dbat::sensei {

    enum SenseiId : uint8_t {
        UNTRAINED = 0,
        ROSHI = 1,
        PICCOLO = 2,
        KRANE = 3,
        NAIL = 4,
        BARDOCK = 5,
        GINYU = 6,
        FRIEZA = 7,
        TAPION = 8,
        SIXTEEN = 9,
        DABURA = 10,
        KIBITO = 11,
        JINTO = 12,
        TSUNA = 13,
        KURZAK = 14
    };

    // Sensei Stuff
    struct Sensei {
        [[nodiscard]] virtual std::size_t getId() = 0;
        [[nodiscard]] virtual std::string getName() = 0;
        // Abbreviation is usually the first two letters of the name, but can be overridden.
        [[nodiscard]] virtual std::string getAbbreviation() {
            if(_abbr.empty()) 
                _abbr = getName().substr(0, 2);
            return _abbr;
        }
        [[nodiscard]] virtual std::string getStyleName() = 0;
        [[nodiscard]] virtual RoomId getStartRoom() = 0;
        [[nodiscard]] virtual RoomId getLocation() = 0;
        [[nodiscard]] virtual OpResult<> canSet(entt::entity target);
        std::string _abbr;
    };

    struct Untrained : Sensei {
        [[nodiscard]] std::size_t getId() override {return UNTRAINED;}
        [[nodiscard]] std::string getName() override {return "Untrained";}
        [[nodiscard]] std::string getStyleName() override {return "Like a Bum";}
        [[nodiscard]] RoomId getStartRoom() override {return 300;}
        [[nodiscard]] RoomId getLocation() override {return 300;}
    };

    struct Roshi : Sensei {
        [[nodiscard]] std::size_t getId() override {return ROSHI;}
        [[nodiscard]] std::string getName() override {return "Roshi";}
        [[nodiscard]] std::string getStyleName() override {return "Kame Arts";}
        [[nodiscard]] RoomId getStartRoom() override {return 1130;}
        [[nodiscard]] RoomId getLocation() override {return 1131;}
    };

    struct Piccolo : Sensei {
        [[nodiscard]] std::size_t getId() override {return PICCOLO;}
        [[nodiscard]] std::string getName() override {return "Piccolo";}
        [[nodiscard]] std::string getStyleName() override {return "Demon Taijutsu";}
        [[nodiscard]] RoomId getStartRoom() override {return 1659;}
        [[nodiscard]] RoomId getLocation() override {return 1662;}
    };

    struct Krane : Sensei {
        [[nodiscard]] std::size_t getId() override {return KRANE;}
        [[nodiscard]] std::string getName() override {return "Krane";}
        [[nodiscard]] std::string getStyleName() override {return "Crane Arts";}
        [[nodiscard]] RoomId getStartRoom() override {return 13009;}
        [[nodiscard]] RoomId getLocation() override {return 13012;}
    };

    struct Nail : Sensei {
        [[nodiscard]] std::size_t getId() override {return NAIL;}
        [[nodiscard]] std::string getName() override {return "Nail";}
        [[nodiscard]] std::string getStyleName() override {return "Tranquil Palm";}
        [[nodiscard]] RoomId getStartRoom() override {return 11683;}
        [[nodiscard]] RoomId getLocation() override {return 11683;}
    };

    struct Bardock : Sensei {
        [[nodiscard]] std::size_t getId() override {return BARDOCK;}
        [[nodiscard]] std::string getName() override {return "Bardock";}
        [[nodiscard]] std::string getStyleName() override {return "Brutal Beast";}
        [[nodiscard]] RoomId getStartRoom() override {return 2268;}
        [[nodiscard]] RoomId getLocation() override {return 2267;}
    };

    struct Ginyu : Sensei {
        [[nodiscard]] std::size_t getId() override {return GINYU;}
        [[nodiscard]] std::string getName() override {return "Ginyu";}
        [[nodiscard]] std::string getStyleName() override {return "Flaunted Style";}
        [[nodiscard]] RoomId getStartRoom() override {return 4289;}
        [[nodiscard]] RoomId getLocation() override {return 4290;}
    };

    struct Frieza : Sensei {
        [[nodiscard]] std::size_t getId() override {return FRIEZA;}
        [[nodiscard]] std::string getName() override {return "Frieza";}
        [[nodiscard]] std::string getStyleName() override {return "Frozen Fist";}
        [[nodiscard]] RoomId getStartRoom() override {return 4282;}
        [[nodiscard]] RoomId getLocation() override {return 4283;}
    };

    struct Tapion : Sensei {
        [[nodiscard]] std::size_t getId() override {return TAPION;}
        [[nodiscard]] std::string getName() override {return "Tapion";}
        [[nodiscard]] std::string getStyleName() override {return "Shadow Grappling";}
        [[nodiscard]] RoomId getStartRoom() override {return 8231;}
        [[nodiscard]] RoomId getLocation() override {return 8233;}
    };

    struct Sixteen : Sensei {
        [[nodiscard]] std::size_t getId() override {return SIXTEEN;}
        [[nodiscard]] std::string getName() override {return "Sixteen";}
        [[nodiscard]] std::string getAbbreviation() override {return "16";}
        [[nodiscard]] std::string getStyleName() override {return "Iron Hand";}
        [[nodiscard]] RoomId getStartRoom() override {return 1713;}
        [[nodiscard]] RoomId getLocation() override {return 1714;}
        [[nodiscard]] OpResult<> canSet(entt::entity target) override;
    };

    struct Dabura : Sensei {
        [[nodiscard]] std::size_t getId() override {return DABURA;}
        [[nodiscard]] std::string getName() override {return "Dabura";}
        [[nodiscard]] std::string getStyleName() override {return "Devil Dance";}
        [[nodiscard]] RoomId getStartRoom() override {return 6487;}
        [[nodiscard]] RoomId getLocation() override {return 6486;}
        [[nodiscard]] OpResult<> canSet(entt::entity target) override;
    };

    struct Kibito : Sensei {
        [[nodiscard]] std::size_t getId() override {return KIBITO;}
        [[nodiscard]] std::string getName() override {return "Kibito";}
        [[nodiscard]] std::string getStyleName() override {return "Gentle Fist";}
        [[nodiscard]] RoomId getStartRoom() override {return 12098;}
        [[nodiscard]] RoomId getLocation() override {return 12098;}
    };

    struct Jinto : Sensei {
        [[nodiscard]] std::size_t getId() override {return JINTO;}
        [[nodiscard]] std::string getName() override {return "Jinto";}
        [[nodiscard]] std::string getStyleName() override {return "Star's Radiance";}
        [[nodiscard]] RoomId getStartRoom() override {return 3499;}
        [[nodiscard]] RoomId getLocation() override {return 3499;}
        [[nodiscard]] OpResult<> canSet(entt::entity target) override;
    };

    struct Tsuna : Sensei {
        [[nodiscard]] std::size_t getId() override {return TSUNA;}
        [[nodiscard]] std::string getName() override {return "Tsuna";}
        [[nodiscard]] std::string getStyleName() override {return "Sacred Tsunami";}
        [[nodiscard]] RoomId getStartRoom() override {return 15009;}
        [[nodiscard]] RoomId getLocation() override {return 15009;}
        [[nodiscard]] OpResult<> canSet(entt::entity target) override;
    };

    struct Kurzak : Sensei {
        [[nodiscard]] std::size_t getId() override {return KURZAK;}
        [[nodiscard]] std::string getName() override {return "Kurzak";}
        [[nodiscard]] std::string getStyleName() override {return "Adaptive Taijutsu";}
        [[nodiscard]] RoomId getStartRoom() override {return 16100;}
        [[nodiscard]] RoomId getLocation() override {return 16100;}
        [[nodiscard]] OpResult<> canSet(entt::entity target) override;
    };

    extern std::vector<std::shared_ptr<Sensei>> senseis;

    Sensei* findSensei(std::size_t id, std::function<bool(std::shared_ptr<Sensei>)> predicate = [](std::shared_ptr<Sensei>) { return true; });
    Sensei* findSensei(const std::string &txt, std::function<bool(std::shared_ptr<Sensei>)> predicate = [](std::shared_ptr<Sensei>) { return true; });

}