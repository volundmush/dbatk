#pragma once
#include "dbatk/base.h"

namespace dbat::pref {
    enum PrefId : uint8_t {
        BRIEF = 0,       // Room descs won't normally be shown
        COMPACT = 1,     // No extra CRLF pair before prompts
        DEAF = 2,        // Can't hear shouts
        NOTELL = 3,      // Can't receive tells
        DISPHP = 4,      // Display hit points in prompt
        DISPMANA = 5,    // Display mana points in prompt
        DISPMOVE = 6,    // Display move points in prompt
        AUTOEXIT = 7,    // Display exits in a room
        NOHASSLE = 8,    // Aggr mobs won't attack
        QUEST = 9,       // On quest
        SUMMONABLE = 10, // Can be summoned
        NOREPEAT = 11,   // No repetition of comm commands
        HOLYLIGHT = 12,  // Can see in dark
        COLOR = 13,      // Color
        SPARE = 14,      // Used to be second color bit
        NOWIZ = 15,      // Can't hear wizline
        LOG1 = 16,       // On-line System Log (low bit)
        LOG2 = 17,       // On-line System Log (high bit)
        NOAUCT = 18,     // Can't hear auction channel
        NOGOSS = 19,     // Can't hear gossip channel
        NOGRATZ = 20,    // Can't hear grats channel
        ROOMFLAGS = 21,  // Can see room flags (ROOM_x)
        DISPAUTO = 22,   // Show prompt HP, MP, MV when < 30%.
        CLS = 23,        // Clear screen in OasisOLC
        BUILDWALK = 24,  // Build new rooms when walking
        AFK = 25,        // Player is AFK
        AUTOLOOT = 26,   // Loot everything from a corpse
        AUTOGOLD = 27,   // Loot gold from a corpse
        AUTOSPLIT = 28,  // Split gold with group
        FULL_EXIT = 29,  // Shows full autoexit details
        AUTOSAC = 30,    // Sacrifice a corpse
        AUTOMEM = 31,    // Memorize spells
        VIEWORDER = 32,  // If you want to see the newest first
        NOCOMPRESS = 33, // If you want to force MCCP2 off
        AUTOASSIST = 34, // Auto-assist toggle
        DISPKI = 35,     // Display ki points in prompt
        DISPEXP = 36,    // Display exp points in prompt
        DISPTNL = 37,    // Display TNL exp points in prompt
        TEST = 38,       // Sets triggers safety off for imms
        HIDE = 39,       // Hide on who from other mortals
        NMWARN = 40,     // No mail warning
        HINTS = 41,      // Receives hints
        FURY = 42,       // Sees fury meter
        NODEC = 43,
        NOEQSEE = 44,
        NOMUSIC = 45,
        LKEEP = 46,
        DISTIME = 47,    // Part of Prompt Options
        DISGOLD = 48,    // Part of Prompt Options
        DISPRAC = 49,    // Part of Prompt Options
        NOPARRY = 50,
        DISHUTH = 51,    // Part of Prompt Options
        DISPERC = 52,    // Part of Prompt Options
        CARVE = 53,
        ARENAWATCH = 54,
        NOGIVE = 55,
        INSTRUCT = 56,
        GHEALTH = 57,
        IHEALTH = 58,
        ENERGIZE = 59
    };

    constexpr std::size_t numPrfFlags = 60;

    class PreferenceFlag : public BaseFlag {
    public:
        virtual void onLoad(entt::entity ent) {};
        virtual void onSet(entt::entity ent) {};
        virtual void onClear(entt::entity ent) {};
    };



    extern const std::vector<std::shared_ptr<PreferenceFlag>> preferenceFlags;

    std::optional<PrefId> getPreferenceFlagId(const std::string& name, bool exact = true);
}