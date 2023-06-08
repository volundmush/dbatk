#pragma once
#include "boost/algorithm/string.hpp"


namespace dbat::race {

    // Race
    struct Race {
        [[nodiscard]] virtual std::size_t getID() = 0;
        [[nodiscard]] virtual std::string_view getName() = 0;
        // Abbreviation is usually the first three letters of the name, but can be overridden.
        [[nodiscard]] virtual std::string_view getAbbreviation() {
            if(_abbr.empty()) {
                _abbr = std::string(getName()).substr(0, 3);
            }
            return _abbr;
        }
        [[nodiscard]] virtual bool isPlayable() {return true;}
        [[nodiscard]] virtual int getSize() {return 4;}
        std::string _abbr;
    };

    struct NonPlayableRace : Race {
        [[nodiscard]] bool isPlayable() override {return false;}
    };

    struct Spirit : NonPlayableRace {
        [[nodiscard]] std::size_t getID() override {return 0;}
        [[nodiscard]] std::string_view getName() override {return "Spirit";}
        [[nodiscard]] int getSize() override {return 2;}
    };

    struct Human : Race {
        [[nodiscard]] std::size_t getID() override {return 1;}
        [[nodiscard]] std::string_view getName() override {return "Human";}
    };

    struct Saiyan : Race {
        [[nodiscard]] std::size_t getID() override {return 2;}
        [[nodiscard]] std::string_view getName() override {return "Saiyan";}
    };

    struct Icer : Race {
        [[nodiscard]] std::size_t getID() override {return 3;}
        [[nodiscard]] std::string_view getName() override {return "Icer";}
    };

    struct Konatsu : Race {
        [[nodiscard]] std::size_t getID() override {return 4;}
        [[nodiscard]] std::string_view getName() override {return "Konatsu";}
    };

    struct Namekian : Race {
        [[nodiscard]] std::size_t getID() override {return 5;}
        [[nodiscard]] std::string_view getName() override {return "Namekian";}
    };

    struct Mutant : Race {
        [[nodiscard]] std::size_t getID() override {return 6;}
        [[nodiscard]] std::string_view getName() override {return "Mutant";}
    };

    struct Kanassan : Race {
        [[nodiscard]] std::size_t getID() override {return 7;}
        [[nodiscard]] std::string_view getName() override {return "Kanassan";}
    };

    struct HalfSaiyan : Race {
        [[nodiscard]] std::size_t getID() override {return 8;}
        [[nodiscard]] std::string_view getName() override {return "Half-Saiyan";}
        [[nodiscard]] std::string_view getAbbreviation() override {return "H-S";}
    };

    struct BioAndroid : Race {
        [[nodiscard]] std::size_t getID() override {return 9;}
        [[nodiscard]] std::string_view getName() override {return "Bio-Android";}
    };

    struct Android : Race {
        [[nodiscard]] std::size_t getID() override {return 10;}
        [[nodiscard]] std::string_view getName() override {return "Android";}
    };

    struct Demon : Race {
        [[nodiscard]] std::size_t getID() override {return 11;}
        [[nodiscard]] std::string_view getName() override {return "Demon";}
    };

    struct Majin : Race {
        [[nodiscard]] std::size_t getID() override {return 12;}
        [[nodiscard]] std::string_view getName() override {return "Majin";}
    };

    struct Kai : Race {
        [[nodiscard]] std::size_t getID() override {return 13;}
        [[nodiscard]] std::string_view getName() override {return "Kai";}
    };

    struct Tuffle : Race {
        [[nodiscard]] std::size_t getID() override {return 14;}
        [[nodiscard]] std::string_view getName() override {return "Tuffle";}
        [[nodiscard]] int getSize() override {return 3;}
    };

    struct Hoshijin : Race {
        [[nodiscard]] std::size_t getID() override {return 15;}
        [[nodiscard]] std::string_view getName() override {return "Hoshijin";}
    };

    struct Animal : NonPlayableRace {
        [[nodiscard]] std::size_t getID() override {return 16;}
        [[nodiscard]] std::string_view getName() override {return "Animal";}
    };

    struct Saiba : NonPlayableRace {
        [[nodiscard]] std::size_t getID() override {return 17;}
        [[nodiscard]] std::string_view getName() override {return "Saiba";}
    };

    struct Serpent : NonPlayableRace {
        [[nodiscard]] std::size_t getID() override {return 18;}
        [[nodiscard]] std::string_view getName() override {return "Serpent";}
    };

    struct Ogre : NonPlayableRace {
        [[nodiscard]] std::size_t getID() override {return 19;}
        [[nodiscard]] std::string_view getName() override {return "Ogre";}
        [[nodiscard]] int getSize() override {return 5;}
    };

    struct Yardratian : NonPlayableRace {
        [[nodiscard]] std::size_t getID() override {return 20;}
        [[nodiscard]] std::string_view getName() override {return "Yardratian";}
    };

    struct Arlian : Race {
        [[nodiscard]] std::size_t getID() override {return 21;}
        [[nodiscard]] std::string_view getName() override {return "Arlian";}
    };

    struct Dragon : NonPlayableRace {
        [[nodiscard]] std::size_t getID() override {return 22;}
        [[nodiscard]] std::string_view getName() override {return "Dragon";}
        [[nodiscard]] std::string_view getAbbreviation() override {return "Drg";}
    };

    struct Mechanical : NonPlayableRace {
        [[nodiscard]] std::size_t getID() override {return 23;}
        [[nodiscard]] std::string_view getName() override {return "Mechanical";}
    };

    extern std::vector<std::shared_ptr<Race>> raceRegistry;
    void registerRace(const std::shared_ptr<Race>& entry);
    void registerRaces();
}