#include "dbatk/aspects/celType.h"

namespace dbat::celtype {
    class SimpleCelType : public CelType {
    public:
        SimpleCelType(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    const std::vector<std::shared_ptr<CelType>> celTypes = {
            std::make_shared<SimpleCelType>(PLANET, "PLANET"),
            std::make_shared<SimpleCelType>(MOON, "MOON"),
            std::make_shared<SimpleCelType>(ASTEROID, "ASTEROID"),
            std::make_shared<SimpleCelType>(STAR, "STAR"),
            std::make_shared<SimpleCelType>(STATION, "STATION")
    };

    std::optional<CelestialTypeId> getCelTypeId(const std::string& name, bool exact) {
        auto found = partialMatch(name, celTypes.begin(), celTypes.end(), exact, [](const auto& flag) { return flag->getName(); });
        if(found != celTypes.end()) {
            return static_cast<CelestialTypeId>((*found)->getId());
        }
        return std::nullopt;
    }
}