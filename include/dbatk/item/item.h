#pragma once
#include "dbatk/object/object.h"
#include "dbatk/item/wear.h"
#include "dbatk/item/flags.h"
#include "dbatk/item/liquid.h"

namespace dbat {
    struct LightComponent {
        double timeLeft{0.0};
        double hoursLeft{0.0};
    };

    struct WeaponComponent {
        int skill;    // e.g., VAL_WEAPON_SKILL
        int damDice;  // e.g., VAL_WEAPON_DAMDICE
        int damSize;  // e.g., VAL_WEAPON_DAMSIZE
        int damType;  // e.g., VAL_WEAPON_DAMTYPE
    };

    struct WornComponent {
        // The positions that the item can be worn on.
        std::bitset<wear::NUM_WEAR_FLAGS> wearFlags;
        // Where the item is currently worn, if anywhere.
        std::optional<int> wornOn;

    };

    struct FoodComponent {

    };

    struct ContainerComponent {
        uint16_t capacity{0};
        std::bitset<4> containerFlags;
        std::optional<std::size_t> key;
    };

    struct DrinkComponent {
        int16_t capacity{0};
        int16_t current{0};
        bool poisoned{false};
        liq::LiquidID liquid{liq::WATER};
    };

    class Item : public Object {
    public:
        using Object::Object;
        ~Item() override = default;
        std::string getTypeName() const override { return "item"; };

        virtual double getDurability() const { return durability; };
        virtual double getMaxDurability() const { return maxDurability; };
        virtual void setDurability(double d) { durability = d; };
        virtual void setMaxDurability(double d) { maxDurability = d; };

        std::unique_ptr<LightComponent> light;
        std::unique_ptr<WeaponComponent> weapon;
        std::unique_ptr<WornComponent> worn;
        std::unique_ptr<FoodComponent> food;
        std::unique_ptr<ContainerComponent> container;
        std::unique_ptr<DrinkComponent> drink;
        Room* portalDestination{nullptr};

    protected:
        double durability{0.0};
        double maxDurability{0.0};
        std::bitset<iflags::NUM_ITEM_FLAGS> itemFlags;
    };



}