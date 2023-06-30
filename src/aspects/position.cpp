#include "dbatk/aspects/position.h"

namespace dbat::position {

    const std::vector<std::shared_ptr<Position>> positions = {
        std::make_shared<Dead>(),
        std::make_shared<MortallyWounded>(),
        std::make_shared<Incapacitated>(),
        std::make_shared<Stunned>(),
        std::make_shared<Sleeping>(),
        std::make_shared<Resting>(),
        std::make_shared<Sitting>(),
        std::make_shared<Fighting>(),
        std::make_shared<Standing>()
    };

    std::optional<PositionId> getPositionId(const std::string& name, bool exact) {
        auto found = partialMatch(name, positions.begin(), positions.end(), exact, [](const auto& flag) { return flag->getName(); });
        if(found != positions.end()) {
            return static_cast<PositionId>((*found)->getId());
        }
        return std::nullopt;
    }
}