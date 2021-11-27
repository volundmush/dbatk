//
// Created by volund on 11/26/21.
//

#include "connection.h"

namespace dbat::conn {

    void cb_make_connection(uint64_t conn_id, entt::entity ent) {

    }

    void cb_create_connection(uint64_t conn_id, entt::entity ent) {

    }

    void cb_close_connection(uint64_t conn_id, entt::entity ent) {

    }

    void cb_load_connection(uint64_t conn_id, entt::entity ent, nlohmann::json& j) {

    }

    void cb_save_connection(uint64_t conn_id, entt::entity ent, nlohmann::json& j) {

    }

    void cb_handle_input(uint64_t conn_id, entt::entity ent, nlohmann::json& j) {

    }

    void setup_cb() {
        ring::conn::on_make_connection = cb_make_connection;
        ring::conn::on_create_connection = cb_create_connection;
        ring::conn::on_close_connection = cb_close_connection;
        ring::conn::on_load_connection = cb_load_connection;
        ring::conn::on_save_connection = cb_save_connection;
        ring::conn::handle_input = cb_handle_input;
    }

}