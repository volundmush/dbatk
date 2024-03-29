#include "dbatk/aspects/material.h"

namespace dbat::mat {
    class SimpleMaterial : public Material {
    public:
        SimpleMaterial(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    const std::vector<std::shared_ptr<Material>> materials = {
            std::make_shared<SimpleMaterial>(BONE, "BONE"),
            std::make_shared<SimpleMaterial>(CERAMIC, "CERAMIC"),
            std::make_shared<SimpleMaterial>(COPPER, "COPPER"),
            std::make_shared<SimpleMaterial>(DIAMOND, "DIAMOND"),
            std::make_shared<SimpleMaterial>(GOLD, "GOLD"),
            std::make_shared<SimpleMaterial>(IRON, "IRON"),
            std::make_shared<SimpleMaterial>(LEATHER, "LEATHER"),
            std::make_shared<SimpleMaterial>(MITHRIL, "MITHRIL"),
            std::make_shared<SimpleMaterial>(OBSIDIAN, "OBSIDIAN"),
            std::make_shared<SimpleMaterial>(STEEL, "STEEL"),
            std::make_shared<SimpleMaterial>(STONE, "STONE"),
            std::make_shared<SimpleMaterial>(SILVER, "SILVER"),
            std::make_shared<SimpleMaterial>(WOOD, "WOOD"),
            std::make_shared<SimpleMaterial>(GLASS, "GLASS"),
            std::make_shared<SimpleMaterial>(ORGANIC, "ORGANIC"),
            std::make_shared<SimpleMaterial>(CURRENCY, "CURRENCY"),
            std::make_shared<SimpleMaterial>(PAPER, "PAPER"),
            std::make_shared<SimpleMaterial>(COTTON, "COTTON"),
            std::make_shared<SimpleMaterial>(SATIN, "SATIN"),
            std::make_shared<SimpleMaterial>(SILK, "SILK"),
            std::make_shared<SimpleMaterial>(BURLAP, "BURLAP"),
            std::make_shared<SimpleMaterial>(VELVET, "VELVET"),
            std::make_shared<SimpleMaterial>(PLATINUM, "PLATINUM"),
            std::make_shared<SimpleMaterial>(ADAMANTINE, "ADAMANTINE"),
            std::make_shared<SimpleMaterial>(WOOL, "WOOL"),
            std::make_shared<SimpleMaterial>(ONYX, "ONYX"),
            std::make_shared<SimpleMaterial>(IVORY, "IVORY"),
            std::make_shared<SimpleMaterial>(BRASS, "BRASS"),
            std::make_shared<SimpleMaterial>(MARBLE, "MARBLE"),
            std::make_shared<SimpleMaterial>(BRONZE, "BRONZE"),
            std::make_shared<SimpleMaterial>(KACHIN, "KACHIN"),
            std::make_shared<SimpleMaterial>(RUBY, "RUBY"),
            std::make_shared<SimpleMaterial>(SAPPHIRE, "SAPPHIRE"),
            std::make_shared<SimpleMaterial>(EMERALD, "EMERALD"),
            std::make_shared<SimpleMaterial>(GEMSTONE, "GEMSTONE"),
            std::make_shared<SimpleMaterial>(GRANITE, "GRANITE"),
            std::make_shared<SimpleMaterial>(ENERGY, "ENERGY"),
            std::make_shared<SimpleMaterial>(HEMP, "HEMP"),
            std::make_shared<SimpleMaterial>(CRYSTAL, "CRYSTAL"),
            std::make_shared<SimpleMaterial>(EARTH, "EARTH"),
            std::make_shared<SimpleMaterial>(LIQUID, "LIQUID"),
            std::make_shared<SimpleMaterial>(CLOTH, "CLOTH"),
            std::make_shared<SimpleMaterial>(METAL, "METAL"),
            std::make_shared<SimpleMaterial>(WAX, "WAX"),
            std::make_shared<SimpleMaterial>(OTHER, "OTHER"),
            std::make_shared<SimpleMaterial>(FOOD, "FOOD"),
            std::make_shared<SimpleMaterial>(OIL, "OIL"),
    };
}