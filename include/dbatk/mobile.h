//
// Created by volund on 11/27/21.
//

#ifndef DBATK_MOBILE_H
#define DBATK_MOBILE_H

#include "sysdep.h"

namespace dbat::mobile {
    extern std::map<vnum, entt::entity> mobiles;
    extern std::set<entt::entity> instances;

    struct MobileProtoData {
        entt::entity entity;
        vnum vn;
        std::set<entt::entity> instances;
    };

    struct MobileInstanceData {
        entt::entity entity;
        vnum vn;
        entt::entity prototype;
        std::list<std::string> input;
        opt_type<entt::entity> session;
    };

    entt::entity make_instance(vnum new_mob);

    entt::entity create_instance(vnum new_mob);

    entt::entity load_instance(vnum mob, nlohmann::json& j);

    entt::entity make_mobile(vnum new_mob);

    entt::entity create(vnum new_mob);

    nlohmann::json save_instance(entt::entity ent);

    nlohmann::json save_mobile(vnum mob);

    entt::entity load_mobile(vnum mobile, nlohmann::json& j);

    void load_mobiles(vnum zone, entt::entity zent, nlohmann::json& j);
}

#endif //DBATK_MOBILE_H
