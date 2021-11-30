//
// Created by volund on 11/29/21.
//

#ifndef DBATK_DB_H
#define DBATK_DB_H

#include "sysdep.h"
#include "SQLiteCpp/SQLiteCpp.h"

namespace dbat::db {

    extern std::unique_ptr<SQLite::Database> gamedb;

    bool init_db();

}

#endif //DBATK_DB_H
