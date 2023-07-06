#include "dbatk/aspects/position.h"

namespace dbat::position {
    struct Dead : Position {
        [[nodiscard]] std::size_t getId() override {return DEAD;};
        [[nodiscard]] std::string getName() override {return "Dead";}
    };

    struct MortallyWounded : Position {
        [[nodiscard]] std::size_t getId() override {return MORTALLY_WOUNDED;};
        [[nodiscard]] std::string getName() override {return "Mortally Wounded";}
    };

    struct Incapacitated : Position {
        [[nodiscard]] std::size_t getId() override {return INCAPACITATED;};
        [[nodiscard]] std::string getName() override {return "Incapacitated";}
    };

    struct Stunned : Position {
        [[nodiscard]] std::size_t getId() override {return STUNNED;};
        [[nodiscard]] std::string getName() override {return "Stunned";}
    };

    struct Sleeping : Position {
        [[nodiscard]] std::size_t getId() override {return SLEEPING;};
        [[nodiscard]] std::string getName() override {return "Sleeping";}
    };

    struct Resting : Position {
        [[nodiscard]] std::size_t getId() override {return RESTING;};
        [[nodiscard]] std::string getName() override {return "Resting";}
    };

    struct Sitting : Position {
        [[nodiscard]] std::size_t getId() override {return SITTING;};
        [[nodiscard]] std::string getName() override {return "Sitting";}
    };

    struct Fighting : Position {
        [[nodiscard]] std::size_t getId() override {return FIGHTING;};
        [[nodiscard]] std::string getName() override {return "Fighting";}
    };

    struct Standing : Position {
        [[nodiscard]] std::size_t getId() override {return STANDING;};
        [[nodiscard]] std::string getName() override {return "Standing";}
    };

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