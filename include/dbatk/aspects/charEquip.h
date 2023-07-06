#pragma once

#include "dbatk/aspects/charWear.h"

namespace dbat::cequip {
    enum CEquipID : uint8_t {
        Light = 0,
        RightFinger = 1,
        LeftFinger = 2,
        Neck1 = 3,
        Neck2 = 4,
        Body = 5,
        Head = 6,
        Legs = 7,
        Feet = 8,
        Hands = 9,
        Arms = 10,
        Shield = 11,
        About = 12,
        Waist = 13,
        RightWrist = 14,
        LeftWrist = 15,
        Wield = 16,
        Hold = 17,
        Back = 18,
        Ear1 = 19,
        Ear2 = 20,
        Shoulders = 21,
        Eyes = 22
    };

    constexpr std::size_t numCharEquipSlots = 23;

    class CharacterEquip : public BaseFlag {
    public:
        [[nodiscard]] virtual std::string getBodyPart() const = 0;
        virtual dbat::cwear::WearId getWearId() const = 0;
        virtual OpResult<> canEquip(entt::entity ch, entt::entity item);
        virtual OpResult<> canRemove(entt::entity ch, entt::entity item);
        virtual bool slotIsAvailable(entt::entity ch);
        virtual void equip(entt::entity ch, entt::entity item);
        virtual void remove(entt::entity ch, entt::entity item);
        virtual std::string getEquipMessage() = 0;
        virtual std::string getRemoveMessage() = 0;
        virtual std::size_t getSort() const {return getId() * 10;}
        virtual std::string getDisplaySlot();
    };
    
    extern const std::vector<std::shared_ptr<CharacterEquip>> characterEquips;

}