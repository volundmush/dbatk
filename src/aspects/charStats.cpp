#include "dbatk/aspects/charStats.h"
#include "dbatk/components.h"

namespace dbat::cstat {

    const std::unordered_map<StatId, std::shared_ptr<BaseCharacterStat>> characterStats = {
            // Real stats.
            {STRENGTH, std::make_shared<Strength>()},
            {CONSTITUTION, std::make_shared<Constitution>()},
            {AGILITY, std::make_shared<Agility>()},
            {INTELLIGENCE, std::make_shared<Intelligence>()},
            {SPIRIT, std::make_shared<Spirit>()},
            {WISDOM, std::make_shared<Wisdom>()},
            {ALIGNMENT, std::make_shared<Alignment>()},
            {EXPERIENCE, std::make_shared<Experience>()},
            {DAMAGE_LIFEFORCE, std::make_shared<DamageLifeForce>()},
            {DAMAGE_STAMINA, std::make_shared<DamageStamina>()},

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

    // Virtual Stats section...
    double Level::calcEffective(entt::entity ent, std::unordered_map<std::size_t, double> &cache) {
        auto exp = characterStats.find(EXPERIENCE);
        return 1.0 + std::floor(exp->second->getEffective(ent, cache) / 1000.0);
    }


}