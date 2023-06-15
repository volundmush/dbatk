#pragma once
#include "dbatk/object/object.h"
#include "dbatk/celestialbody/type.h"
#include "dbatk/celestialbody/flags.h"

namespace dbat {
    class CelestialBody : public Object {
    public:
        using Object::Object;
        ~CelestialBody() override = default;
        std::string getTypeName() const override { return "celestialbody"; };

        celtype::CelestialTypeID getCelestialTypeID() const { return celestialTypeId; };
        void setCelestialTypeID(celtype::CelestialTypeID id) { celestialTypeId = id; };
        void setCelFlag(std::size_t flag, bool value);
        [[nodiscard]] bool checkCelFlag(std::size_t flag) const;
        [[nodiscard]] double getGravity() override { return gravity; };
        void setGravity(double grav) override { gravity = grav; };
    protected:
        celtype::CelestialTypeID celestialTypeId = celtype::CelestialTypeID::PLANET;
        std::bitset<celflags::NUM_CELESTIAL_FLAGS> celFlags;
        double gravity{1.0};
    };
}