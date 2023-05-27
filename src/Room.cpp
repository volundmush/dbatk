#include "dbatk/Room.h"

namespace kaizermud::dbat {
    std::string_view Room::getMainType() const {
        return "room";
    }

}