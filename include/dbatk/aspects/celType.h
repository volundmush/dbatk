#pragma once

#include "dbatk/aspects/flags.h"

namespace dbat::celtype {
    enum CelestialTypeId : uint8_t {
        PLANET = 0,
        MOON = 1,
        ASTEROID = 2,
        STAR = 3,
        STATION = 4
    };

    constexpr std::size_t numCelestialTypes = 5;

    class CelType : public BaseFlag {
    public:
        virtual void onLoad(entt::entity room) {};
        virtual void onSet(entt::entity room) {};
        virtual void onClear(entt::entity room) {};
    };

    class SimpleCelType : public CelType {
    public:
        SimpleCelType(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    std::optional<CelestialTypeId> getCelTypeId(const std::string& name, bool exact);
    extern const std::vector<std::shared_ptr<CelType>> celTypes;
}