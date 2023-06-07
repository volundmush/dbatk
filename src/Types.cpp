#include "dbatk/Types.h"

namespace dbat::types {

    void registerDBTypes() {
        kaizer::typeRegistry["room"] = std::make_shared<Room>();
    }

}