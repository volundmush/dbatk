#pragma once
#include "dbatk/Object.h"

namespace kaizermud::dbat {

    class Character : public Object {
    public:
        using Object::Object;
        [[nodiscard]] std::string_view getMainType() const override;
    };

    class PlayerCharacter : public Character {
    public:
        using Character::Character;
        [[nodiscard]] std::string_view getSubType() const override;
    };

    class NonPlayerCharacter : public Character {
    public:
        using Character::Character;
        [[nodiscard]] std::string_view getSubType() const override;
    };

    extern std::map<int, std::string_view> npcFlags, playerFlags, adminFlags, preferenceFlags;

}