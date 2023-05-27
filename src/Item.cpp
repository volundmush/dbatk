#include "dbatk/Item.h"

namespace kaizermud::dbat {
    std::string_view Item::getMainType() const {
        return "item";
    }

}