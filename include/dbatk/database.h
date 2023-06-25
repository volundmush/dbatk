#pragma once
#include "dbatk/base.h"
#include "core/database.h"


namespace dbat {

    extern std::vector<std::string> extraSchema;

    void loadZones();

    void loadScripts();

    void saveZone(std::size_t id);

    void saveZones();

    void saveScripts();

    void saveScript(std::size_t id);

    void loadLegacySpace();
    void saveLegacySpace();

    void deserializeDbatEntity(entt::entity ent, const nlohmann::json& j);
    void serializeDbatEntity(entt::entity ent, bool asPrototype, nlohmann::json& j);

}