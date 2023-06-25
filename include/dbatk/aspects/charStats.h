#pragma once
#include "dbatk/base.h"
#include "core/stat.h"

namespace dbat::cstat {

    enum StatId : uint16_t {
        // REAL STATS...
        // Base Attributes
        STRENGTH = 0,
        CONSTITUTION = 1,
        AGILITY = 2,
        INTELLIGENCE = 3,
        SPIRIT = 4,
        WISDOM = 5,

        // The Good vs Evil thing still seems weird to have based on stats, but.. for now...
        ALIGNMENT = 6,
        EXPERIENCE = 7,
        // Power is how much effort a character is putting forth. It ranges from 1 to 100 as a percentage.
        // If Power is 100 then a character is putting forth 100% of their effort.
        // This is variable so that a character can 'power down/suppress' their abilities for sparring and
        // or hiding their true power, etc.
        POWER = 8,

        DAMAGE_LIFEFORCE = 9,
        DAMAGE_STAMINA = 10,

        // The innate toughness of a character. Mostly, only NPCs have an actual
        // base stat, for PCs it mostly comes from their equiipment bonus.
        ARMOR = 11,

        // VIRTUAL STATS... starting at 1000 to avoid collisions with real stats.
        // Thank you copilot.
        // These are stats that are calculated from other stats or traits.
        // Level is EXPERIENCE / 1000.
        LEVEL = 1000,
        BASE_POWERLEVEL = 1001,
        FULL_POWERLEVEL = 1002,
        MAX_LIFEFORCE = 1003,
        MAX_STAMINA = 1005,

    };

    constexpr std::size_t numCharStats = 12;

    class BaseCharacterStat : public BaseStat {
    public:
        double getBase(entt::entity ent) override;
    protected:
        void doSetBase(entt::entity ent, double value) override;
    };

    // Real Stats section...
    class AttributeStat : public BaseCharacterStat {
    public:
        [[nodiscard]] double getDefault() const override {return 5.0;};
        [[nodiscard]] std::optional<double> getMinimumBase() const override {return 5.0;};
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

    class Agility : public AttributeStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return AGILITY;};
        [[nodiscard]] std::string getName() const override {return "agility";};
    };

    class Intelligence : public AttributeStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return INTELLIGENCE;};
        [[nodiscard]] std::string getName() const override {return "intelligence";};
    };

    class Spirit : public AttributeStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return SPIRIT;};
        [[nodiscard]] std::string getName() const override {return "spirit";};
    };

    class Wisdom : public AttributeStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return WISDOM;};
        [[nodiscard]] std::string getName() const override {return "wisdom";};
    };

    class Alignment : public BaseCharacterStat {
        [[nodiscard]] std::size_t getId() const override {return ALIGNMENT;};
        [[nodiscard]] std::string getName() const override {return "alignment";};
        [[nodiscard]] double getDefault() const override {return 0.0;};
        [[nodiscard]] std::optional<double> getMinimumBase() const override {return -1000.0;};
        [[nodiscard]] std::optional<double> getMaximumBase() const override {return 1000.0;};
    };

    class Experience : public BaseCharacterStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return EXPERIENCE;};
        [[nodiscard]] std::string getName() const override {return "experience";};
        [[nodiscard]] double getDefault() const override {return 0.0;};
        [[nodiscard]] std::optional<double> getMinimumBase() const override {return 0.0;};
    };

    class DamageLifeForce : public BaseCharacterStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return DAMAGE_LIFEFORCE;};
        [[nodiscard]] std::string getName() const override {return "damage_lifeforce";};
        [[nodiscard]] double getDefault() const override {return 0.0;};
        [[nodiscard]] std::optional<double> getMinimumBase() const override {return 0.0;};
    };

    class DamageStamina : public BaseCharacterStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return DAMAGE_STAMINA;};
        [[nodiscard]] std::string getName() const override {return "damage_stamina";};
        [[nodiscard]] double getDefault() const override {return 0.0;};
        [[nodiscard]] std::optional<double> getMinimumBase() const override {return 0.0;};
    };

    class Armor : public BaseCharacterStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return ARMOR;};
        [[nodiscard]] std::string getName() const override {return "armor";};
        [[nodiscard]] double getDefault() const override {return 0.0;};
        [[nodiscard]] std::optional<double> getMinimumBase() const override {return 0.0;};
    };

    // Virtual Stats
    // Level first. It is 1 + (experience / 1000) rounded down.
    class Level : public BaseCharacterStat {
    public:
        [[nodiscard]] std::size_t getId() const override {return LEVEL;};
        [[nodiscard]] std::string getName() const override {return "level";};
        [[nodiscard]] double getDefault() const override {return 1.0;};
        [[nodiscard]] std::optional<double> getMinimumBase() const override {return 1.0;};
        [[nodiscard]] double calcEffective(entt::entity ent, std::unordered_map<std::size_t, double>& cache) override;
    };

    extern const std::unordered_map<StatId, std::shared_ptr<BaseCharacterStat>> characterStats;

}