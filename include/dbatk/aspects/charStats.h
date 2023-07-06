#pragma once
#include "dbatk/base.h"
#include "dbatk/stat.h"

namespace dbat::cstat {

    enum StatId : int16_t {
        // REAL STATS...
        // Base Attributes
        STRENGTH = 0,
        INTELLIGENCE = 1,
        WISDOM = 2,
        DEXTERITY = 3,
        CONSTITUTION = 4,
        SPEED = 5,

        ALIGNMENT = 6,
        EXPERIENCE = 7,
        LEVEL = 8,
        SUPPRESS = 9,
        ARMOR = 10,

        POWERLEVEL = 11,
        STAMINA = 12,
        KI = 13,

        DAMAGE_POWERLEVEL = 14,
        DAMAGE_STAMINA = 15,
        DAMAGE_KI = 16,
        DAMAGE_LIFEFORCE = 17,

        // VIRTUAL STATS... starting at 1000 to avoid collisions with real stats.

        // These are stats that are calculated from other stats or traits.
        SPEEDNAR = 1000,
        MAX_CARRY_WEIGHT = 1001,
        CURRENT_CARRY_WEIGHT = 1002,
        EQUIPPED_WEIGHT = 1003,

        BASE_POWERLEVEL = 2000,
        TRUE_FULL_POWERLEVEL = 2001,
        FULL_POWERLEVEL = 2002,
        CURRENT_POWERLEVEL = 2003,

        BASE_STAMINA = 2010,
        TRUE_FULL_STAMINA = 2011,
        FULL_STAMINA = 2012,
        CURRENT_STAMINA = 2013,

        BASE_KI = 2020,
        TRUE_FULL_KI = 2021,
        FULL_KI = 2022,
        CURRENT_KI = 2023,

        BASE_LIFEFORCE = 2030,
        TRUE_FULL_LIFEFORCE = 2031,
        FULL_LIFEFORCE = 2032,
        CURRENT_LIFEFORCE = 2033,
    };

    constexpr std::size_t numCharStats = 18;

    class BaseCharacterStat : public BaseStat {
    public:
        double getBase(entt::entity ent) override;
    protected:
        void doSetBase(entt::entity ent, double value) override;
    };



    extern const std::unordered_map<StatId, std::shared_ptr<BaseCharacterStat>> characterStats;
    extern const std::unordered_map<std::string, StatId> characterStatNames;

}