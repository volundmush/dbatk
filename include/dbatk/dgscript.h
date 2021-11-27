//
// Created by volund on 11/27/21.
//

#ifndef DBATK_DGSCRIPT_H
#define DBATK_DGSCRIPT_H

#include "sysdep.h"
#include "ringmud/dgscript.h"

namespace dbat::dgscript {

    void cb_create_dgscript(vnum dg, entt::entity ent);
    void cb_delete_dgscript(vnum dg, entt::entity ent);
    void cb_make_dgscript(vnum dg, entt::entity ent);
    void cb_save_dgscript(vnum dg, entt::entity ent, nlohmann::json& j);
    void cb_load_dgscript(vnum dg, entt::entity ent, nlohmann::json& j);

    void setup_cb();

}

#endif //DBATK_DGSCRIPT_H
