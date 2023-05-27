#include "dbatk/Aspect.h"

namespace kaizermud::dbat::aspect {

    kaizermud::game::OpResult DBATAspectSlot::atPostLoad() {
        if (aspect) {
            return {true, std::nullopt};
        }
        auto def = getDefault();
        if(!def.has_value()) return {true, std::nullopt};
        return setAspect(def.value())
    }

    std::string_view DBATAspectSlot::getDefault() const {
        return std::nullopt;
    }

    // Race Stuff
    std::optional<std::string> RaceSlot::getDefault() const {
        return "spirit";
    }

    std::optional<std::string> PositionSlot::getDefault() const {
        return "standing";
    }

    std::optional<std::string> SexSlot::getDefault() const {
        return "neuter";
    }

}