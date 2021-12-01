//
// Created by volund on 11/25/21.
//

#include "utils.h"

namespace dbat::util {
    bool dump_json(const fs::path& dir, nlohmann::json &j, const std::string &fname) {
        if(j.empty())
            return true;
        std::ofstream f(dir / fname);
        if(!f.is_open()) throw std::runtime_error(fmt::format("Cannot open {} at {}", fname, dir.string()));
        f << j.dump(4) << std::endl;
        f.close();
        return true;
    }
}