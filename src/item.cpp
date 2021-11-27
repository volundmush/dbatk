//
// Created by volund on 11/27/21.
//

#include "item.h"

namespace dbat::item {
    void cb_create_item(vnum item, entt::entity ent) {}
    void cb_make_item(vnum item, entt::entity ent) {}
    void cb_delete_item(vnum item, entt::entity ent) {}
    void cb_save_item(vnum item, entt::entity ent, nlohmann::json& j) {}
    void cb_load_item(vnum item, entt::entity ent, nlohmann::json& j) {}

    void cb_make_instance(vnum item, entt::entity ent) {}
    void cb_create_instance(vnum item, entt::entity ent) {}
    void cb_save_instance(entt::entity ent, nlohmann::json& j) {}
    void cb_load_instance(entt::entity ent, nlohmann::json& j) {}

    void setup_cb() {
        ring::item::on_create_item = cb_create_item;
        ring::item::on_delete_item = cb_delete_item;
        ring::item::on_make_item = cb_make_item;
        ring::item::on_save_item = cb_save_item;
        ring::item::on_load_item = cb_load_item;
        ring::item::on_make_instance = cb_make_instance;
        ring::item::on_create_instance = cb_create_instance;
        ring::item::on_save_instance = cb_save_instance;
        ring::item::on_load_instance = cb_load_instance;
    }
}