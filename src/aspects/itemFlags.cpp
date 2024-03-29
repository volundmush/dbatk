#include "dbatk/aspects/itemFlags.h"

namespace dbat::iflags {

    class SimpleItemFlag : public ItemFlag {
    public:
        SimpleItemFlag(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    const std::vector<std::shared_ptr<ItemFlag>> itemFlags = {
            std::make_shared<SimpleItemFlag>(GLOW, "GLOW"),
            std::make_shared<SimpleItemFlag>(HUM, "HUM"),
            std::make_shared<SimpleItemFlag>(NORENT, "NORENT"),
            std::make_shared<SimpleItemFlag>(NODONATE, "NODONATE"),
            std::make_shared<SimpleItemFlag>(NOINVIS, "NOINVIS"),
            std::make_shared<SimpleItemFlag>(INVISIBLE, "INVISIBLE"),
            std::make_shared<SimpleItemFlag>(MAGIC, "MAGIC"),
            std::make_shared<SimpleItemFlag>(NODROP, "NODROP"),
            std::make_shared<SimpleItemFlag>(BLESS, "BLESS"),
            std::make_shared<SimpleItemFlag>(ANTI_GOOD, "ANTI_GOOD"),
            std::make_shared<SimpleItemFlag>(ANTI_EVIL, "ANTI_EVIL"),
            std::make_shared<SimpleItemFlag>(ANTI_NEUTRAL, "ANTI_NEUTRAL"),
            std::make_shared<SimpleItemFlag>(ANTI_WIZARD, "ANTI_WIZARD"),
            std::make_shared<SimpleItemFlag>(ANTI_CLERIC, "ANTI_CLERIC"),
            std::make_shared<SimpleItemFlag>(ANTI_ROGUE, "ANTI_ROGUE"),
            std::make_shared<SimpleItemFlag>(ANTI_FIGHTER, "ANTI_FIGHTER"),
            std::make_shared<SimpleItemFlag>(NOSELL, "NOSELL"),
            std::make_shared<SimpleItemFlag>(ANTI_DRUID, "ANTI_DRUID"),
            std::make_shared<SimpleItemFlag>(TWO_HAND, "TWO_HAND"),
            std::make_shared<SimpleItemFlag>(ANTI_BARD, "ANTI_BARD"),
            std::make_shared<SimpleItemFlag>(ANTI_RANGER, "ANTI_RANGER"),
            std::make_shared<SimpleItemFlag>(ANTI_PALADIN, "ANTI_PALADIN"),
            std::make_shared<SimpleItemFlag>(ANTI_HUMAN, "ANTI_HUMAN"),
            std::make_shared<SimpleItemFlag>(ANTI_ICER, "ANTI_ICER"),
            std::make_shared<SimpleItemFlag>(ANTI_SAIYAN, "ANTI_SAIYAN"),
            std::make_shared<SimpleItemFlag>(ANTI_KONATSU, "ANTI_KONATSU"),
            std::make_shared<SimpleItemFlag>(UNIQUE_SAVE, "UNIQUE_SAVE"),
            std::make_shared<SimpleItemFlag>(BROKEN, "BROKEN"),
            std::make_shared<SimpleItemFlag>(UNBREAKABLE, "UNBREAKABLE"),
            std::make_shared<SimpleItemFlag>(ANTI_MONK, "ANTI_MONK"),
            std::make_shared<SimpleItemFlag>(ANTI_BARBARIAN, "ANTI_BARBARIAN"),
            std::make_shared<SimpleItemFlag>(ANTI_SORCERER, "ANTI_SORCERER"),
            std::make_shared<SimpleItemFlag>(DOUBLE, "DOUBLE"),
            std::make_shared<SimpleItemFlag>(ONLY_WIZARD, "ONLY_WIZARD"),
            std::make_shared<SimpleItemFlag>(ONLY_CLERIC, "ONLY_CLERIC"),
            std::make_shared<SimpleItemFlag>(ONLY_ROGUE, "ONLY_ROGUE"),
            std::make_shared<SimpleItemFlag>(ONLY_FIGHTER, "ONLY_FIGHTER"),
            std::make_shared<SimpleItemFlag>(ONLY_DRUID, "ONLY_DRUID"),
            std::make_shared<SimpleItemFlag>(ONLY_BARD, "ONLY_BARD"),
            std::make_shared<SimpleItemFlag>(ONLY_RANGER, "ONLY_RANGER"),
            std::make_shared<SimpleItemFlag>(ONLY_PALADIN, "ONLY_PALADIN"),
            std::make_shared<SimpleItemFlag>(ONLY_HUMAN, "ONLY_HUMAN"),
            std::make_shared<SimpleItemFlag>(ONLY_ICER, "ONLY_ICER"),
            std::make_shared<SimpleItemFlag>(ONLY_SAIYAN, "ONLY_SAIYAN"),
            std::make_shared<SimpleItemFlag>(ONLY_KONATSU, "ONLY_KONATSU"),
            std::make_shared<SimpleItemFlag>(ONLY_MONK, "ONLY_MONK"),
            std::make_shared<SimpleItemFlag>(ONLY_BARBARIAN, "ONLY_BARBARIAN"),
            std::make_shared<SimpleItemFlag>(ONLY_SORCERER, "ONLY_SORCERER"),
            std::make_shared<SimpleItemFlag>(ANTI_ARCANE_ARCHER, "ANTI_ARCANE_ARCHER"),
            std::make_shared<SimpleItemFlag>(ANTI_ARCANE_TRICKSTER, "ANTI_ARCANE_TRICKSTER"),
            std::make_shared<SimpleItemFlag>(ANTI_ARCHMAGE, "ANTI_ARCHMAGE"),
            std::make_shared<SimpleItemFlag>(ANTI_ASSASSIN, "ANTI_ASSASSIN"),
            std::make_shared<SimpleItemFlag>(ANTI_BLACKGUARD, "ANTI_BLACKGUARD"),
            std::make_shared<SimpleItemFlag>(ANTI_DRAGON_DISCIPLE, "ANTI_DRAGON_DISCIPLE"),
            std::make_shared<SimpleItemFlag>(ANTI_DUELIST, "ANTI_DUELIST"),
            std::make_shared<SimpleItemFlag>(ANTI_DWARVEN_DEFENDER, "ANTI_DWARVEN_DEFENDER"),
            std::make_shared<SimpleItemFlag>(ANTI_ELDRITCH_KNIGHT, "ANTI_ELDRITCH_KNIGHT"),
            std::make_shared<SimpleItemFlag>(ANTI_HIEROPHANT, "ANTI_HIEROPHANT"),
            std::make_shared<SimpleItemFlag>(ANTI_HORIZON_WALKER, "ANTI_HORIZON_WALKER"),
            std::make_shared<SimpleItemFlag>(ANTI_LOREMASTER, "ANTI_LOREMASTER"),
            std::make_shared<SimpleItemFlag>(ANTI_MYSTIC_THEURGE, "ANTI_MYSTIC_THEURGE"),
            std::make_shared<SimpleItemFlag>(ANTI_SHADOWDANCER, "ANTI_SHADOWDANCER"),
            std::make_shared<SimpleItemFlag>(ANTI_THAUMATURGIST, "ANTI_THAUMATURGIST"),
            std::make_shared<SimpleItemFlag>(BSCOUTER, "BSCOUTER"),
            std::make_shared<SimpleItemFlag>(MSCOUTER, "MSCOUTER"),
            std::make_shared<SimpleItemFlag>(ASCOUTER, "ASCOUTER"),
            std::make_shared<SimpleItemFlag>(USCOUTER, "USCOUTER"),
            std::make_shared<SimpleItemFlag>(WEAPLVL1, "WEAPLVL1"),
            std::make_shared<SimpleItemFlag>(WEAPLVL2, "WEAPLVL2"),
            std::make_shared<SimpleItemFlag>(WEAPLVL3, "WEAPLVL3"),
            std::make_shared<SimpleItemFlag>(WEAPLVL4, "WEAPLVL4"),
            std::make_shared<SimpleItemFlag>(WEAPLVL5, "WEAPLVL5"),
            std::make_shared<SimpleItemFlag>(CBOARD, "CBOARD"),
            std::make_shared<SimpleItemFlag>(FORGED, "FORGED"),
            std::make_shared<SimpleItemFlag>(SHEATH, "SHEATH"),
            std::make_shared<SimpleItemFlag>(ONLY_JINTO, "ONLY_JINTO"),
            std::make_shared<SimpleItemFlag>(BURIED, "BURIED"),
            std::make_shared<SimpleItemFlag>(SLOT1, "SLOT1"),
            std::make_shared<SimpleItemFlag>(SLOT2, "SLOT2"),
            std::make_shared<SimpleItemFlag>(TOKEN, "TOKEN"),
            std::make_shared<SimpleItemFlag>(SLOT_ONE, "SLOT_ONE"),
            std::make_shared<SimpleItemFlag>(SLOTS_FILLED, "SLOTS_FILLED"),
            std::make_shared<SimpleItemFlag>(RESTRING, "RESTRING"),
            std::make_shared<SimpleItemFlag>(CUSTOM, "CUSTOM"),
            std::make_shared<SimpleItemFlag>(PROTECTED, "PROTECTED"),
            std::make_shared<SimpleItemFlag>(NORANDOM, "NORANDOM"),
            std::make_shared<SimpleItemFlag>(THROW, "THROW"),
            std::make_shared<SimpleItemFlag>(HOT, "HOT"),
            std::make_shared<SimpleItemFlag>(PURGE, "PURGE"),
            std::make_shared<SimpleItemFlag>(ICE, "ICE"),
            std::make_shared<SimpleItemFlag>(DUPLICATE, "DUPLICATE"),
            std::make_shared<SimpleItemFlag>(MATURE, "MATURE"),
            std::make_shared<SimpleItemFlag>(CARDCASE, "CARDCASE"),
            std::make_shared<SimpleItemFlag>(NOPICKUP, "NOPICKUP"),
            std::make_shared<SimpleItemFlag>(NOSTEAL, "NOSTEAL"),
            std::make_shared<SimpleItemFlag>(NOTAKE, "NOTAKE"),
            std::make_shared<SimpleItemFlag>(PAPER, "PAPER"),
            std::make_shared<SimpleItemFlag>(MONEY, "MONEY")
    };

    std::optional<IFlagId> getItemFlagId(const std::string& name, bool exact) {
        auto found = partialMatch(name, itemFlags.begin(), itemFlags.end(), exact, [](const auto& flag) { return flag->getName(); });
        if(found != itemFlags.end()) {
            return static_cast<IFlagId>((*found)->getId());
        }
        return std::nullopt;
    }
}