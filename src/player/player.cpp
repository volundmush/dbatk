#include "dbatk/player/player.h"

namespace dbat {

    static bool checkPlayableRace(std::shared_ptr<race::Race> r) {
        if(r->isPlayable()) {
            return true;
        }
        return false;
    }

    OpResult<> Player::setRace(race::RaceID id) {
        auto r = race::findRace(id, &checkPlayableRace);
        if(!r) {
            return {false, "Race not found."};
        }
        raceId = r->getID();
        return {true, std::nullopt};
    }

    OpResult<> Player::setRace(const std::string& txt) {
        auto r = race::findRace(txt, &checkPlayableRace);
        if(!r) {
            return {false, "Race not found."};
        }
        raceId = r->getID();
        return {true, std::nullopt};
    }

}