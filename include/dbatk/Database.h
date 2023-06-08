#pragma once
#include "kaizermud/Database.h"


namespace dbat {
    extern std::vector<std::string> extendSchema;

    void serializeEntity(entt::entity ent, nlohmann::json& j, bool asPrototype);

    void deserializeEntity(entt::entity ent, const nlohmann::json& j);

    void registerDBFuncs();

}