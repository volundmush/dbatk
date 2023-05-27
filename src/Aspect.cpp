#include "dbatk/Aspect.h"

namespace kaizermud::dbat::aspect {

    kaizermud::OpResult<> DBATAspectSlot::atPostLoad(entt::entity ent) {
        //if (aspect) {
         //   return {true, std::nullopt};
        //}
        auto def = getDefault();
        if(!def.has_value()) return {true, std::nullopt};
        return setAspect(ent, def.value());
    }

    std::optional<std::string> DBATAspectSlot::getDefault() const {
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

    std::optional<std::string> SenseiSlot::getDefault() const {
        return "commoner";
    }

}