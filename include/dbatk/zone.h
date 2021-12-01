//
// Created by volund on 11/27/21.
//

#ifndef DBATK_ZONE_H
#define DBATK_ZONE_H

#include "sysdep.h"

namespace dbat::zone {

    extern std::map<vnum, entt::entity> zones;

    struct ResetCommand {
        std::string command, sarg1, sarg2;
        bool if_flag = false;
        int arg1 = 0, arg2 = 0, arg3 = 0, arg4 = 0, arg5 = 0;
        /*
        *  Commands:              *
        *  'M': Read a mobile     *
        *  'O': Read an object    *
        *  'G': Give obj to mob   *
        *  'P': Put obj in obj    *
        *  'G': Obj to char       *
        *  'E': Obj to char equip *
        *  'D': Set state of door *
        *  'T': Trigger command   *
        *  'V': Assign a variable *
       */
        nlohmann::json serialize();
    };

    struct ZoneData {
        ZoneData(vnum start, vnum stop);
        int lifespan = 0, age = 0;
        int min_level = 0, max_level = 0;
        uint8_t reset_mode = 0;
        vnum start, stop;
        std::set<flag> flags;
        std::vector<ResetCommand> commands;

        nlohmann::json serialize();
    };

    struct ZoneIndex {
        std::map<vnum, entt::entity> mobiles, items, dgscripts, shops, guilds, rooms;
    };

    extern std::map<vnum, entt::entity> zones;

    opt_type<vnum> zone_for(vnum check);

    entt::entity create(vnum new_zone, vnum start, vnum stop);

    entt::entity make_zone(vnum new_zone, vnum start, vnum stop);

    void load_zone(vnum zone, const nlohmann::json &j);

    void load_zone_folder(const fs::path& zpath);

    nlohmann::json save_zone(vnum zone);

    void save_zone_folder(const fs::path& zpath, vnum zone);

    void load_all(const fs::path& zpath);

}

#endif //DBATK_ZONE_H
