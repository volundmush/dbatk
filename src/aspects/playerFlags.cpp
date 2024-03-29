#include "dbatk/aspects/playerFlags.h"

namespace dbat::pflags {

    class SimplePlayerFlag : public PlayerFlag {
    public:
        SimplePlayerFlag(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    const std::vector<std::shared_ptr<PlayerFlag>> playerFlags = {
            std::make_shared<SimplePlayerFlag>(KILLER, "KILLER"),
            std::make_shared<SimplePlayerFlag>(THIEF, "THIEF"),
            std::make_shared<SimplePlayerFlag>(FROZEN, "FROZEN"),
            std::make_shared<SimplePlayerFlag>(DONTSET, "DONTSET"),
            std::make_shared<SimplePlayerFlag>(WRITING, "WRITING"),
            std::make_shared<SimplePlayerFlag>(MAILING, "MAILING"),
            std::make_shared<SimplePlayerFlag>(CRASH, "CRASH"),
            std::make_shared<SimplePlayerFlag>(SITEOK, "SITEOK"),
            std::make_shared<SimplePlayerFlag>(NOSHOUT, "NOSHOUT"),
            std::make_shared<SimplePlayerFlag>(NOTITLE, "NOTITLE"),
            std::make_shared<SimplePlayerFlag>(DELETED, "DELETED"),
            std::make_shared<SimplePlayerFlag>(LOADROOM, "LOADROOM"),
            std::make_shared<SimplePlayerFlag>(NOWIZLIST, "NOWIZLIST"),
            std::make_shared<SimplePlayerFlag>(NODELETE, "NODELETE"),
            std::make_shared<SimplePlayerFlag>(INVSTART, "INVSTART"),
            std::make_shared<SimplePlayerFlag>(CRYO, "CRYO"),
            std::make_shared<SimplePlayerFlag>(NOTDEADYET, "NOTDEADYET"),
            std::make_shared<SimplePlayerFlag>(AGEMID_G, "AGEMID_G"),
            std::make_shared<SimplePlayerFlag>(AGEMID_B, "AGEMID_B"),
            std::make_shared<SimplePlayerFlag>(AGEOLD_G, "AGEOLD_G"),
            std::make_shared<SimplePlayerFlag>(AGEOLD_B, "AGEOLD_B"),
            std::make_shared<SimplePlayerFlag>(AGEVEN_G, "AGEVEN_G"),
            std::make_shared<SimplePlayerFlag>(AGEVEN_B, "AGEVEN_B"),
            std::make_shared<SimplePlayerFlag>(OLDAGE, "OLDAGE"),
            std::make_shared<SimplePlayerFlag>(RARM, "RARM"),
            std::make_shared<SimplePlayerFlag>(LARM, "LARM"),
            std::make_shared<SimplePlayerFlag>(RLEG, "RLEG"),
            std::make_shared<SimplePlayerFlag>(LLEG, "LLEG"),
            std::make_shared<SimplePlayerFlag>(HEAD, "HEAD"),
            std::make_shared<SimplePlayerFlag>(STAIL, "STAIL"),
            std::make_shared<SimplePlayerFlag>(TAIL, "TAIL"),
            std::make_shared<SimplePlayerFlag>(PILOTING, "PILOTING"),
            std::make_shared<SimplePlayerFlag>(SKILLP, "SKILLP"),
            std::make_shared<SimplePlayerFlag>(SPAR, "SPAR"),
            std::make_shared<SimplePlayerFlag>(CHARGE, "CHARGE"),
            std::make_shared<SimplePlayerFlag>(TRANS1, "TRANS1"),
            std::make_shared<SimplePlayerFlag>(TRANS2, "TRANS2"),
            std::make_shared<SimplePlayerFlag>(TRANS3, "TRANS3"),
            std::make_shared<SimplePlayerFlag>(TRANS4, "TRANS4"),
            std::make_shared<SimplePlayerFlag>(TRANS5, "TRANS5"),
            std::make_shared<SimplePlayerFlag>(TRANS6, "TRANS6"),
            std::make_shared<SimplePlayerFlag>(ABSORB, "ABSORB"),
            std::make_shared<SimplePlayerFlag>(REPAIR, "REPAIR"),
            std::make_shared<SimplePlayerFlag>(SENSEM, "SENSEM"),
            std::make_shared<SimplePlayerFlag>(POWERUP, "POWERUP"),
            std::make_shared<SimplePlayerFlag>(KNOCKED, "KNOCKED"),
            std::make_shared<SimplePlayerFlag>(CRARM, "CRARM"),
            std::make_shared<SimplePlayerFlag>(CLARM, "CLARM"),
            std::make_shared<SimplePlayerFlag>(CRLEG, "CRLEG"),
            std::make_shared<SimplePlayerFlag>(CLLEG, "CLLEG"),
            std::make_shared<SimplePlayerFlag>(FPSSJ, "FPSSJ"),
            std::make_shared<SimplePlayerFlag>(IMMORTAL, "IMMORTAL"),
            std::make_shared<SimplePlayerFlag>(EYEC, "EYEC"),
            std::make_shared<SimplePlayerFlag>(DISGUISED, "DISGUISED"),
            std::make_shared<SimplePlayerFlag>(BANDAGED, "BANDAGED"),
            std::make_shared<SimplePlayerFlag>(PR, "PR"),
            std::make_shared<SimplePlayerFlag>(HEALT, "HEALT"),
            std::make_shared<SimplePlayerFlag>(FURY, "FURY"),
            std::make_shared<SimplePlayerFlag>(POSE, "POSE"),
            std::make_shared<SimplePlayerFlag>(OOZARU, "OOZARU"),
            std::make_shared<SimplePlayerFlag>(ABSORBED, "ABSORBED"),
            std::make_shared<SimplePlayerFlag>(MULTP, "MULTP"),
            std::make_shared<SimplePlayerFlag>(PDEATH, "PDEATH"),
            std::make_shared<SimplePlayerFlag>(THANDW, "THANDW"),
            std::make_shared<SimplePlayerFlag>(SELFD, "SELFD"),
            std::make_shared<SimplePlayerFlag>(SELFD2, "SELFD2"),
            std::make_shared<SimplePlayerFlag>(SPIRAL, "SPIRAL"),
            std::make_shared<SimplePlayerFlag>(BIOGR, "BIOGR"),
            std::make_shared<SimplePlayerFlag>(LSSJ, "LSSJ"),
            std::make_shared<SimplePlayerFlag>(REPLEARN, "REPLEARN"),
            std::make_shared<SimplePlayerFlag>(FORGET, "FORGET"),
            std::make_shared<SimplePlayerFlag>(TRANSMISSION, "TRANSMISSION"),
            std::make_shared<SimplePlayerFlag>(FISHING, "FISHING"),
            std::make_shared<SimplePlayerFlag>(GOOP, "GOOP"),
            std::make_shared<SimplePlayerFlag>(MULTIHIT, "MULTIHIT"),
            std::make_shared<SimplePlayerFlag>(AURALIGHT, "AURALIGHT"),
            std::make_shared<SimplePlayerFlag>(RDISPLAY, "RDISPLAY"),
            std::make_shared<SimplePlayerFlag>(STOLEN, "STOLEN"),
            std::make_shared<SimplePlayerFlag>(TAILHIDE, "TAILHIDE"),
            std::make_shared<SimplePlayerFlag>(NOGROW, "NOGROW")
    };

    std::optional<PFlagId> getPlayerFlagId(const std::string& name, bool exact) {
        auto found = partialMatch(name, playerFlags.begin(), playerFlags.end(), exact, [](const auto& flag) { return flag->getName(); });
        if(found != playerFlags.end()) {
            return static_cast<PFlagId>((*found)->getId());
        }
        return std::nullopt;
    }

}