//
// Created by volund on 11/27/21.
//

#ifndef DBATK_ITEM_H
#define DBATK_ITEM_H

#include "sysdep.h"
#include "ringmud/item.h"

namespace dbat::item {
    void cb_create_item(vnum item, entt::entity ent);
    void cb_make_item(vnum item, entt::entity ent);
    void cb_delete_item(vnum item, entt::entity ent);
    void cb_save_item(vnum item, entt::entity ent, nlohmann::json& j);
    void cb_load_item(vnum item, entt::entity ent, nlohmann::json& j);

    void cb_make_instance(vnum item, entt::entity ent);
    void cb_create_instance(vnum item, entt::entity ent);
    void cb_save_instance(entt::entity ent, nlohmann::json& j);
    void cb_load_instance(entt::entity ent, nlohmann::json& j);

    void setup_cb();
}

#endif //DBATK_ITEM_H
