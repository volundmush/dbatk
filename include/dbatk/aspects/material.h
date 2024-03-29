#pragma once
#include "dbatk/base.h"

namespace dbat::mat {
    enum MaterialId : uint8_t {
        BONE = 0,
        CERAMIC = 1,
        COPPER = 2,
        DIAMOND = 3,
        GOLD = 4,
        IRON = 5,
        LEATHER = 6,
        MITHRIL = 7,
        OBSIDIAN = 8,
        STEEL = 9,
        STONE = 10,
        SILVER = 11,
        WOOD = 12,
        GLASS = 13,
        ORGANIC = 14,
        CURRENCY = 15,
        PAPER = 16,
        COTTON = 17,
        SATIN = 18,
        SILK = 19,
        BURLAP = 20,
        VELVET = 21,
        PLATINUM = 22,
        ADAMANTINE = 23,
        WOOL = 24,
        ONYX = 25,
        IVORY = 26,
        BRASS = 27,
        MARBLE = 28,
        BRONZE = 29,
        KACHIN = 30,
        RUBY = 31,
        SAPPHIRE = 32,
        EMERALD = 33,
        GEMSTONE = 34,
        GRANITE = 35,
        ENERGY = 36,
        HEMP = 37,
        CRYSTAL = 38,
        EARTH = 39,
        LIQUID = 40,
        CLOTH = 41,
        METAL = 42,
        WAX = 43,
        OTHER = 44,
        FOOD = 45,
        OIL = 46
    };

    constexpr std::size_t numMaterials = 47;

    class Material : public BaseFlag {
    public:
        virtual void onLoad(entt::entity room) {};
        virtual void onSet(entt::entity room) {};
        virtual void onClear(entt::entity room) {};
    };

    extern const std::vector<std::shared_ptr<Material>> materials;
}