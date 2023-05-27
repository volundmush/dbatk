#include "dbatk/Database.h"
#include "fmt/format.h"
#include "kaizermud/utils.h"

namespace kaizermud::dbat {
    std::vector<std::string> extendSchema = {
            "CREATE TABLE zones ("
            "    id INTEGER PRIMARY KEY,"
            "    name TEXT,"
            "    builders TEXT,"
            "    lifespan INTEGER,"
            "    age INTEGER,"
            "    bot INTEGER,"
            "    top INTEGER,"
            "    reset_mode INTEGER,"
            "    min_level INTEGER,"
            "    max_level INTEGER"
            ");",

            "CREATE TABLE zones_reset_commands ("
            "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "    zone_id INTEGER,"
            "    command CHAR,"
            "    if_flag BOOLEAN,"
            "    arg1 INTEGER,"
            "    arg2 INTEGER,"
            "    arg3 INTEGER,"
            "    arg4 INTEGER,"
            "    arg5 INTEGER,"
            "    line INTEGER,"
            "    sarg1 TEXT,"
            "    sarg2 TEXT,"
            "    FOREIGN KEY(zone_id) REFERENCES zones(id)"
            ");",

            "CREATE TABLE zones_zone_flags ("
            "    zone_id INTEGER,"
            "    flag_id INTEGER,"
            "    PRIMARY KEY(zone_id, flag_id),"
            "    FOREIGN KEY(zone_id) REFERENCES zones(id),"
            "    FOREIGN KEY(flag_id) REFERENCES zone_bits(id)"
            ");",

            "CREATE TABLE zones_zone_flag_bits ("
            "   id INTEGER PRIMARY KEY,"
            "   name TEXT"
            ");"

            "CREATE TABLE dgscripts ("
            "    id INTEGER PRIMARY KEY,"
            "    name TEXT,"
            "    script_type INTEGER,"
            "    trigger_type INTEGER,"
            "    script_body TEXT,"
            "    arglist TEXT,"
            "    zone_id INTEGER,"
            "    FOREIGN KEY(zone_id) REFERENCES zones(id)"
            ");",

            "CREATE TABLE dgscript_npc_bits ("
            "   id INTEGER PRIMARY KEY,"
            "   name TEXT"
            ");",

            "CREATE TABLE dgscript_room_bits ("
            "   id INTEGER PRIMARY KEY,"
            "   name TEXT"
            ");",

            "CREATE TABLE dgscript_item_bits ("
            "   id INTEGER PRIMARY KEY,"
            "   name TEXT"
            ");",

            "CREATE TABLE player_flag_bits ("
            "   id INTEGER PRIMARY KEY,"
            "   name TEXT"
            ");",

            "CREATE TABLE player_flags ("
            "   object_id INTEGER,"
            "   flag_id INTEGER,"
            "   PRIMARY KEY(object_id, flag_id)"
            ");",

            "CREATE TABLE npc_flag_bits ("
            "   id INTEGER PRIMARY KEY,"
            "   name TEXT"
            ");",

            "CREATE TABLE npc_flags ("
            "   object_id INTEGER,"
            "   flag_id INTEGER,"
            "   PRIMARY KEY(object_id, flag_id)"
            ");",

            "CREATE TABLE room_flag_bits ("
            "   id INTEGER PRIMARY KEY,"
            "   name TEXT"
            ");",

            "CREATE TABLE room_flags ("
            "   object_id INTEGER,"
            "   flag_id INTEGER,"
            "   PRIMARY KEY(object_id, flag_id)"
            ");",

            "CREATE TABLE exit_flag_bits ("
            "   id INTEGER PRIMARY KEY,"
            "   name TEXT"
            ");",

            "CREATE TABLE exit_flags ("
            "   object_id INTEGER,"
            "   flag_id INTEGER,"
            "   PRIMARY KEY(object_id, flag_id)"
            ");",

            "CREATE TABLE admin_flag_bits ("
            "   id INTEGER PRIMARY KEY,"
            "   name TEXT"
            ");",

            "CREATE TABLE admin_flags ("
            "   object_id INTEGER,"
            "   flag_id INTEGER,"
            "   PRIMARY KEY(object_id, flag_id)"
            ");",

            "CREATE TABLE preference_flag_bits ("
            "   id INTEGER PRIMARY KEY,"
            "   name TEXT"
            ");",

            "CREATE TABLE preference_flags ("
            "   object_id INTEGER,"
            "   flag_id INTEGER,"
            "   PRIMARY KEY(object_id, flag_id)"
            ");",

    };

    std::vector<std::string> protoSchema = {
            "CREATE TABLE IF NOT EXISTS protos (\n"
            "    id INTEGER PRIMARY KEY,\n"
            "    vnum INTEGER NOT NULL,\n"
            "    mainType TEXT NOT NULL,\n"
            "    subType TEXT NOT NULL,\n"
            "    UNIQUE(vnum,mainType)"
            ");",

            "CREATE TABLE IF NOT EXISTS protoStrings (\n"
            "    id INTEGER PRIMARY KEY,\n"
            "    protoId INTEGER NOT NULL,\n"
            "    key TEXT NOT NULL,\n"
            "    value TEXT NOT NULL,\n"
            "    UNIQUE(protoId, key)\n"
            ");",

            "CREATE TABLE IF NOT EXISTS protoStats (\n"
            "    id INTEGER PRIMARY KEY,\n"
            "    protoId INTEGER NOT NULL,\n"
            "    statType TEXT NOT NULL,\n"
            "    value REAL NOT NULL,\n"
            "    UNIQUE(protoId, statType)\n"
            ");",

            "CREATE TABLE IF NOT EXISTS protoAspect ("
            "   id INTEGER PRIMARY KEY,"
            "   protoId INTEGER NOT NULL,"
            "   aspectType TEXT NOT NULL,"
            "   aspectKey TEXT NOT NULL,"
            "   UNIQUE(protoId, aspectType)"
            ");",

            "CREATE TABLE IF NOT EXISTS protoFlags ("
            "   protoId INTEGER NOT NULL,"
            "   flagType TEXT NOT NULL,"
            "   flagId INTEGER NOT NULL,"
            "   PRIMARY KEY(protoId, flagType, flagId)"
            ");"
    };

    std::vector<std::pair<std::string, std::reference_wrapper<std::map<int, std::string>>>> getFlagDefs() {
        return {

        };
    }

    void loadFlags(const std::shared_ptr<SQLite::Database>& db) {
        for(const auto &[tbl_name, flag_map] : getFlagDefs()) {
            auto query = fmt::format("SELECT id, name FROM {}", tbl_name);
            auto stmt = SQLite::Statement(*db, query);
            auto fmap = flag_map.get();
            while(stmt.executeStep()) {
                auto id = stmt.getColumn(0).getInt();
                auto name = stmt.getColumn(1).getText();
                fmap.emplace(id, name);
            }

        }
    }

    void saveFlags(const std::shared_ptr<SQLite::Database>& db) {
        for(const auto &[tbl_name, flag_map] : getFlagDefs()) {
            auto query = fmt::format("INSERT INTO {} (id, name) VALUES (?, ?)", tbl_name);
            auto stmt = SQLite::Statement(*db, query);
            for(const auto &[id, name] : flag_map.get()) {
                stmt.bind(1, id);
                stmt.bind(2, std::string(name));
                stmt.exec();
            }
        }
    }

}