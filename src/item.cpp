//
// Created by volund on 11/27/21.
//

#include "item.h"
#include "core.h"

namespace dbat::item {
    std::map<vnum, entt::entity> items;
    std::set<entt::entity> instances;

    entt::entity make_instance(vnum new_item) {
        auto pent = items[new_item];
        auto new_ent = core::registry.create();
        auto &iprot = core::registry.get<ItemProtoData>(pent);
        auto &idata = core::registry.emplace<ItemInstanceData>(new_ent);
        idata.vn = new_item;
        idata.entity = new_ent;
        iprot.instances.insert(new_ent);
        idata.prototype = pent;
        instances.insert(new_ent);

        return new_ent;
    }

    entt::entity make_item(vnum new_item) {
        auto new_ent = core::registry.create();
        auto &iprot = core::registry.emplace<ItemProtoData>(new_ent);
        iprot.vn = new_item;
        iprot.entity = new_ent;
        items[new_item] = new_ent;

        return new_ent;
    }

    entt::entity create_instance(vnum new_item) {
        if(!items.count(new_item))
            throw std::runtime_error(fmt::format("Item {} does not exist!", new_item));
        auto ent = make_instance(new_item);

        return ent;
    }

    entt::entity create(vnum new_item) {
        if(items.count(new_item))
            throw std::runtime_error(fmt::format("Item {} already exists!", new_item));
        auto ent = make_item(new_item);

        return ent;
    }

    nlohmann::json save_instance(entt::entity ent) {
        nlohmann::json j;

        return j;
    }

    nlohmann::json save_item(vnum item) {
        auto ent = items[item];
        nlohmann::json j;

        return j;
    }

    entt::entity load_item(vnum item, nlohmann::json& j) {
        auto ent = make_item(item);
        auto &pdata = core::registry.get<ItemProtoData>(ent);

        return ent;
    }

    entt::entity load_instance(vnum item, nlohmann::json& j) {
        auto ent = make_instance(item);

    }

    void load_items(vnum zone, entt::entity zent, nlohmann::json& j) {
        for(nlohmann::json::iterator it = j.begin(); it != j.end();it++) {
            vnum vn = std::strtoull(it.key().c_str(), nullptr, 10);
            load_item(vn, it.value());
        }
    }
}