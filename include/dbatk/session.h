//
// Created by volund on 11/27/21.
//

#ifndef DBATK_SESSION_H
#define DBATK_SESSION_H

#include "sysdep.h"
#include "ringmud/session.h"

namespace dbat::session {

    void cb_make_session(uint64_t char_id, entt::entity ent);
    void cb_create_session(uint64_t char_id, entt::entity ent);
    void cb_handle_output(uint64_t char_id, entt::entity ent);
    void cb_save_session(uint64_t char_id, entt::entity ent, nlohmann::json& j);
    void cb_load_session(uint64_t char_id, entt::entity ent, nlohmann::json& j);
    void cb_handle_command(uint64_t, entt::entity ent, const std::string& txt);

    void setup_cb();
}

#endif //DBATK_SESSION_H
