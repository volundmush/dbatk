#include "dbatk/aspects/race.h"

namespace dbat::race {

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