#include "dbatk/aspects/objFlags.h"

namespace dbat::oflags {
    class SimpleObjectFlag : public ObjectFlag {
    public:
        SimpleObjectFlag(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    const std::vector<std::shared_ptr<ObjectFlag>> objectFlags = {
        std::make_shared<SimpleObjectFlag>(oflags::GLOBALROOMS, "GLOBALROOMS")
    };
}