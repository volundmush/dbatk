#pragma once
#include "kaizermud/Database.h"
#include <map>

namespace dbat {
    extern std::vector<std::string> extendSchema;

    std::vector<std::pair<std::string, std::reference_wrapper<std::map<int, std::string>>>> getFlagDefs();

    void loadFlags(const std::shared_ptr<SQLite::Database>& db);

    void saveFlags(const std::shared_ptr<SQLite::Database>& db);
}