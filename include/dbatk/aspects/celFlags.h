#include "dbatk/base.h"

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



    constexpr std::size_t numCelestialFlags = 2;

    extern const std::vector<std::shared_ptr<CelestialFlag>> celestialFlags;
}