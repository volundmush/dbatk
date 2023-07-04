#pragma once
#include "dbatk/aspects/flags.h"

namespace dbat::iflags {
    enum IFlagId : uint8_t {
        GLOW = 0, // Item is glowing
        HUM = 1, // Item is humming
        NORENT = 2, // Item cannot be rented
        NODONATE = 3, // Item cannot be donated
        NOINVIS = 4, // Item cannot be made invis
        INVISIBLE = 5, // Item is invisible
        MAGIC = 6, // Item is magical
        NODROP = 7, // Item is cursed: can't drop
        BLESS = 8, // Item is blessed
        ANTI_GOOD = 9, // Not usable by good people
        ANTI_EVIL = 10, // Not usable by evil people
        ANTI_NEUTRAL = 11, // Not usable by neutral people
        ANTI_WIZARD = 12, // Not usable by mages
        ANTI_CLERIC = 13, // Not usable by clerics
        ANTI_ROGUE = 14, // Not usable by thieves
        ANTI_FIGHTER = 15, // Not usable by warriors
        NOSELL = 16, // Shopkeepers won't touch it
        ANTI_DRUID = 17, // Not usable by druids
        TWO_HAND = 18, // Requires two free hands
        ANTI_BARD = 19, // Not usable by bards
        ANTI_RANGER = 20, // Not usable by rangers
        ANTI_PALADIN = 21, // Not usable by paladins
        ANTI_HUMAN = 22, // Not usable by humans
        ANTI_ICER = 23, // Not usable by dwarves
        ANTI_SAIYAN = 24, // Not usable by elves
        ANTI_KONATSU = 25, // Not usable by gnomes
        UNIQUE_SAVE = 26, // unique object save
        BROKEN = 27, // Item is broken hands
        UNBREAKABLE = 28, // Item is unbreakable
        ANTI_MONK = 29, // Not usable by monks
        ANTI_BARBARIAN = 30, // Not usable by barbarians
        ANTI_SORCERER = 31, // Not usable by sorcerers
        DOUBLE = 32, // Double weapon
        ONLY_WIZARD = 33, // Only usable by mages
        ONLY_CLERIC = 34, // Only usable by clerics
        ONLY_ROGUE = 35, // Only usable by thieves
        ONLY_FIGHTER = 36, // Only usable by warriors
        ONLY_DRUID = 37, // Only usable by druids
        ONLY_BARD = 38, // Only usable by bards
        ONLY_RANGER = 39, // Only usable by rangers
        ONLY_PALADIN = 40, // Only usable by paladins
        ONLY_HUMAN = 41, // Only usable by humans
        ONLY_ICER = 42, // Only usable by dwarves
        ONLY_SAIYAN = 43, // Only usable by elves
        ONLY_KONATSU = 44, // Only usable by gnomes
        ONLY_MONK = 45, // Only usable by monks
        ONLY_BARBARIAN = 46, // Only usable by barbarians
        ONLY_SORCERER = 47, // Only usable by sorcerers
        ANTI_ARCANE_ARCHER = 48, // Not usable by Aracane Archers
        ANTI_ARCANE_TRICKSTER = 49, // Not usable by Aracane tricksters
        ANTI_ARCHMAGE = 50, // Not usable by Archmages
        ANTI_ASSASSIN = 51, // Not usable by Assassins
        ANTI_BLACKGUARD = 52, // Not usable by Blackguards
        ANTI_DRAGON_DISCIPLE = 53, // Not usable by Dragon disciples
        ANTI_DUELIST = 54, // Not usable by Duelists
        ANTI_DWARVEN_DEFENDER = 55, // Not usable by Dwarven defenders
        ANTI_ELDRITCH_KNIGHT = 56, // Not usable by Eldritch knights
        ANTI_HIEROPHANT = 57, // Not usable by Hierophants
        ANTI_HORIZON_WALKER = 58, // Not usable by Horizon walkers
        ANTI_LOREMASTER = 59, // Not usable by Loremasters
        ANTI_MYSTIC_THEURGE = 60, // Not usable by Mystic theurges
        ANTI_SHADOWDANCER = 61, // Not useable by shadowdancers
        ANTI_THAUMATURGIST = 62, // Not useable by thaumaturgists
        BSCOUTER = 63,
        MSCOUTER = 64,
        ASCOUTER = 65,
        USCOUTER = 66,
        WEAPLVL1 = 67,
        WEAPLVL2 = 68,
        WEAPLVL3 = 69,
        WEAPLVL4 = 70,
        WEAPLVL5 = 71,
        CBOARD = 72,
        FORGED = 73,
        SHEATH = 74,
        ONLY_JINTO = 75,
        BURIED = 76,
        SLOT1 = 77,
        SLOT2 = 78,
        TOKEN = 79,
        SLOT_ONE = 80,
        SLOTS_FILLED = 81,
        RESTRING = 82,
        CUSTOM = 83,
        PROTECTED = 84,
        NORANDOM = 85,
        THROW = 86,
        HOT = 87,
        PURGE = 88,
        ICE = 89,
        DUPLICATE = 90,
        MATURE = 91,
        CARDCASE = 92,
        NOPICKUP = 93,
        NOSTEAL = 94,
        NOTAKE = 95,
        PAPER = 96,
        MONEY = 97
    };

    constexpr std::size_t numItemFlags = 98;

    class ItemFlag : public BaseFlag {
    public:
        virtual void onLoad(entt::entity room) {};
        virtual void onSet(entt::entity room) {};
        virtual void onClear(entt::entity room) {};
    };

    class SimpleItemFlag : public ItemFlag {
    public:
        SimpleItemFlag(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    extern const std::vector<std::shared_ptr<ItemFlag>> itemFlags;

    std::optional<IFlagId> getItemFlagId(const std::string& name, bool exact = true);

}
