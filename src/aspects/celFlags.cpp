#include "dbatk/aspects/celFlags.h"

namespace dbat::celflags {

    class SimpleCelestialFlag : public CelestialFlag {
    public:
        SimpleCelestialFlag(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    const std::vector<std::shared_ptr<CelestialFlag>> celestialFlags = {
            std::make_shared<SimpleCelestialFlag>(ETHERSTREAM, "ETHERSTREAM"),
            std::make_shared<SimpleCelestialFlag>(HASMOON, "HASMOON"),
    };
}