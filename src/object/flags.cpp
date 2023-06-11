#include "dbatk/object/flags.h"

namespace dbat::oflags {
    const std::vector<std::shared_ptr<ObjectFlag>> objectFlags = {
        std::make_shared<SimpleObjectFlag>(OFlag::GLOBALROOMS, "GLOBALROOMS")
    };
}