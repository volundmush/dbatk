#pragma once
#include "dbatk/base.h"


namespace dbat {

    extern std::unique_ptr<SQLite::Database> db;

    extern std::vector<std::string> schema;

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

    void loadZones();

    void loadScripts();

    void loadDatabase();

    void loadObjects();

    void saveZone(std::size_t id);

    void saveZones();

    void saveScripts();

    void saveScript(std::size_t id);

    void savePrototype(const std::string& name, const nlohmann::json& j);

    std::optional<nlohmann::json> getPrototype(const std::string &name);

    void loadLegacySpace();
    void saveLegacySpace();

}