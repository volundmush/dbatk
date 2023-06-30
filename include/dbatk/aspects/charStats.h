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

    // Real Stats section...
    class AttributeStat : public BaseCharacterStat {
    public:
        [[nodiscard]] double getDefault(entt::entity ent) const override {return 5.0;};
        [[nodiscard]] std::optional<double> getMinimumBase(entt::entity ent) const override {return 5.0;};
    };

    class Strength : public AttributeStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return STRENGTH;};
        [[nodiscard]] std::string getName() const override {return "strength";};
    };

    class Constitution : public AttributeStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return CONSTITUTION;};
        [[nodiscard]] std::string getName() const override {return "constitution";};
    };

    class Dexterity : public AttributeStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return DEXTERITY;};
        [[nodiscard]] std::string getName() const override {return "dexterity";};
    };

    class Intelligence : public AttributeStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return INTELLIGENCE;};
        [[nodiscard]] std::string getName() const override {return "intelligence";};
    };

    class Speed : public AttributeStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return SPEED;};
        [[nodiscard]] std::string getName() const override {return "speed";};
    };

    class Wisdom : public AttributeStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return WISDOM;};
        [[nodiscard]] std::string getName() const override {return "wisdom";};
    };

    class Alignment : public BaseCharacterStat {
        [[nodiscard]] std::size_t getId() const override {return ALIGNMENT;};
        [[nodiscard]] std::string getName() const override {return "alignment";};
        [[nodiscard]] double getDefault(entt::entity ent) const override {return 0.0;};
        [[nodiscard]] std::optional<double> getMinimumBase(entt::entity ent) const override {return -1000.0;};
        [[nodiscard]] std::optional<double> getMaximumBase(entt::entity ent) const override {return 1000.0;};
    };

    class Experience : public BaseCharacterStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return EXPERIENCE;};
        [[nodiscard]] std::string getName() const override {return "experience";};
        [[nodiscard]] double getDefault(entt::entity ent) const override {return 0.0;};
        [[nodiscard]] std::optional<double> getMinimumBase(entt::entity ent) const override {return 0.0;};
    };

    class Level : public BaseCharacterStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return LEVEL;};
        [[nodiscard]] std::string getName() const override {return "level";};
        [[nodiscard]] double getDefault(entt::entity ent) const override {return 1.0;};
        [[nodiscard]] std::optional<double> getMinimumBase(entt::entity ent) const override {return 1.0;};
    };

    class Suppress : public BaseCharacterStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return SUPPRESS;};
        [[nodiscard]] std::string getName() const override {return "suppress";};
        [[nodiscard]] double getDefault(entt::entity ent) const override {return 100.0;};
        [[nodiscard]] std::optional<double> getMaximumBase(entt::entity ent) const override {return 100.0;};
        [[nodiscard]] std::optional<double> getMinimumBase(entt::entity ent) const override {return 1.0;};
    };

    class Armor : public BaseCharacterStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return ARMOR;};
        [[nodiscard]] std::string getName() const override {return "armor";};
        [[nodiscard]] double getDefault(entt::entity ent) const override {return 0.0;};
    };

    class PowerLevel : public BaseCharacterStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return POWERLEVEL;};
        [[nodiscard]] std::string getName() const override {return "powerlevel";};
        [[nodiscard]] double getDefault(entt::entity ent) const override {return 5.0;};
        [[nodiscard]] std::optional<double> getMinimumBase(entt::entity ent) const override {return 1.0;};
    };

    class Stamina : public BaseCharacterStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return STAMINA;};
        [[nodiscard]] std::string getName() const override {return "stamina";};
        [[nodiscard]] double getDefault(entt::entity ent) const override {return 5.0;};
        [[nodiscard]] std::optional<double> getMinimumBase(entt::entity ent) const override {return 1.0;};
    };

    class Ki : public BaseCharacterStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return KI;};
        [[nodiscard]] std::string getName() const override {return "ki";};
        [[nodiscard]] double getDefault(entt::entity ent) const override {return 5.0;};
        [[nodiscard]] std::optional<double> getMinimumBase(entt::entity ent) const override {return 1.0;};
    };

    class DamagePowerLevel : public BaseCharacterStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return DAMAGE_POWERLEVEL;};
        [[nodiscard]] std::string getName() const override {return "damage_powerlevel";};
        [[nodiscard]] double getDefault(entt::entity ent) const override {return 0.0;};
        [[nodiscard]] std::optional<double> getMinimumBase(entt::entity ent) const override {return 0.0;};
    };

    class DamageStamina : public BaseCharacterStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return DAMAGE_STAMINA;};
        [[nodiscard]] std::string getName() const override {return "damage_stamina";};
        [[nodiscard]] double getDefault(entt::entity ent) const override {return 0.0;};
        [[nodiscard]] std::optional<double> getMinimumBase(entt::entity ent) const override {return 0.0;};
    };

    class DamageKi : public BaseCharacterStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return DAMAGE_KI;};
        [[nodiscard]] std::string getName() const override {return "damage_ki";};
        [[nodiscard]] double getDefault(entt::entity ent) const override {return 0.0;};
        [[nodiscard]] std::optional<double> getMinimumBase(entt::entity ent) const override {return 0.0;};
    };

    class DamageLifeForce : public BaseCharacterStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return DAMAGE_LIFEFORCE;};
        [[nodiscard]] std::string getName() const override {return "damage_lifeforce";};
        [[nodiscard]] double getDefault(entt::entity ent) const override {return 0.0;};
        [[nodiscard]] std::optional<double> getMinimumBase(entt::entity ent) const override {return 0.0;};
    };


    // Virtual Stats
    // Level first. It is 1 + (experience / 1000) rounded down.

    extern const std::unordered_map<StatId, std::shared_ptr<BaseCharacterStat>> characterStats;
    extern const std::unordered_map<std::string, StatId> characterStatNames;

}