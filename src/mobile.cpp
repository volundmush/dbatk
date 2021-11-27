//
// Created by volund on 11/27/21.
//

#include "mobile.h"

namespace dbat::mobile {
    void cb_create_mobile(vnum mob, entt::entity ent) {}
    void cb_make_mobile(vnum mob, entt::entity ent) {}
    void cb_delete_mobile(vnum mob, entt::entity ent) {}
    void cb_mobile_input(entt::entity ent) {}
    void cb_mobile_command(entt::entity ent, const std::string& cmd) {}
    void cb_save_mobile(vnum mob, entt::entity ent, nlohmann::json& j) {}
    void cb_load_mobile(vnum mob, entt::entity ent, nlohmann::json& j) {}

    void cb_make_instance(vnum mob, entt::entity ent) {}
    void cb_create_instance(vnum mob, entt::entity ent) {}
    void cb_save_instance(entt::entity ent, nlohmann::json& j) {}
    void cb_load_instance(entt::entity ent, nlohmann::json& j) {}

    void setup_cb() {
        ring::mobile::on_create_mobile = cb_create_mobile;
        ring::mobile::on_delete_mobile = cb_delete_mobile;
        ring::mobile::on_make_mobile = cb_make_mobile;
        ring::mobile::on_save_mobile = cb_save_mobile;
        ring::mobile::on_load_mobile = cb_load_mobile;
        ring::mobile::handle_mobile_input = cb_mobile_input;
        ring::mobile::handle_mobile_command = cb_mobile_command;
        ring::mobile::on_make_instance = cb_make_instance;
        ring::mobile::on_create_instance = cb_create_instance;
        ring::mobile::on_save_instance = cb_save_instance;
        ring::mobile::on_load_instance = cb_load_instance;
    }
}