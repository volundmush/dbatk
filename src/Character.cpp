#include "dbatk/Character.h"

namespace kaizermud::dbat {

    std::map<int, std::string_view> npcFlags, playerFlags, adminFlags, preferenceFlags;

    std::string_view Character::getMainType() const {
        return "character";
    }

    std::string_view PlayerCharacter::getSubType() const {
        return "player";
    }

    std::string_view NonPlayerCharacter::getSubType() const {
        return "npc";
    }

}