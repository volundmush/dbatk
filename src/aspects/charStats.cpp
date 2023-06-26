#include "dbatk/aspects/charStats.h"
#include "dbatk/components.h"

namespace dbat::cstat {

    const std::unordered_map<StatId, std::shared_ptr<BaseCharacterStat>> characterStats = {
            // Real stats.
            {STRENGTH, std::make_shared<Strength>()},
            {INTELLIGENCE, std::make_shared<Intelligence>()},
            {WISDOM, std::make_shared<Wisdom>()},
            {DEXTERITY, std::make_shared<Dexterity>()},
            {CONSTITUTION, std::make_shared<Constitution>()},
            {SPEED, std::make_shared<Speed>()},

            {ALIGNMENT, std::make_shared<Alignment>()},
            {EXPERIENCE, std::make_shared<Experience>()},
            {LEVEL, std::make_shared<Level>()},
            {SUPPRESS, std::make_shared<Suppress>()},
            {ARMOR, std::make_shared<Armor>()},

            {POWERLEVEL, std::make_shared<PowerLevel>()},
            {STAMINA, std::make_shared<Stamina>()},
            {KI, std::make_shared<Ki>()},

            {DAMAGE_POWERLEVEL, std::make_shared<DamagePowerLevel>()},
            {DAMAGE_STAMINA, std::make_shared<DamageStamina>()},
            {DAMAGE_KI, std::make_shared<DamageKi>()},
            {DAMAGE_LIFEFORCE, std::make_shared<DamageLifeForce>()},

            // Virtual Stats follow...
    };

    double BaseCharacterStat::getBase(entt::entity ent) {
        // if ID is within the NUM_CHAR_STATS range, get base from CharacterComponent, else return 0.0.
        auto id = getId();
        if (id < numCharStats) {
            auto &c = registry.get_or_emplace<CharacterStats>(ent);
            return c.data[id];
        } else {
            return 0.0;
        }
    }

    void BaseCharacterStat::doSetBase(entt::entity ent, double value) {
        auto id = getId();
        if (id < numCharStats) {
            auto &c = registry.get_or_emplace<CharacterStats>(ent);
            c.data[id] = value;
        }
    }

    // Real Stats section...



}