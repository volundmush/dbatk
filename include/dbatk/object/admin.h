#pragma once
#include "dbatk/flags.h"

namespace dbat::aflags {
    // These are special powers afforded to admins and builders for interacting with the game world at
    // a privileged level. They do things like make you invulnerable or able to see into a player's inventory.
    enum AFlagID : std::size_t {
        // Nightvision, bypass all invisibility and hide checks, always pass canDetect() checks,
        // see into containers, see beyond locked doors, see into player inventories, see object IDs,
        // room displays show debug information like flags and IDs, etc.
        MATRIX = 0,
        // Cannot be targeted by any sort of physical interaction such as attacks, spells, or
        // commands like "push" or "pull". Nothing can give you items or target you with skills.
        // You cannot die, cannot take damage, don't suffer negative affects from virtually anything.
        UNDEFEATABLE = 1,
        // You have infinite money.
        MIDAS = 2,
        // You ignore keys and locks on exits and also bypass security checks from triggers and scripts.
        // You don't care about terrain restrictions like space or deep water or sky. Hardcoded effects
        // which would penalize you in some way for being in a room such as by relocating you or harming
        // you are ignored.
        PARKOUR = 3,
        // Nothing can detect you. You are invisible to all senses. This doesn't necessarily mean nothing
        // can affect you, as things which don't care about canDetect() may still affect you. Combine this
        // with UNTOUCHABLE to be completely invulnerable.
        // Ghost is used to lurk around and hide from players.
        GHOST = 4,
        // You ignore limitations like weight, size, and capacity. You can carry anything, wield anything,
        // You can pick up objects that should not be picked up and drop objects in rooms that don't allow
        // such. You can take objects from players and closed/locked containers using "get" and "take".
        ULTRAHAND = 5,
        // You can eat anything, even if that makes no sense, without any penalty.
        KIRBY = 6,
        // All your attacks deal obscene amounts of damage and kill anything in one shot.
        BOOMHEADSHOT = 7,
        // Overhear all whispers and private tells between players.
        PANOPTICON = 8
    };

    constexpr std::size_t NUM_ADM_FLAGS = 8;

}