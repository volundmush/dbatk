#include "dbatk/aspects/charWear.h"

namespace dbat::cwear {
    const std::vector<std::shared_ptr<CharacterWear>> characterWears = {
            std::make_shared<SimpleCharacterWear>(FINGER, "FINGER"),
            std::make_shared<SimpleCharacterWear>(NECK, "NECK"),
            std::make_shared<SimpleCharacterWear>(BODY, "BODY"),
            std::make_shared<SimpleCharacterWear>(HEAD, "HEAD"),
            std::make_shared<SimpleCharacterWear>(LEGS, "LEGS"),
            std::make_shared<SimpleCharacterWear>(FEET, "FEET"),
            std::make_shared<SimpleCharacterWear>(HANDS, "HANDS"),
            std::make_shared<SimpleCharacterWear>(ARMS, "ARMS"),
            std::make_shared<SimpleCharacterWear>(SHIELD, "SHIELD"),
            std::make_shared<SimpleCharacterWear>(ABOUT, "ABOUT"),
            std::make_shared<SimpleCharacterWear>(WAIST, "WAIST"),
            std::make_shared<SimpleCharacterWear>(WRIST, "WRIST"),
            std::make_shared<SimpleCharacterWear>(WIELD, "WIELD"),
            std::make_shared<SimpleCharacterWear>(HOLD, "HOLD"),
            std::make_shared<SimpleCharacterWear>(PACK, "PACK"),
            std::make_shared<SimpleCharacterWear>(EAR, "EAR"),
            std::make_shared<SimpleCharacterWear>(SHOULDER, "SHOULDER"),
            std::make_shared<SimpleCharacterWear>(EYE, "EYE"),
    };
}