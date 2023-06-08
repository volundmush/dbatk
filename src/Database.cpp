#include "dbatk/Database.h"
#include "fmt/format.h"
#include "kaizermud/utils.h"
#include "dbatk/Components.h"

namespace dbat {
    std::vector<std::string> extendSchema = {
            "CREATE TABLE IF NOT EXISTS zones ("
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

            "CREATE TABLE IF NOT EXISTS zones_reset_commands ("
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

            "CREATE TABLE IF NOT EXISTS zones_zone_flags ("
            "    zone_id INTEGER,"
            "    flag_id INTEGER,"
            "    PRIMARY KEY(zone_id, flag_id),"
            "    FOREIGN KEY(zone_id) REFERENCES zones(id),"
            "    FOREIGN KEY(flag_id) REFERENCES zone_bits(id)"
            ");",

            "CREATE TABLE IF NOT EXISTS zones_zone_flag_bits ("
            "   id INTEGER PRIMARY KEY,"
            "   name TEXT"
            ");"

            "CREATE TABLE IF NOT EXISTS dgscripts ("
            "    id INTEGER PRIMARY KEY,"
            "    name TEXT,"
            "    script_type INTEGER,"
            "    trigger_type INTEGER,"
            "    script_body TEXT,"
            "    arglist TEXT,"
            "    zone_id INTEGER,"
            "    FOREIGN KEY(zone_id) REFERENCES zones(id)"
            ");",

            "CREATE TABLE IF NOT EXISTS dgscript_npc_bits ("
            "   id INTEGER PRIMARY KEY,"
            "   name TEXT"
            ");",

            "CREATE TABLE IF NOT EXISTS dgscript_room_bits ("
            "   id INTEGER PRIMARY KEY,"
            "   name TEXT"
            ");",

            "CREATE TABLE IF NOT EXISTS dgscript_item_bits ("
            "   id INTEGER PRIMARY KEY,"
            "   name TEXT"
            ");"
    };

    void serializeEntity(entt::entity ent, nlohmann::json& j, bool asPrototype) {
        auto room = kaizer::registry.try_get<dbat::components::Room>(ent);
        if(room) {
            nlohmann::json r;
            r["terrain"] = room->terrain;
            j["Room"] = r;
        }

        auto character = kaizer::registry.try_get<dbat::components::Character>(ent);
        if(character) {
            nlohmann::json c;
            c["race"] = character->race;
            c["sensei"] = character->sensei;
            c["position"] = character->position;
            c["sex"] = character->sex;
            j["Character"] = c;
        }

    }

    void deserializeEntity(entt::entity ent, const nlohmann::json& j) {
        if(j.count("Character")) {
            auto &character = kaizer::registry.get_or_emplace<dbat::components::Character>(ent);
            auto c = j["Character"];
            if(c.contains("race")) character.race = c["race"];
            if(c.contains("sensei")) character.sensei = c["sensei"];
            if(c.contains("position")) character.position = c["position"];
            if(c.contains("sex")) character.sex = c["sex"];
        }

        if(j.count("Room")) {
            auto &room = kaizer::registry.get_or_emplace<dbat::components::Room>(ent);
            auto r = j["Room"];
            if(r.contains("terrain")) room.terrain = r["terrain"];
        }
    }

    void registerDBFuncs() {
        kaizer::serializeFuncs.push_back(serializeEntity);
        kaizer::deserializeFuncs.push_back(deserializeEntity);
    }

}