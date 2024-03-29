#include "dbatk/aspects/effect.h"

namespace dbat::effect {

    class SimpleEffect : public Effect {
    public:
        SimpleEffect(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    const std::vector<std::shared_ptr<Effect>> effectTypes = {
            std::make_shared<SimpleEffect>(NONE, "NONE"),
            std::make_shared<SimpleEffect>(STR, "STR"),
            std::make_shared<SimpleEffect>(DEX, "DEX"),
            std::make_shared<SimpleEffect>(INT, "INT"),
            std::make_shared<SimpleEffect>(WIS, "WIS"),
            std::make_shared<SimpleEffect>(CON, "CON"),
            std::make_shared<SimpleEffect>(CHA, "CHA"),
            std::make_shared<SimpleEffect>(CLASS, "CLASS"),
            std::make_shared<SimpleEffect>(LEVEL, "LEVEL"),
            std::make_shared<SimpleEffect>(AGE, "AGE"),
            std::make_shared<SimpleEffect>(CHAR_WEIGHT, "CHAR_WEIGHT"),
            std::make_shared<SimpleEffect>(CHAR_HEIGHT, "CHAR_HEIGHT"),
            std::make_shared<SimpleEffect>(MANA, "MANA"),
            std::make_shared<SimpleEffect>(HIT, "HIT"),
            std::make_shared<SimpleEffect>(MOVE, "MOVE"),
            std::make_shared<SimpleEffect>(GOLD, "GOLD"),
            std::make_shared<SimpleEffect>(EXP, "EXP"),
            std::make_shared<SimpleEffect>(AC, "AC"),
            std::make_shared<SimpleEffect>(ACCURACY, "ACCURACY"),
            std::make_shared<SimpleEffect>(DAMAGE, "DAMAGE"),
            std::make_shared<SimpleEffect>(REGEN, "REGEN"),
            std::make_shared<SimpleEffect>(TRAIN, "TRAIN"),
            std::make_shared<SimpleEffect>(LIFEMAX, "LIFEMAX"),
            std::make_shared<SimpleEffect>(UNUSED3, "UNUSED3"),
            std::make_shared<SimpleEffect>(UNUSED4, "UNUSED4"),
            std::make_shared<SimpleEffect>(RACE, "RACE"),
            std::make_shared<SimpleEffect>(TURN_LEVEL, "TURN_LEVEL"),
            std::make_shared<SimpleEffect>(SPELL_LVL_0, "SPELL_LVL_0"),
            std::make_shared<SimpleEffect>(SPELL_LVL_1, "SPELL_LVL_1"),
            std::make_shared<SimpleEffect>(SPELL_LVL_2, "SPELL_LVL_2"),
            std::make_shared<SimpleEffect>(SPELL_LVL_3, "SPELL_LVL_3"),
            std::make_shared<SimpleEffect>(SPELL_LVL_4, "SPELL_LVL_4"),
            std::make_shared<SimpleEffect>(SPELL_LVL_5, "SPELL_LVL_5"),
            std::make_shared<SimpleEffect>(SPELL_LVL_6, "SPELL_LVL_6"),
            std::make_shared<SimpleEffect>(SPELL_LVL_7, "SPELL_LVL_7"),
            std::make_shared<SimpleEffect>(SPELL_LVL_8, "SPELL_LVL_8"),
            std::make_shared<SimpleEffect>(SPELL_LVL_9, "SPELL_LVL_9"),
            std::make_shared<SimpleEffect>(KI, "KI"),
            std::make_shared<SimpleEffect>(FORTITUDE, "FORTITUDE"),
            std::make_shared<SimpleEffect>(REFLEX, "REFLEX"),
            std::make_shared<SimpleEffect>(WILL, "WILL"),
            std::make_shared<SimpleEffect>(SKILL, "SKILL"),
            std::make_shared<SimpleEffect>(FEAT, "FEAT"),
            std::make_shared<SimpleEffect>(ALLSAVES, "ALLSAVES"),
            std::make_shared<SimpleEffect>(RESISTANCE, "RESISTANCE"),
            std::make_shared<SimpleEffect>(ALL_STATS, "ALL_STATS"),
    };
}