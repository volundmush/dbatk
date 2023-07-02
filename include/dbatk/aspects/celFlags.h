#include "dbatk/aspects/flags.h"

namespace dbat::celflags {

    enum CelestialFlagId : uint8_t {
        ETHERSTREAM = 0,
        HASMOON = 1,
    };

    class CelestialFlag : public BaseFlag {
    public:
        virtual void onLoad(entt::entity room) {};
        virtual void onSet(entt::entity room) {};
        virtual void onClear(entt::entity room) {};
    };

    class SimpleCelestialFlag : public CelestialFlag {
    public:
        SimpleCelestialFlag(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    constexpr std::size_t numCelestialFlags = 2;

    extern const std::vector<std::shared_ptr<CelestialFlag>> celestialFlags;
}