#pragma once
#include "dbatk/character/character.h"

namespace dbat {
    class Player : public Character {
    public:
        using Character::Character;
        ~Player() override = default;
        std::string getTypeName() const override { return "player"; };
        OpResult<> setRace(race::RaceID id) override;
        OpResult<> setRace(const std::string& txt) override;
        bool isNPC() const override { return false; };
    };
}