#include "dbatk/exit/flags.h"


namespace dbat::eflags {
    const std::vector<std::shared_ptr<ExitFlag>> exitFlags = {
            std::make_shared<SimpleExitFlag>(ISDOOR, "ISDOOR"),
            std::make_shared<SimpleExitFlag>(CLOSED, "CLOSED"),
            std::make_shared<SimpleExitFlag>(LOCKED, "LOCKED"),
            std::make_shared<SimpleExitFlag>(PICKPROOF, "PICKPROOF"),
            std::make_shared<SimpleExitFlag>(SECRET, "SECRET"),
    };
}