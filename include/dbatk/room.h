//
// Created by volund on 11/27/21.
//

#ifndef DBATK_ROOM_H
#define DBATK_ROOM_H

#include "sysdep.h"
#include "ringmud/room.h"

namespace dbat::room {
    void cb_create_room(vnum room, entt::entity ent);
    void cb_delete_room(vnum room, entt::entity ent);
    void cb_make_room(vnum room, entt::entity ent);
    void cb_save_room(vnum room, entt::entity ent, nlohmann::json& j);
    void cb_load_room(vnum room, entt::entity ent, nlohmann::json& j);

    void setup_cb();
}

#endif //DBATK_ROOM_H
