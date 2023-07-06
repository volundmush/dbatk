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

    extern std::vector<std::shared_ptr<Sensei>> senseis;

    Sensei* findSensei(std::size_t id, std::function<bool(std::shared_ptr<Sensei>)> predicate = [](std::shared_ptr<Sensei>) { return true; });
    Sensei* findSensei(const std::string &txt, std::function<bool(std::shared_ptr<Sensei>)> predicate = [](std::shared_ptr<Sensei>) { return true; });

}