//
// Created by volund on 11/27/21.
//

#ifndef DBATK_ZONE_H
#define DBATK_ZONE_H

#include "sysdep.h"
#include "ringmud/zone.h"

namespace dbat::zone {

    void cb_create_zone(vnum zone, entt::entity ent);
    void cb_delete_zone(vnum zone, entt::entity ent);
    void cb_make_zone(vnum zone, entt::entity ent);
    void cb_save_zone(vnum zone, entt::entity ent, nlohmann::json& j);
    void cb_load_zone(vnum zone, entt::entity ent, nlohmann::json& j);

    void cb_save_zone_folder(vnum zone, entt::entity ent, const fs::path& zpath);
    void cb_load_zone_folder(vnum zone, entt::entity ent, const fs::path& zpath);

    void setup_cb();

}

#endif //DBATK_ZONE_H
