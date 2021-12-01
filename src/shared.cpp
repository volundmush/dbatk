//
// Created by volund on 12/1/21.
//

#include "shared.h"

namespace dbat::shared {

    nlohmann::json EntityData::serialize() {
        nlohmann::json j;
        j["vnum"] = vn;
        j["zone"] = zone;
        j["vtype"] = vtype;
        j["name"] = name;
        return j;
    }

}