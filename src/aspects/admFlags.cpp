#include "dbatk/aspects/admFlags.h"

namespace dbat::aflags {

    class SimpleAdminFlag : public AdminFlag {
    public:
        SimpleAdminFlag(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    const std::vector<std::shared_ptr<AdminFlag>> adminFlags = {
            std::make_shared<SimpleAdminFlag>(MATRIX, "MATRIX"),
            std::make_shared<SimpleAdminFlag>(UNDEFEATABLE, "UNDEFEATABLE"),
            std::make_shared<SimpleAdminFlag>(MIDAS, "MIDAS"),
            std::make_shared<SimpleAdminFlag>(PARKOUR, "PARKOUR"),
            std::make_shared<SimpleAdminFlag>(GHOST, "GHOST"),
            std::make_shared<SimpleAdminFlag>(ULTRAHAND, "ULTRAHAND"),
            std::make_shared<SimpleAdminFlag>(KIRBY, "KIRBY"),
            std::make_shared<SimpleAdminFlag>(BOOMHEADSHOT, "BOOMHEADSHOT")
    };

    std::optional<AFlagId> getAdminFlagId(const std::string& name, bool exact) {
        auto found = partialMatch(name, adminFlags.begin(), adminFlags.end(), exact, [](const auto& flag) { return flag->getName(); });
        if(found != adminFlags.end()) {
            return static_cast<AFlagId>((*found)->getId());
        }
        return std::nullopt;
    }
}