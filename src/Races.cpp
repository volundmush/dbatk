#include <kaizermud/Api.h>
#include "dbatk/Races.h"

namespace dbat::race {

    std::vector<std::shared_ptr<Race>> raceRegistry;
    
    void registerRace(const std::shared_ptr<Race>& entry) {
        while(raceRegistry.size() < entry->getID() + 1) {
            raceRegistry.emplace_back(nullptr);
        }
        raceRegistry[entry->getID()] = entry;
    }

    void registerRaces() {
        registerRace(std::make_shared<Mechanical>());
        registerRace(std::make_shared<Spirit>());
        registerRace(std::make_shared<Human>());
        registerRace(std::make_shared<Saiyan>());
        registerRace(std::make_shared<Icer>());
        registerRace(std::make_shared<Konatsu>());
        registerRace(std::make_shared<Namekian>());
        registerRace(std::make_shared<Mutant>());
        registerRace(std::make_shared<Kanassan>());
        registerRace(std::make_shared<HalfSaiyan>());
        registerRace(std::make_shared<BioAndroid>());
        registerRace(std::make_shared<Android>());
        registerRace(std::make_shared<Demon>());
        registerRace(std::make_shared<Majin>());
        registerRace(std::make_shared<Kai>());
        registerRace(std::make_shared<Tuffle>());
        registerRace(std::make_shared<Hoshijin>());
        registerRace(std::make_shared<Animal>());
        registerRace(std::make_shared<Saiba>());
        registerRace(std::make_shared<Serpent>());
        registerRace(std::make_shared<Ogre>());
        registerRace(std::make_shared<Yardratian>());
        registerRace(std::make_shared<Arlian>());
        registerRace(std::make_shared<Dragon>());
    }

}