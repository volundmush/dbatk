//
// Created by volund on 11/26/21.
//

#ifndef DBATK_CONNECTION_H
#define DBATK_CONNECTION_H

#include "sysdep.h"
#include "ringmud/connection.h"

namespace dbat::conn {

    enum ConnState : uint8_t {
        Login = 0,
        UserMenu = 1,
        Session = 2
    };

    struct StateData {
        ConnState state = Login;
        opt_type<int64_t> user_id;
        opt_type<uint64_t> session;
    };

    void cb_make_connection(uint64_t conn_id, entt::entity ent);
    void cb_create_connection(uint64_t conn_id, entt::entity ent);
    void cb_close_connection(uint64_t conn_id, entt::entity ent);

    void cb_load_connection(uint64_t conn_id, entt::entity ent, nlohmann::json& j);
    void cb_save_connection(uint64_t conn_id, entt::entity ent, nlohmann::json& j);

    void cb_handle_input(uint64_t conn_id, entt::entity ent, nlohmann::json& j);

    void handle_cmd_login(uint64_t conn_id, entt::entity ent, const std::string& txt);

    void handle_cmd_menu(uint64_t conn_id, entt::entity ent, const std::string& txt);

    void handle_cmd_session(uint64_t conn_id, entt::entity ent, const std::string& txt);

    void setup_cb();

}

#endif //DBATK_CONNECTION_H
