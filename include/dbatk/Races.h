#pragma once
#include "dbatk/Aspects.h"
#include "boost/algorithm/string.hpp"


namespace dbat::race {

    // Race
    struct Race : dbat::aspect::DBATAspect {
        [[nodiscard]] std::string_view getSlot() override {return "race";}
        [[nodiscard]] virtual std::string_view getName() = 0;
        // Abbreviation is usually the first three letters of the name, but can be overridden.
        [[nodiscard]] virtual std::string_view getAbbreviation() {
            if(_abbr.empty()) {
                _abbr = std::string(getName()).substr(0, 3);
            }
            return _abbr;
        }
        [[nodiscard]] std::string_view getKey() override {
            if(_key.empty()) {
                _key = boost::to_lower_copy(std::string(getName()));
            }
            return _key;
        }
        [[nodiscard]] virtual bool isPlayable() {return true;}
        [[nodiscard]] virtual int getSize() {return 4;}
        std::string _abbr, _key;
    };

    struct NonPlayableRace : Race {
        [[nodiscard]] kaizer::OpResult<> canSet(entt::entity ent) override;
    };

    struct Human : Race {
        [[nodiscard]] std::string_view getName() override {return "Human";}
    };

    struct Saiyan : Race {
        [[nodiscard]] std::string_view getName() override {return "Saiyan";}
    };

    struct Icer : Race {
        [[nodiscard]] std::string_view getName() override {return "Icer";}
    };

    struct Konatsu : Race {
        [[nodiscard]] std::string_view getName() override {return "Konatsu";}
    };

    struct Namekian : Race {
        [[nodiscard]] std::string_view getName() override {return "Namekian";}
    };

    struct Mutant : Race {
        [[nodiscard]] std::string_view getName() override {return "Mutant";}
    };

    struct Kanassan : Race {
        [[nodiscard]] std::string_view getName() override {return "Kanassan";}
    };

    struct HalfSaiyan : Race {
        [[nodiscard]] std::string_view getName() override {return "Half-Saiyan";}
        [[nodiscard]] std::string_view getKey() override {return "halfsaiyan";}
        [[nodiscard]] std::string_view getAbbreviation() override {return "H-S";}
    };

    struct BioAndroid : Race {
        [[nodiscard]] std::string_view getName() override {return "Bio-Android";}
        [[nodiscard]] std::string_view getKey() override {return "bioandroid";}
    };

    struct Android : Race {
        [[nodiscard]] std::string_view getName() override {return "Android";}
    };

    struct Demon : Race {
        [[nodiscard]] std::string_view getName() override {return "Demon";}
    };

    struct Majin : Race {
        [[nodiscard]] std::string_view getName() override {return "Majin";}
    };

    struct Kai : Race {
        [[nodiscard]] std::string_view getName() override {return "Kai";}
    };

    struct Tuffle : Race {
        [[nodiscard]] std::string_view getName() override {return "Tuffle";}
        [[nodiscard]] int getSize() override {return 3;}
    };

    struct Hoshijin : Race {
        [[nodiscard]] std::string_view getName() override {return "Hoshijin";}
    };

    struct Animal : NonPlayableRace {
        [[nodiscard]] std::string_view getName() override {return "Animal";}
    };

    struct Saiba : NonPlayableRace {
        [[nodiscard]] std::string_view getName() override {return "Saiba";}
    };

    struct Serpent : NonPlayableRace {
        [[nodiscard]] std::string_view getName() override {return "Serpent";}
    };

    struct Ogre : NonPlayableRace {
        [[nodiscard]] std::string_view getName() override {return "Ogre";}
        [[nodiscard]] int getSize() override {return 5;}
    };

    struct Yardratian : NonPlayableRace {
        [[nodiscard]] std::string_view getName() override {return "Yardratian";}
    };

    struct Arlian : Race {
        [[nodiscard]] std::string_view getName() override {return "Arlian";}
    };

    struct Dragon : NonPlayableRace {
        [[nodiscard]] std::string_view getName() override {return "Dragon";}
        [[nodiscard]] std::string_view getAbbreviation() override {return "Drg";}
    };

    struct Mechanical : NonPlayableRace {
        [[nodiscard]] std::string_view getName() override {return "Mechanical";}
    };

    struct Spirit : NonPlayableRace {
        [[nodiscard]] std::string_view getName() override {return "Spirit";}
        [[nodiscard]] int getSize() override {return 2;}
    };


    void registerRaces();
}