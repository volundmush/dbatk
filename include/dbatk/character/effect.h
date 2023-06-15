#pragma once
#include "dbatk/base.h"

namespace dbat::effect {
    enum EffectID : std::size_t {
        NONE = 0,          // No effect
        STR = 1,           // Apply to strength
        DEX = 2,           // Apply to dexterity
        INT = 3,           // Apply to intelligence
        WIS = 4,           // Apply to wisdom
        CON = 5,           // Apply to constitution
        CHA = 6,           // Apply to charisma
        CLASS = 7,         // Reserved
        LEVEL = 8,         // Reserved
        AGE = 9,           // Apply to age
        CHAR_WEIGHT = 10,  // Apply to weight
        CHAR_HEIGHT = 11,  // Apply to height
        MANA = 12,         // Apply to max mana
        HIT = 13,          // Apply to max hit points
        MOVE = 14,         // Apply to max move points
        GOLD = 15,         // Reserved
        EXP = 16,          // Reserved
        AC = 17,           // Apply to Armor Class
        ACCURACY = 18,     // Apply to accuracy
        DAMAGE = 19,       // Apply to damage
        REGEN = 20,        // Regen Rate Buffed
        TRAIN = 21,        // Skill training rate buffed
        LIFEMAX = 22,      // Life Force max buffed
        UNUSED3 = 23,      // Unused
        UNUSED4 = 24,      // Unused
        RACE = 25,         // Apply to race
        TURN_LEVEL = 26,   // Apply to turn undead
        SPELL_LVL_0 = 27,  // Apply to spell cast per day
        SPELL_LVL_1 = 28,  // Apply to spell cast per day
        SPELL_LVL_2 = 29,  // Apply to spell cast per day
        SPELL_LVL_3 = 30,  // Apply to spell cast per day
        SPELL_LVL_4 = 31,  // Apply to spell cast per day
        SPELL_LVL_5 = 32,  // Apply to spell cast per day
        SPELL_LVL_6 = 33,  // Apply to spell cast per day
        SPELL_LVL_7 = 34,  // Apply to spell cast per day
        SPELL_LVL_8 = 35,  // Apply to spell cast per day
        SPELL_LVL_9 = 36,  // Apply to spell cast per day
        KI = 37,           // Apply to max ki
        FORTITUDE = 38,    // Apply to fortitue save
        REFLEX = 39,       // Apply to reflex save
        WILL = 40,         // Apply to will save
        SKILL = 41,        // Apply to a specific skill
        FEAT = 42,         // Apply to a specific feat
        ALLSAVES = 43,     // Apply to all 3 save types
        RESISTANCE = 44,   // Apply to resistance
        ALL_STATS = 45,    // Apply to all attributes
    };
}