//
// Created by volund on 11/27/21.
//

#include "zone.h"

namespace dbat::zone {
    void cb_create_zone(vnum zone, entt::entity ent) {}
    void cb_delete_zone(vnum zone, entt::entity ent) {}
    void cb_make_zone(vnum zone, entt::entity ent) {}
    void cb_save_zone(vnum zone, entt::entity ent, nlohmann::json& j) {}
    void cb_load_zone(vnum zone, entt::entity ent, nlohmann::json& j) {}

    void cb_save_zone_folder(vnum zone, entt::entity ent, const fs::path& zpath) {}
    void cb_load_zone_folder(vnum zone, entt::entity ent, const fs::path& zpath) {}

    void setup_cb() {
        ring::zone::on_make_zone = cb_make_zone;
        ring::zone::on_create_zone = cb_create_zone;
        ring::zone::on_delete_zone = cb_delete_zone;
        ring::zone::on_save_zone = cb_save_zone;
        ring::zone::on_load_zone = cb_load_zone;
        ring::zone::on_save_zone_folder = cb_save_zone_folder;
        ring::zone::on_load_zone_folder = cb_load_zone_folder;
    }
}