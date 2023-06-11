#pragma once
#include "dbatk/base.h"
#include "dbatk/object/size.h"

namespace dbat::race {

    enum RaceID : std::size_t {
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
        [[nodiscard]] virtual RaceID getID() = 0;
        [[nodiscard]] virtual std::string getName() = 0;
        // Abbreviation is usually the first three letters of the name, but can be overridden.
        [[nodiscard]] virtual std::string getAbbreviation() {
            if(_abbr.empty()) {
                _abbr = std::string(getName()).substr(0, 3);
            }
            return _abbr;
        }
        [[nodiscard]] virtual bool isPlayable() {return true;}
        [[nodiscard]] virtual size::SizeID getSize() {return size::MEDIUM;}
        std::string _abbr{};
    };

    struct NonPlayableRace : Race {
        [[nodiscard]] bool isPlayable() override {return false;}
    };

    struct Formless : NonPlayableRace {
        [[nodiscard]] RaceID getID() override {return FORMLESS;}
        [[nodiscard]] std::string getName() override {return "Formless";}
        [[nodiscard]] std::string getAbbreviation() override {return "---";}
    };

    struct Human : Race {
        [[nodiscard]] RaceID getID() override {return HUMAN;}
        [[nodiscard]] std::string getName() override {return "Human";}
    };

    struct Saiyan : Race {
        [[nodiscard]] RaceID getID() override {return SAIYAN;}
        [[nodiscard]] std::string getName() override {return "Saiyan";}
    };

    struct Icer : Race {
        [[nodiscard]] RaceID getID() override {return ICER;}
        [[nodiscard]] std::string getName() override {return "Icer";}
    };

    struct Konatsu : Race {
        [[nodiscard]] RaceID getID() override {return KONATSU;}
        [[nodiscard]] std::string getName() override {return "Konatsu";}
    };

    struct Namekian : Race {
        [[nodiscard]] RaceID getID() override {return NAMEKIAN;}
        [[nodiscard]] std::string getName() override {return "Namekian";}
    };

    struct Mutant : Race {
        [[nodiscard]] RaceID getID() override {return MUTANT;}
        [[nodiscard]] std::string getName() override {return "Mutant";}
    };

    struct Kanassan : Race {
        [[nodiscard]] RaceID getID() override {return KANASSAN;}
        [[nodiscard]] std::string getName() override {return "Kanassan";}
    };

    struct HalfSaiyan : Race {
        [[nodiscard]] RaceID getID() override {return HALFSAIYAN;}
        [[nodiscard]] std::string getName() override {return "Half-Saiyan";}
        [[nodiscard]] std::string getAbbreviation() override {return "H-S";}
    };

    struct BioAndroid : Race {
        [[nodiscard]] RaceID getID() override {return BIOANDROID;}
        [[nodiscard]] std::string getName() override {return "Bio-Android";}
    };

    struct Android : Race {
        [[nodiscard]] RaceID getID() override {return ANDROID;}
        [[nodiscard]] std::string getName() override {return "Android";}
    };

    struct Demon : Race {
        [[nodiscard]] RaceID getID() override {return DEMON;}
        [[nodiscard]] std::string getName() override {return "Demon";}
    };

    struct Majin : Race {
        [[nodiscard]] RaceID getID() override {return MAJIN;}
        [[nodiscard]] std::string getName() override {return "Majin";}
    };

    struct Kai : Race {
        [[nodiscard]] RaceID getID() override {return KAI;}
        [[nodiscard]] std::string getName() override {return "Kai";}
    };

    struct Tuffle : Race {
        [[nodiscard]] RaceID getID() override {return TUFFLE;}
        [[nodiscard]] std::string getName() override {return "Tuffle";}
        [[nodiscard]] size::SizeID getSize() override {return size::SMALL;}
    };

    struct Hoshijin : Race {
        [[nodiscard]] RaceID getID() override {return HOSHIJIN;}
        [[nodiscard]] std::string getName() override {return "Hoshijin";}
    };

    struct Animal : NonPlayableRace {
        [[nodiscard]] RaceID getID() override {return ANIMAL;}
        [[nodiscard]] std::string getName() override {return "Animal";}
    };

    struct Saiba : NonPlayableRace {
        [[nodiscard]] RaceID getID() override {return SAIBA;}
        [[nodiscard]] std::string getName() override {return "Saiba";}
    };

    struct Serpent : NonPlayableRace {
        [[nodiscard]] RaceID getID() override {return SERPENT;}
        [[nodiscard]] std::string getName() override {return "Serpent";}
    };

    struct Ogre : NonPlayableRace {
        [[nodiscard]] RaceID getID() override {return OGRE;}
        [[nodiscard]] std::string getName() override {return "Ogre";}
        [[nodiscard]] size::SizeID getSize() override {return size::LARGE;}
    };

    struct Yardratian : NonPlayableRace {
        [[nodiscard]] RaceID getID() override {return YARDRATIAN;}
        [[nodiscard]] std::string getName() override {return "Yardratian";}
    };

    struct Arlian : Race {
        [[nodiscard]] RaceID getID() override {return ARLIAN;}
        [[nodiscard]] std::string getName() override {return "Arlian";}
    };

    struct Dragon : NonPlayableRace {
        [[nodiscard]] RaceID getID() override {return DRAGON;}
        [[nodiscard]] std::string getName() override {return "Dragon";}
        [[nodiscard]] std::string getAbbreviation() override {return "Drg";}
    };

    struct Mechanical : NonPlayableRace {
        [[nodiscard]] RaceID getID() override {return MECHANICAL;}
        [[nodiscard]] std::string getName() override {return "Mechanical";}
    };

    struct Spirit : NonPlayableRace {
        [[nodiscard]] RaceID getID() override {return SPIRIT;}
        [[nodiscard]] std::string getName() override {return "Spirit";}
        [[nodiscard]] size::SizeID getSize() override {return size::TINY;}
    };

    Race* findRace(std::size_t id, std::function<bool(std::shared_ptr<Race>)> predicate = [](std::shared_ptr<Race>) { return true; });
    Race* findRace(const std::string &txt, std::function<bool(std::shared_ptr<Race>)> predicate = [](std::shared_ptr<Race>) { return true; });

    extern std::vector<std::shared_ptr<Race>> races;
}