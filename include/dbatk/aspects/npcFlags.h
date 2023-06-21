#pragma once
#include "dbatk/aspects/flags.h"

namespace dbat::nflags {
    enum NFlagId : uint8_t {
        SPEC = 0,            // Mob has a callable spec-proc
        SENTINEL = 1,        // Mob should not move
        NOSCAVENGER = 2,     // Mob won't pick up items from rooms
        ISNPC = 3,           // (R) Automatically set on all Mobs
        AWARE = 4,           // Mob can't be backstabbed
        AGGRESSIVE = 5,      // Mob auto-attacks everybody nearby
        STAY_ZONE = 6,       // Mob shouldn't wander out of zone
        WIMPY = 7,           // Mob flees if severely injured
        AGGR_EVIL = 8,       // Auto-attack any evil PC's
        AGGR_GOOD = 9,       // Auto-attack any good PC's
        AGGR_NEUTRAL = 10,   // Auto-attack any neutral PC's
        MEMORY = 11,         // remember attackers if attacked
        HELPER = 12,         // attack PCs fighting other NPCs
        NOCHARM = 13,        // Mob can't be charmed
        NOSUMMON = 14,       // Mob can't be summoned
        NOSLEEP = 15,        // Mob can't be slept
        AUTOBALANCE = 16,    // Mob stats autobalance
        NOBLIND = 17,        // Mob can't be blinded
        NOKILL = 18,         // Mob can't be killed
        NOTDEADYET = 19,     // (R) Mob being extracted
        MOUNTABLE = 20,      // Mob is mountable
        RARM = 21,           // Player has a right arm
        LARM = 22,           // Player has a left arm
        RLEG = 23,           // Player has a right leg
        LLEG = 24,           // Player has a left leg
        HEAD = 25,           // Player has a head
        JUSTDESC = 26,       // Mob doesn't use auto desc
        HUSK = 27,           // Is an extracted Husk
        SPAR = 28,           // This is mob sparring
        DUMMY = 29,          // This mob will not fight back
        ABSORB = 30,         // Absorb type android
        REPAIR = 31,         // Repair type android
        NOPOISON = 32,       // No poison
        KNOWKAIO = 33,       // Knows kaioken
        POWERUP = 34,        // Is powering up
    };

    constexpr std::size_t numNpcFlags = 35;
}