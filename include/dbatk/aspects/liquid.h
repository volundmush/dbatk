#pragma once
#include "dbatk/aspects/flags.h"

namespace dbat::liq {
    enum LiquidId : uint8_t {
        WATER = 0,
        BEER = 1,
        WINE = 2,
        ALE = 3,
        DARKALE = 4,
        WHISKY = 5,
        LEMONADE = 6,
        FIREBREATH = 7,
        LOCALSPECIALTY = 8,
        SLIME = 9,
        MILK = 10,
        TEA = 11,
        COFFEE = 12,
        BLOOD = 13,
        SALTWATER = 14,
        CLEARWATER = 15
    };

    constexpr std::size_t numLiquids = 16;

    class Liquid : public BaseFlag {
    public:
        virtual void onLoad(entt::entity room) {};
        virtual void onSet(entt::entity room) {};
        virtual void onClear(entt::entity room) {};
    };

    class SimpleLiquid : public Liquid {
    public:
        SimpleLiquid(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    extern const std::vector<std::shared_ptr<Liquid>> liquids;
}