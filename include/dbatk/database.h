#pragma once
#include "dbatk/base.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include <bitset>

namespace dbat {

    struct Location;

    // For backwards compatability with the old DBAT code, we have this map which effectively replicates
    // the old 'world' variable. It is filled with the rooms from Objects that are marked GLOBALROOM.
    // Beware of ID collisions when setting objects GLOBALROOM.
    extern std::unordered_map<RoomId, Location> legacyRooms;

    extern std::unique_ptr<SQLite::Database> db;

    extern std::vector<std::string> schema;

    struct PrototypeData {
        std::string name;
        nlohmann::json data;
        std::size_t instanceCount;
        entt::entity spawn();
        std::string entityName();
    };

    extern std::unordered_map<std::string, std::shared_ptr<PrototypeData>> prototypes;

    template<size_t N>
    nlohmann::json bitsetToJson(const std::bitset<N>& bits) {
        nlohmann::json json = nlohmann::json::array();
        if(bits.none()) return json;
        for (size_t i = 0; i < N; ++i) {
            if (bits[i]) {
                json.push_back(i);
            }
        }
        return json;
    }

    template<size_t N>
    void jsonToBitset(const nlohmann::json& json, std::bitset<N>& bits) {
        bits.reset(); // Clear all bits
        for (auto& element : json) {
            bits.set(element);
        }
    }

    nlohmann::json serializeEntity(entt::entity ent, bool asPrototype = false);

    void deserializeEntity(entt::entity ent, const nlohmann::json& j);

    void processDirty();

    void readyDatabase();

    void loadPrototypes();

    void loadDatabase();

    void loadObjects();

    void savePrototype(const std::string& name, const nlohmann::json& j);

    std::optional<nlohmann::json> getPrototype(const std::string &name);

    void loadZones();

    void loadScripts();

    void saveZone(std::size_t id);

    void saveZones();

    void saveScripts();

    void saveScript(std::size_t id);

    void loadLegacySpace();
    void saveLegacySpace();

    void loadRelations();

}