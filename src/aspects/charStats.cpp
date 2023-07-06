#include "dbatk/aspects/charStats.h"
#include "dbatk/components.h"

namespace dbat::cstat {

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

    const std::unordered_map<std::string, StatId> characterStatNames = {
            {"strength", STRENGTH},
            {"intelligence", INTELLIGENCE},
            {"wisdom", WISDOM},
            {"dexterity", DEXTERITY},
            {"constitution", CONSTITUTION},
            {"speed", SPEED},

            {"alignment", ALIGNMENT},
            {"experience", EXPERIENCE},
            {"level", LEVEL},
            {"suppress", SUPPRESS},
            {"armor", ARMOR},

            {"powerlevel", POWERLEVEL},
            {"stamina", STAMINA},
            {"ki", KI},

            {"damage_powerlevel", DAMAGE_POWERLEVEL},
            {"damage_stamina", DAMAGE_STAMINA},
            {"damage_ki", DAMAGE_KI},
            {"damage_lifeforce", DAMAGE_LIFEFORCE},
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