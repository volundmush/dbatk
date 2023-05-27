#pragma once
#include "kaizermud/Database.h"
#include <map>

namespace kaizermud::dbat {
    extern std::vector<std::string> extendSchema;

    std::vector<std::pair<std::string, std::reference_wrapper<std::map<int, std::string_view>>>> getFlagDefs();

    void loadFlags(const std::shared_ptr<SQLite::Database>& db);

    void saveFlags(const std::shared_ptr<SQLite::Database>& db);
}