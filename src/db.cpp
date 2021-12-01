//
// Created by volund on 11/29/21.
//

#include "db.h"
#include "core.h"

namespace dbat::db {

    std::unique_ptr<SQLite::Database> gamedb;

    bool init_db() {
        gamedb = std::make_unique<SQLite::Database>(core::profile_path / "gamedb.sqlite3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
        SQLite::Statement q(*gamedb, "CREATE TABLE IF NOT EXISTS Migrations(mig_id INT PRIMARY KEY,mig_filename TEXT NOT NULL UNIQUE,mig_date DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP);");
        q.exec();
        auto schema_path = core::profile_path / "schema";

        std::list<std::string> mignames;
        for (const auto &f: std::filesystem::directory_iterator(schema_path)) {
            if (f.exists() && f.is_regular_file()) {
                if (f.path().extension().string() != ".sql") continue;
                mignames.push_back(f.path().filename().string());
            }
        }
        mignames.sort();

        for (const auto &fname: mignames) {
            SQLite::Statement exists(*gamedb, "SELECT * FROM Migrations WHERE mig_filename=?");
            exists.bind(1, fname);
            if (!exists.executeStep()) {
                auto mpath = schema_path / fname;

                std::ifstream ifs(mpath);
                std::string content((std::istreambuf_iterator<char>(ifs)),
                                std::istreambuf_iterator<char>());

                auto err2 = gamedb->exec(content);
                if (err2) {
                    return false;
                }

                SQLite::Statement add_mig(*gamedb, "INSERT INTO Migrations (mig_filename) VALUES (?);");
                add_mig.bind(1, fname);
                add_mig.exec();

            }
        }
        return true;
    }

}