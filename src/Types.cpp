#include "dbatk/Types.h"

namespace dbat::types {

    void registerDBTypes() {
        registerType(std::make_shared<Room>());
    }

}