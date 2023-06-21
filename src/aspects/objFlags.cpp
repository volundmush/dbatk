#include "dbatk/aspects/objFlags.h"

namespace dbat::oflags {
    const std::vector<std::shared_ptr<ObjectFlag>> objectFlags = {
        std::make_shared<SimpleObjectFlag>(oflags::GLOBALROOMS, "GLOBALROOMS")
    };
}