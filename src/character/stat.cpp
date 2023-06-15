#include "dbatk/character/stat.h"
#include "dbatk/character/character.h"

namespace dbat::cstat {

    const std::unordered_map<StatID, std::shared_ptr<BaseCharacterStat>> characterStats = {
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

    std::array<double, NUM_CHAR_STATS>& BaseCharacterStat::getStorage(const std::shared_ptr<Character>& obj) {
        return obj->getCstats();
    }


    // Real Stats section...

    // Virtual Stats section...
    double Level::calcEffective(const std::shared_ptr<Character> &obj, std::unordered_map<std::size_t, double> &cache) {
        auto exp = characterStats.find(EXPERIENCE);
        return 1.0 + std::floor(exp->second->getEffective(obj, cache) / 1000.0);
    }


}