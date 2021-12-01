//
// Created by volund on 11/27/21.
//

#ifndef DBATK_ROOM_H
#define DBATK_ROOM_H

#include "sysdep.h"
#include "shared.h"

namespace dbat::room {
    extern std::map<vnum, entt::entity> rooms;

    enum Direction : uint8_t {
        North = 0,
        East = 1,
        South = 2,
        West = 3,
        Up = 4,
        Down = 5,
        Northwest = 6,
        Northeast = 7,
        Southeast = 8,
        Southwest = 9,
        Inside = 10,
        Outside = 11
    };

    enum ExitFlags : uint8_t {
        ExDoor = 0,
        ExClosed = 1,
        ExLocked = 2,
        ExPickProof = 3,
        ExSecret = 4
    };

    extern std::map<Direction, Direction> opposite_dir;

    struct ExitData {
        Direction dir;
        std::string description;
        std::vector<std::string> keywords;
        std::bitset<5> flags;
        uint16_t exit_info = 0;
        opt_type<vnum> key, to_room, failroom, totalfailroom;
        int dclock = 0, dchide = 0, dcskill = 0, dcmove = 0, failsavetype = 0, dcfailsave = 0;
    };


    struct RoomData {
        vnum zone, vn;
        entt::entity entity;
        std::string name = "Unnamed Room", description = "It looks unfinished...";
        std::vector<shared::ExDesc> ex_descriptions;
        std::map<Direction, ExitData> exits;
        std::set<flag> flags;
        std::set<vnum> objects, mobiles, triggers;
        uint8_t sector_type;
    };

    entt::entity make_room(vnum zone, vnum new_room);

    entt::entity create(vnum new_room);

    nlohmann::json save_room(vnum room);

    void load_room(vnum zone, vnum room, nlohmann::json &j);

    void load_rooms(vnum zone, entt::entity zent, nlohmann::json &j);
}

#endif //DBATK_ROOM_H
