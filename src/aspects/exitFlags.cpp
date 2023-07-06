#include "dbatk/aspects/exitFlags.h"


namespace dbat::eflags {
    class SimpleExitFlag : public ExitFlag {
    public:
        SimpleExitFlag(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    const std::vector<std::shared_ptr<ExitFlag>> exitFlags = {
            std::make_shared<SimpleExitFlag>(ISDOOR, "ISDOOR"),
            std::make_shared<SimpleExitFlag>(CLOSED, "CLOSED"),
            std::make_shared<SimpleExitFlag>(LOCKED, "LOCKED"),
            std::make_shared<SimpleExitFlag>(PICKPROOF, "PICKPROOF"),
            std::make_shared<SimpleExitFlag>(SECRET, "SECRET"),
    };
}