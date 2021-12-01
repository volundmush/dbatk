//
// Created by volund on 11/27/21.
//

#include "mobile.h"
#include "core.h"

namespace dbat::mobile {
    std::map<vnum, entt::entity> mobiles;
    std::set<entt::entity> instances;


    entt::entity make_instance(vnum new_mob) {
        auto pent = mobiles[new_mob];
        auto new_ent = core::registry.create();
        auto &iprot = core::registry.get<MobileProtoData>(pent);
        auto &idata = core::registry.emplace<MobileInstanceData>(new_ent);
        idata.vn = new_mob;
        idata.entity = new_ent;
        iprot.instances.insert(new_ent);
        idata.prototype = pent;
        instances.insert(new_ent);

        return new_ent;
    }

    entt::entity make_mobile(vnum new_mob) {
        auto new_ent = core::registry.create();
        auto &iprot = core::registry.emplace<MobileProtoData>(new_ent);
        iprot.vn = new_mob;
        iprot.entity = new_ent;
        mobiles[new_mob] = new_ent;

        return new_ent;
    }

    entt::entity create_instance(vnum new_mob) {
        if(!mobiles.count(new_mob))
            throw std::runtime_error(fmt::format("Mobile {} does not exist!", new_mob));
        auto ent = make_instance(new_mob);

        return ent;
    }

    entt::entity create(vnum new_mob) {
        if(mobiles.count(new_mob))
            throw std::runtime_error(fmt::format("Mobile {} already exists!", new_mob));
        auto ent = make_mobile(new_mob);

        return ent;
    }

    nlohmann::json save_instance(entt::entity ent) {
        nlohmann::json j;

        return j;
    }

    nlohmann::json save_mobile(vnum mob) {
        auto ent = mobiles[mob];
        nlohmann::json j;

        return j;
    }

    entt::entity load_mobile(vnum mobile, nlohmann::json& j) {
        auto ent = make_mobile(mobile);
        auto &pdata = core::registry.get<MobileProtoData>(ent);

        return ent;
    }

    entt::entity load_instance(vnum item, nlohmann::json& j) {
        auto ent = make_instance(item);

    }

    void load_mobiles(vnum zone, entt::entity zent, nlohmann::json& j) {
        for(nlohmann::json::iterator it = j.begin(); it != j.end();it++) {
            vnum vn = std::strtoull(it.key().c_str(), nullptr, 10);
            load_mobile(vn, it.value());
        }
    }
}