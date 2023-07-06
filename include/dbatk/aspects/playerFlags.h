#pragma once
#include "dbatk/base.h"

namespace dbat::pflags {
    enum PFlagId : std::size_t {
        KILLER = 0, /* Player is a player-killer */
        THIEF = 1, /* Player is a player-thief */
        FROZEN = 2, /* Player is frozen */
        DONTSET = 3, /* Don't EVER set (ISNPC bit) */
        WRITING = 4, /* Player writing (board/mail/olc) */
        MAILING = 5, /* Player is writing mail */
        CRASH = 6, /* Player needs to be crash-saved */
        SITEOK = 7, /* Player has been site-cleared */
        NOSHOUT = 8, /* Player not allowed to shout/goss */
        NOTITLE = 9, /* Player not allowed to set title */
        DELETED = 10, /* Player deleted - space reusable */
        LOADROOM = 11, /* Player uses nonstandard loadroom */
        NOWIZLIST = 12, /* Player shouldn't be on wizlist */
        NODELETE = 13, /* Player shouldn't be deleted */
        INVSTART = 14, /* Player should enter game wizinvis */
        CRYO = 15, /* Player is cryo-saved (purge prog) */
        NOTDEADYET = 16, /* (R) Player being extracted. */
        AGEMID_G = 17, /* Player has had pos of middle age */
        AGEMID_B = 18, /* Player has had neg of middle age */
        AGEOLD_G = 19, /* Player has had pos of old age */
        AGEOLD_B = 20, /* Player has had neg of old age */
        AGEVEN_G = 21, /* Player has had pos of venerable age */
        AGEVEN_B = 22, /* Player has had neg of venerable age */
        OLDAGE = 23, /* Player is dead of old age */
        RARM = 24, /* Player has a right arm */
        LARM = 25, /* Player has a left arm */
        RLEG = 26, /* Player has a right leg */
        LLEG = 27, /* Player has a left leg */
        HEAD = 28, /* Player has a head */
        STAIL = 29, /* Player has a saiyan tail */
        TAIL = 30, /* Player has a non-saiyan tail */
        PILOTING = 31, /* Player is sitting in the pilots chair */
        SKILLP = 32, /* Player made a good choice in CC */
        SPAR = 33, /* Player is in a spar stance */
        CHARGE = 34, /* Player is charging */
        TRANS1 = 35, /* Transformation 1 */
        TRANS2 = 36, /* Transformation 2 */
        TRANS3 = 37, /* Transformation 3 */
        TRANS4 = 38, /* Transformation 4 */
        TRANS5 = 39, /* Transformation 5 */
        TRANS6 = 40, /* Transformation 6 */
        ABSORB = 41, /* Absorb model */
        REPAIR = 42, /* Repair model */
        SENSEM = 43, /* Sense-Powersense model */
        POWERUP = 44, /* Powering Up */
        KNOCKED = 45, /* Knocked OUT */
        CRARM = 46, /* Cybernetic Right Arm */
        CLARM = 47, /* Cybernetic Left Arm */
        CRLEG = 48, /* Cybernetic Right Leg */
        CLLEG = 49, /* Cybernetic Left Leg */
        FPSSJ = 50, /* Full Power Super Saiyan */
        IMMORTAL = 51, /* The player is immortal */
        EYEC = 52, /* The player has their eyes closed */
        DISGUISED = 53, /* The player is disguised */
        BANDAGED = 54, /* The player has been bandaged */
        PR = 55, /* Has had their potential released */
        HEALT = 56, /* Is inside a healing tank */
        FURY = 57, /* Is in fury mode */
        POSE = 58, /* Ginyu Pose Effect */
        OOZARU = 59,
        ABSORBED = 60,
        MULTP = 61,
        PDEATH = 62,
        THANDW = 63,
        SELFD = 64,
        SELFD2 = 65,
        SPIRAL = 66,
        BIOGR = 67,
        LSSJ = 68,
        REPLEARN = 69,
        FORGET = 70,
        TRANSMISSION = 71,
        FISHING = 72,
        GOOP = 73,
        MULTIHIT = 74,
        AURALIGHT = 75,
        RDISPLAY = 76,
        STOLEN = 77,
        TAILHIDE = 78, /* Hides tail for S & HB */
        NOGROW = 79, /* Halt Growth for S & HB */
    };

    constexpr std::size_t countPlayerFlags = 80;

    class PlayerFlag : public BaseFlag {
    public:
        virtual void onLoad(entt::entity ent) {};
        virtual void onSet(entt::entity ent) {};
        virtual void onClear(entt::entity ent) {};
    };

    extern const std::vector<std::shared_ptr<PlayerFlag>> playerFlags;

    std::optional<PFlagId> getPlayerFlagId(const std::string& name, bool exact = true);

}