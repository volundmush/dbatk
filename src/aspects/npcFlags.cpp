#include "dbatk/aspects/npcFlags.h"

namespace dbat::nflags {
    class SimpleNPCFlag : public NPCFlag {
    public:
        SimpleNPCFlag(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    const std::vector<std::shared_ptr<NPCFlag>> npcFlags = {
            std::make_shared<SimpleNPCFlag>(SPEC, "SPEC"),
            std::make_shared<SimpleNPCFlag>(SENTINEL, "SENTINEL"),
            std::make_shared<SimpleNPCFlag>(NOSCAVENGER, "NOSCAVENGER"),
            std::make_shared<SimpleNPCFlag>(ISNPC, "ISNPC"),
            std::make_shared<SimpleNPCFlag>(AWARE, "AWARE"),
            std::make_shared<SimpleNPCFlag>(AGGRESSIVE, "AGGRESSIVE"),
            std::make_shared<SimpleNPCFlag>(STAY_ZONE, "STAY_ZONE"),
            std::make_shared<SimpleNPCFlag>(WIMPY, "WIMPY"),
            std::make_shared<SimpleNPCFlag>(AGGR_EVIL, "AGGR_EVIL"),
            std::make_shared<SimpleNPCFlag>(AGGR_GOOD, "AGGR_GOOD"),
            std::make_shared<SimpleNPCFlag>(AGGR_NEUTRAL, "AGGR_NEUTRAL"),
            std::make_shared<SimpleNPCFlag>(MEMORY, "MEMORY"),
            std::make_shared<SimpleNPCFlag>(HELPER, "HELPER"),
            std::make_shared<SimpleNPCFlag>(NOCHARM, "NOCHARM"),
            std::make_shared<SimpleNPCFlag>(NOSUMMON, "NOSUMMON"),
            std::make_shared<SimpleNPCFlag>(NOSLEEP, "NOSLEEP"),
            std::make_shared<SimpleNPCFlag>(AUTOBALANCE, "AUTOBALANCE"),
            std::make_shared<SimpleNPCFlag>(NOBLIND, "NOBLIND"),
            std::make_shared<SimpleNPCFlag>(NOKILL, "NOKILL"),
            std::make_shared<SimpleNPCFlag>(NOTDEADYET, "NOTDEADYET"),
            std::make_shared<SimpleNPCFlag>(MOUNTABLE, "MOUNTABLE"),
            std::make_shared<SimpleNPCFlag>(RARM, "RARM"),
            std::make_shared<SimpleNPCFlag>(LARM, "LARM"),
            std::make_shared<SimpleNPCFlag>(RLEG, "RLEG"),
            std::make_shared<SimpleNPCFlag>(LLEG, "LLEG"),
            std::make_shared<SimpleNPCFlag>(HEAD, "HEAD"),
            std::make_shared<SimpleNPCFlag>(JUSTDESC, "JUSTDESC"),
            std::make_shared<SimpleNPCFlag>(HUSK, "HUSK"),
            std::make_shared<SimpleNPCFlag>(SPAR, "SPAR"),
            std::make_shared<SimpleNPCFlag>(DUMMY, "DUMMY"),
            std::make_shared<SimpleNPCFlag>(ABSORB, "ABSORB"),
            std::make_shared<SimpleNPCFlag>(REPAIR, "REPAIR"),
            std::make_shared<SimpleNPCFlag>(NOPOISON, "NOPOISON"),
            std::make_shared<SimpleNPCFlag>(KNOWKAIO, "KNOWKAIO"),
            std::make_shared<SimpleNPCFlag>(POWERUP, "POWERUP"),
    };
}