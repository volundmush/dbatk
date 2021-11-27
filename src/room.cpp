//
// Created by volund on 11/27/21.
//

#include "room.h"

namespace dbat::room {

    void cb_create_room(vnum room, entt::entity ent) {}
    void cb_delete_room(vnum room, entt::entity ent) {}
    void cb_make_room(vnum room, entt::entity ent) {}
    void cb_save_room(vnum room, entt::entity ent, nlohmann::json& j) {}
    void cb_load_room(vnum room, entt::entity ent, nlohmann::json& j) {}

    void setup_cb() {
        ring::room::on_create_room = cb_create_room;
        ring::room::on_make_room = cb_make_room;
        ring::room::on_delete_room = cb_delete_room;
        ring::room::on_save_room = cb_save_room;
        ring::room::on_load_room = cb_load_room;
    }

}