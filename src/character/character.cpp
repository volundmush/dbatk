#include "dbatk/character/character.h"

namespace dbat {

    size::SizeID Character::getSizeID() const {
        return sizeId.value_or(getRace()->getSize());
    }

    race::RaceID Character::getRaceID() const {
        return raceId;
    }

    race::Race* Character::getRace() const {
        return race::races[raceId].get();
    }

    sensei::SenseiID Character::getSenseiID() const {
        return senseiId;
    }

    sensei::Sensei* Character::getSensei() const {
        return sensei::senseis[senseiId].get();
    }

    position::PositionID Character::getPositionID() const {
        return positionId;
    }

    position::Position* Character::getPosition() const {
        return position::positions[positionId].get();
    }



}