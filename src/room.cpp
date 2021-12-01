//
// Created by volund on 11/27/21.
//

#include "room.h"
#include "core.h"
#include "zone.h"

namespace dbat::room {

    std::map<vnum, entt::entity> rooms;

    std::map<Direction, Direction> opposite_dir = {
            {North, South},
            {South, North},

            {East, West},
            {West, East},

            {Up, Down},
            {Down, Up},

            {Northwest, Southeast},
            {Southeast, Northwest},

            {Northeast, Southwest},
            {Southwest, Northeast},

            {Inside, Outside},
            {Outside, Inside}
    };

    entt::entity make_room(vnum zone, vnum new_room) {
        auto new_ent = core::registry.create();
        auto &rdata = core::registry.emplace<RoomData>(new_ent);
        rdata.entity = new_ent;
        rdata.vn = new_room;
        rdata.zone = zone;
        rooms[new_room] = new_ent;
        auto zent = zone::zones[zone];
        auto &zindex = core::registry.get<zone::ZoneIndex>(zent);
        zindex.rooms[new_room] = new_ent;

        return new_ent;
    }

    entt::entity create(vnum new_room) {
        if(rooms.count(new_room)) throw std::runtime_error(fmt::format("Room {} already exists!", new_room));
        auto zone = zone::zone_for(new_room);
        if(!zone.has_value()) throw std::runtime_error(fmt::format("Vnum {} lies outside any zone.", new_room));
        auto ent = make_room(zone.value(), new_room);

        return ent;
    }

    nlohmann::json save_room(vnum room) {
        auto ent = rooms[room];
        nlohmann::json j;
        // TODO: export the room

        return j;
    }

    void load_room(vnum zone, vnum room, nlohmann::json &j) {
        auto ent = make_room(zone, room);
        auto &rdata = core::registry.get<RoomData>(ent);

        // TODO: read from json here.

    }

    void load_rooms(vnum zone, entt::entity zent, nlohmann::json &j) {
        for(nlohmann::json::iterator it = j.begin(); it != j.end();it++) {
            vnum rvnum = std::strtoull(it.key().c_str(), nullptr, 10);
            load_room(zone, rvnum, it.value());
        }
    }

}