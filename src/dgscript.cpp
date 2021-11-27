//
// Created by volund on 11/27/21.
//

#include "dgscript.h"

namespace dbat::dgscript {

    void cb_create_dgscript(vnum dg, entt::entity ent) {}
    void cb_delete_dgscript(vnum dg, entt::entity ent) {}
    void cb_make_dgscript(vnum dg, entt::entity ent) {}
    void cb_save_dgscript(vnum dg, entt::entity ent, nlohmann::json& j) {}
    void cb_load_dgscript(vnum dg, entt::entity ent, nlohmann::json& j) {}

    void setup_cb() {
        ring::dgscript::on_create_dgscript = cb_create_dgscript;
        ring::dgscript::on_make_dgscript = cb_make_dgscript;
        ring::dgscript::on_delete_dgscript = cb_delete_dgscript;
        ring::dgscript::on_save_dgscript = cb_save_dgscript;
        ring::dgscript::on_load_dgscript = cb_load_dgscript;
    }

}