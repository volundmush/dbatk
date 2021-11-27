//
// Created by volund on 11/27/21.
//

#include "session.h"

namespace dbat::session {

    void cb_make_session(uint64_t char_id, entt::entity ent) {}
    void cb_create_session(uint64_t char_id, entt::entity ent) {}
    void cb_handle_output(uint64_t char_id, entt::entity ent) {}
    void cb_save_session(uint64_t char_id, entt::entity ent, nlohmann::json& j) {}
    void cb_load_session(uint64_t char_id, entt::entity ent, nlohmann::json& j) {}
    void cb_handle_command(uint64_t, entt::entity ent, const std::string& txt) {}

    void setup_cb() {
        ring::session::on_make_session = cb_make_session;
        ring::session::on_create_session = cb_create_session;
        ring::session::handle_output = cb_handle_output;
        ring::session::on_save_session = cb_save_session;
        ring::session::on_load_session = cb_load_session;
        ring::session::handle_command = cb_handle_command;
    }

}