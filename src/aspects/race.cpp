#include "dbatk/aspects/race.h"

namespace dbat::race {

    struct NonPlayableRace : Race {
        [[nodiscard]] bool isPlayable() override {return false;}
    };

    struct Formless : NonPlayableRace {
        [[nodiscard]] RaceId getId() override {return FORMLESS;}
        [[nodiscard]] std::string getName() override {return "Formless";}
        [[nodiscard]] std::string getAbbreviation() override {return "---";}
    };

    struct Human : Race {
        [[nodiscard]] RaceId getId() override {return HUMAN;}
        [[nodiscard]] std::string getName() override {return "Human";}
    };

    struct Saiyan : Race {
        [[nodiscard]] RaceId getId() override {return SAIYAN;}
        [[nodiscard]] std::string getName() override {return "Saiyan";}
    };

    struct Icer : Race {
        [[nodiscard]] RaceId getId() override {return ICER;}
        [[nodiscard]] std::string getName() override {return "Icer";}
    };

    struct Konatsu : Race {
        [[nodiscard]] RaceId getId() override {return KONATSU;}
        [[nodiscard]] std::string getName() override {return "Konatsu";}
    };

    struct Namekian : Race {
        [[nodiscard]] RaceId getId() override {return NAMEKIAN;}
        [[nodiscard]] std::string getName() override {return "Namekian";}
    };

    struct Mutant : Race {
        [[nodiscard]] RaceId getId() override {return MUTANT;}
        [[nodiscard]] std::string getName() override {return "Mutant";}
    };

    struct Kanassan : Race {
        [[nodiscard]] RaceId getId() override {return KANASSAN;}
        [[nodiscard]] std::string getName() override {return "Kanassan";}
    };

    struct HalfSaiyan : Race {
        [[nodiscard]] RaceId getId() override {return HALFSAIYAN;}
        [[nodiscard]] std::string getName() override {return "Half-Saiyan";}
        [[nodiscard]] std::string getAbbreviation() override {return "H-S";}
    };

    struct BioAndroid : Race {
        [[nodiscard]] RaceId getId() override {return BIOANDROID;}
        [[nodiscard]] std::string getName() override {return "Bio-Android";}
    };

    struct Android : Race {
        [[nodiscard]] RaceId getId() override {return ANDROID;}
        [[nodiscard]] std::string getName() override {return "Android";}
    };

    struct Demon : Race {
        [[nodiscard]] RaceId getId() override {return DEMON;}
        [[nodiscard]] std::string getName() override {return "Demon";}
    };

    struct Majin : Race {
        [[nodiscard]] RaceId getId() override {return MAJIN;}
        [[nodiscard]] std::string getName() override {return "Majin";}
    };

    struct Kai : Race {
        [[nodiscard]] RaceId getId() override {return KAI;}
        [[nodiscard]] std::string getName() override {return "Kai";}
    };

    struct Tuffle : Race {
        [[nodiscard]] RaceId getId() override {return TUFFLE;}
        [[nodiscard]] std::string getName() override {return "Tuffle";}
        [[nodiscard]] size::SizeId getSize() override {return size::SMALL;}
    };

    struct Hoshijin : Race {
        [[nodiscard]] RaceId getId() override {return HOSHIJIN;}
        [[nodiscard]] std::string getName() override {return "Hoshijin";}
    };

    struct Animal : NonPlayableRace {
        [[nodiscard]] RaceId getId() override {return ANIMAL;}
        [[nodiscard]] std::string getName() override {return "Animal";}
    };

    struct Saiba : NonPlayableRace {
        [[nodiscard]] RaceId getId() override {return SAIBA;}
        [[nodiscard]] std::string getName() override {return "Saiba";}
    };

    struct Serpent : NonPlayableRace {
        [[nodiscard]] RaceId getId() override {return SERPENT;}
        [[nodiscard]] std::string getName() override {return "Serpent";}
    };

    struct Ogre : NonPlayableRace {
        [[nodiscard]] RaceId getId() override {return OGRE;}
        [[nodiscard]] std::string getName() override {return "Ogre";}
        [[nodiscard]] size::SizeId getSize() override {return size::LARGE;}
    };

    struct Yardratian : NonPlayableRace {
        [[nodiscard]] RaceId getId() override {return YARDRATIAN;}
        [[nodiscard]] std::string getName() override {return "Yardratian";}
    };

    struct Arlian : Race {
        [[nodiscard]] RaceId getId() override {return ARLIAN;}
        [[nodiscard]] std::string getName() override {return "Arlian";}
    };

    struct Dragon : NonPlayableRace {
        [[nodiscard]] RaceId getId() override {return DRAGON;}
        [[nodiscard]] std::string getName() override {return "Dragon";}
        [[nodiscard]] std::string getAbbreviation() override {return "Drg";}
    };

    struct Mechanical : NonPlayableRace {
        [[nodiscard]] RaceId getId() override {return MECHANICAL;}
        [[nodiscard]] std::string getName() override {return "Mechanical";}
    };

    struct Spirit : NonPlayableRace {
        [[nodiscard]] RaceId getId() override {return SPIRIT;}
        [[nodiscard]] std::string getName() override {return "Spirit";}
        [[nodiscard]] size::SizeId getSize() override {return size::TINY;}
    };


    std::vector<std::shared_ptr<Race>> races = {
            std::make_shared<Formless>(),
            std::make_shared<Human>(),
            std::make_shared<Saiyan>(),
            std::make_shared<Icer>(),
            std::make_shared<Konatsu>(),
            std::make_shared<Namekian>(),
            std::make_shared<Mutant>(),
            std::make_shared<Kanassan>(),
            std::make_shared<HalfSaiyan>(),
            std::make_shared<BioAndroid>(),
            std::make_shared<Android>(),
            std::make_shared<Demon>(),
            std::make_shared<Majin>(),
            std::make_shared<Kai>(),
            std::make_shared<Tuffle>(),
            std::make_shared<Hoshijin>(),
            std::make_shared<Animal>(),
            std::make_shared<Saiba>(),
            std::make_shared<Serpent>(),
            std::make_shared<Ogre>(),
            std::make_shared<Yardratian>(),
            std::make_shared<Arlian>(),
            std::make_shared<Dragon>(),
            std::make_shared<Mechanical>(),
            std::make_shared<Spirit>()
    };

    Race* findRace(std::size_t id, std::function<bool(std::shared_ptr<Race>)> predicate) {
        // First, if the id is not within the range of races, return nullptr.
        if(id > races.size() - 1) {
            return nullptr;
        }
        auto &r = races[id];
        // check predicate...
        if(predicate(r)) {
            return r.get();
        }
        return nullptr;
    }

    Race* findRace(const std::string &txt, std::function<bool(std::shared_ptr<Race>)> predicate) {
        std::vector<std::shared_ptr<Race>> filtered;
        std::copy_if(races.begin(), races.end(), std::back_inserter(filtered), predicate);
        auto res = partialMatch(txt, filtered.begin(), filtered.end(), false, [](auto r) { return r->getName(); });
        return res == filtered.end() ? nullptr : (*res).get();
    }


}