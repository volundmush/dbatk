#include <kaizermud/Api.h>
#include "dbatk/Races.h"

namespace dbat::race {

    kaizer::OpResult<> NonPlayableRace::canSet(entt::entity ent) {
        if(kaizer::getType(ent, "player")) {
            return {false, "This race is only available for NPCs!"};
        }
        return {true, std::nullopt};
    }


    void registerRaces() {
        auto spirit = std::make_shared<Spirit>();
        registerAspect(spirit);
        kaizer::aspectSlotDefaults["race"] = spirit;

        registerAspect(std::make_shared<Human>());
        registerAspect(std::make_shared<Saiyan>());
        registerAspect(std::make_shared<Icer>());
        registerAspect(std::make_shared<Konatsu>());
        registerAspect(std::make_shared<Namekian>());
        registerAspect(std::make_shared<Mutant>());
        registerAspect(std::make_shared<Kanassan>());
        registerAspect(std::make_shared<HalfSaiyan>());
        registerAspect(std::make_shared<BioAndroid>());
        registerAspect(std::make_shared<Android>());
        registerAspect(std::make_shared<Demon>());
        registerAspect(std::make_shared<Majin>());
        registerAspect(std::make_shared<Kai>());
        registerAspect(std::make_shared<Tuffle>());
        registerAspect(std::make_shared<Hoshijin>());
        registerAspect(std::make_shared<Animal>());
        registerAspect(std::make_shared<Saiba>());
        registerAspect(std::make_shared<Serpent>());
        registerAspect(std::make_shared<Ogre>());
        registerAspect(std::make_shared<Yardratian>());
        registerAspect(std::make_shared<Arlian>());
        registerAspect(std::make_shared<Dragon>());
        registerAspect(std::make_shared<Mechanical>());
    }

}