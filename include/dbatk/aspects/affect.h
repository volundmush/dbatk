#pragma once
#include "dbatk/base.h"

namespace dbat::affect {
    enum AffectId : uint8_t {
        DONTUSE = 0,           // DON'T USE!
        BLIND = 1,             // (R) Char is blind
        INVISIBLE = 2,         // Char is invisible
        DETECT_ALIGN = 3,      // Char is sensitive to align
        DETECT_INVIS = 4,      // Char can see invis chars
        DETECT_MAGIC = 5,      // Char is sensitive to magic
        SENSE_LIFE = 6,        // Char can sense hidden life
        WATERWALK = 7,         // Char can walk on water
        SANCTUARY = 8,         // Char protected by sanct.
        GROUP = 9,             // (R) Char is grouped
        CURSE = 10,            // Char is cursed
        INFRAVISION = 11,      // Char can see in dark
        POISON = 12,           // (R) Char is poisoned
        WEAKENED_STATE = 13,   // Char protected from evil
        PROTECT_GOOD = 14,     // Char protected from good
        SLEEP = 15,            // (R) Char magically asleep
        NOTRACK = 16,          // Char can't be tracked
        UNDEAD = 17,           // Char is undead
        PARALYZE = 18,         // Char is paralized
        SNEAK = 19,            // Char can move quietly
        HIDE = 20,             // Char is hidden
        UNUSED20 = 21,         // Room for future expansion
        CHARM = 22,            // Char is charmed
        FLYING = 23,           // Char is flying
        WATERBREATH = 24,      // Char can breath non O2
        ANGELIC = 25,          // Char is an angelic being
        ETHEREAL = 26,         // Char is ethereal
        MAGICONLY = 27,        // Char only hurt by magic
        NEXTPARTIAL = 28,      // Next action cannot be full
        NEXTNOACTION = 29,     // Next action cannot attack (took full action between rounds)
        STUNNED = 30,          // Char is stunned
        TAMED = 31,            // Char has been tamed
        CDEATH = 32,           // Char is undergoing creeping death
        SPIRIT = 33,           // Char has no body
        STONESKIN = 34,        // Char has temporary DR
        SUMMONED = 35,         // Char is summoned (i.e. transient)
        CELESTIAL = 36,        // Char is celestial
        FIENDISH = 37,         // Char is fiendish
        FIRE_SHIELD = 38,      // Char has fire shield
        LOW_LIGHT = 39,        // Char has low light eyes
        ZANZOKEN = 40,         // Char is ready to zanzoken
        KNOCKED = 41,          // Char is knocked OUT!
        MIGHT = 42,            // Strength +3
        FLEX = 43,             // Agility +3
        GENIUS = 44,           // Intelligence +3
        BLESS = 45,            // Bless for better regen
        BURNT = 46,            // Disintergrated corpse
        BURNED = 47,           // Burned by honoo or similar skill
        MBREAK = 48,           // Can't charge while flagged
        HASS = 49,             // Does double punch damage
        FUTURE = 50,           // Future Sight
        PARA = 51,             // Real Paralyze
        INFUSE = 52,           // Ki infused attacks
        ENLIGHTEN = 53,        // Enlighten
        FROZEN = 54,           // They got frozededed
        FIRESHIELD = 55,       // They have a blazing personality
        ENSNARED = 56,         // They have silk ensnaring their arms!
        HAYASA = 57,           // They are speedy!
        PURSUIT = 58,          // Being followed
        WITHER = 59,           // Their body is withered
        HYDROZAP = 60,         // Custom Skill Kanso Suru
        POSITION = 61,         // Better combat position
        SHOCKED = 62,          // Psychic Shock
        METAMORPH = 63,        // Metamorphisis, Demon's Ripoff Custom Skill
        HEALGLOW = 64,         // Healing Glow
        EARMOR = 65,           // Ethereal Armor
        ECHAINS = 66,          // Ethereal Chains
        WUNJO = 67,            // Wunjo rune
        POTENT = 68,           // Purisaz rune
        ASHED = 69,            // Leaves ash
        PUKED = 70,
        LIQUEFIED = 71,
        SHELL = 72,
        IMMUNITY = 73,
        SPIRITCONTROL = 74
    };

    constexpr std::size_t countAffectFlags = 75;
}