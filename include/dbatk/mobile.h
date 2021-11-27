//
// Created by volund on 11/27/21.
//

#ifndef DBATK_MOBILE_H
#define DBATK_MOBILE_H

#include "sysdep.h"
#include "ringmud/mobile.h"

namespace dbat::mobile {
    void cb_create_mobile(vnum mob, entt::entity ent);
    void cb_make_mobile(vnum mob, entt::entity ent);
    void cb_delete_mobile(vnum mob, entt::entity ent);
    void cb_mobile_input(entt::entity ent);
    void cb_mobile_command(entt::entity ent, const std::string& cmd);
    void cb_save_mobile(vnum mob, entt::entity ent, nlohmann::json& j);
    void cb_load_mobile(vnum mob, entt::entity ent, nlohmann::json& j);

    void cb_make_instance(vnum mob, entt::entity ent);
    void cb_create_instance(vnum mob, entt::entity ent);
    void cb_save_instance(entt::entity ent, nlohmann::json& j);
    void cb_load_instance(entt::entity ent, nlohmann::json& j);

    void setup_cb();
}

#endif //DBATK_MOBILE_H
