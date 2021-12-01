//
// Created by volund on 11/25/21.
//

#ifndef DBATK_UTILS_H
#define DBATK_UTILS_H

#include "core.h"

namespace dbat::util {
    bool dump_json(const fs::path& dir, nlohmann::json &j, const std::string &fname);
}

#endif //RINGMUD_UTILS_H
