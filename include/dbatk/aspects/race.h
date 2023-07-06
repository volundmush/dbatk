#pragma once
#include "dbatk/base.h"
#include "dbatk/aspects/size.h"

namespace dbat::race {

    enum RaceId : uint8_t {
        FORMLESS = 0,
        HUMAN = 1,
        SAIYAN = 2,
        ICER = 3,
        KONATSU = 4,
        NAMEKIAN = 5,
        MUTANT = 6,
        KANASSAN = 7,
        HALFSAIYAN = 8,
        BIOANDROID = 9,
        ANDROID = 10,
        DEMON = 11,
        MAJIN = 12,
        KAI = 13,
        TUFFLE = 14,
        HOSHIJIN = 15,
        ANIMAL = 16,
        SAIBA = 17,
        SERPENT = 18,
        OGRE = 19,
        YARDRATIAN = 20,
        ARLIAN = 21,
        DRAGON = 22,
        MECHANICAL = 23,
        SPIRIT = 24,

    };

    // Race
    struct Race {
        [[nodiscard]] virtual RaceId getId() = 0;
        [[nodiscard]] virtual std::string getName() = 0;
        // Abbreviation is usually the first three letters of the name, but can be overridden.
        [[nodiscard]] virtual std::string getAbbreviation() {
            if(_abbr.empty()) {
                _abbr = std::string(getName()).substr(0, 3);
            }
            return _abbr;
        }
        [[nodiscard]] virtual bool isPlayable() {return true;}
        [[nodiscard]] virtual size::SizeId getSize() {return size::MEDIUM;}
        std::string _abbr{};
    };

    Race* findRace(std::size_t id, std::function<bool(std::shared_ptr<Race>)> predicate = [](std::shared_ptr<Race>) { return true; });
    Race* findRace(const std::string &txt, std::function<bool(std::shared_ptr<Race>)> predicate = [](std::shared_ptr<Race>) { return true; });

    extern std::vector<std::shared_ptr<Race>> races;
}