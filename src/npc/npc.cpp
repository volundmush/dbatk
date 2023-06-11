#include "dbatk/npc/npc.h"

namespace dbat {

    OpResult<> NPC::setRace(race::RaceID id) {
        auto r = race::findRace(id);
        if(!r) {
            return {false, "Race not found."};
        }
        raceId = r->getID();
        return {true, std::nullopt};
    }

    OpResult<> NPC::setRace(const std::string& txt) {
        auto r = race::findRace(txt);
        if(!r) {
            return {false, "Race not found."};
        }
        raceId = r->getID();
        return {true, std::nullopt};
    }

}