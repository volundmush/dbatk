#pragma once
#include "dbatk/character/character.h"

namespace dbat {
    class NPC : public Character {
    public:
        using Character::Character;
        ~NPC() override = default;
        std::string getTypeName() const override { return "npc"; };
        OpResult<> setRace(race::RaceID id) override;
        OpResult<> setRace(const std::string& txt) override;
        bool isNPC() const override { return true; };
    };
}