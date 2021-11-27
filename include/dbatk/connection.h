//
// Created by volund on 11/26/21.
//

#ifndef DBATK_CONNECTION_H
#define DBATK_CONNECTION_H

#include "sysdep.h"
#include "ringmud/connection.h"

namespace dbat::conn {

    void cb_make_connection(uint64_t conn_id, entt::entity ent);
    void cb_create_connection(uint64_t conn_id, entt::entity ent);
    void cb_close_connection(uint64_t conn_id, entt::entity ent);

    void cb_load_connection(uint64_t conn_id, entt::entity ent, nlohmann::json& j);
    void cb_save_connection(uint64_t conn_id, entt::entity ent, nlohmann::json& j);

    void cb_handle_input(uint64_t conn_id, entt::entity ent, nlohmann::json& j);

    void setup_cb();

}

#endif //DBATK_CONNECTION_H
