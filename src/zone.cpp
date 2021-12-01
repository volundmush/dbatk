//
// Created by volund on 11/27/21.
//

#include "core.h"
#include "zone.h"
#include "utils.h"
#include "shared.h"
#include "dgscript.h"
#include "room.h"
#include "mobile.h"
#include "item.h"

namespace dbat::zone {

    std::map<vnum, entt::entity> zones;

    ZoneData::ZoneData(vnum start, vnum stop) : start(start), stop(stop) {}

    nlohmann::json ResetCommand::serialize() {
        nlohmann::json j;
        j["command"] = command;
        j["sarg1"] = sarg1;
        j["sarg2"] = sarg2;
        j["arg1"] = arg1;
        j["arg2"] = arg2;
        j["arg3"] = arg3;
        j["arg4"] = arg4;
        j["arg5"] = arg5;
        j["if_flag"] = if_flag;
        return j;
    }

    nlohmann::json ZoneData::serialize() {
        nlohmann::json j;
        j["lifespan"] = lifespan;
        j["age"] = age;
        j["min_level"] = min_level;
        j["max_level"] = max_level;
        j["reset_mode"] = reset_mode;
        j["start"] = start;
        j["stop"] = stop;
        j["flags"] = flags;

        nlohmann::json j2;
        for(auto& c: commands) {
            j2.push_back(c.serialize());
        }

        if(!j2.empty()) {
            j["commands"] = j2;
        }

        return j;
    }

    opt_type<vnum> zone_for(vnum check) {
        for(const auto &z : zones) {
            const auto &zdata = core::registry.get<ZoneData>(z.second);
            if(check >= zdata.start && check <= zdata.stop) return z.first;
        }
        return {};
    }

    entt::entity create(vnum new_zone, vnum start, vnum stop) {
        if(zones.count(new_zone))
            throw std::runtime_error(fmt::format("Zone {} already exists!", new_zone));

        if((start == stop) || (stop < start))
            throw std::runtime_error("Invalid start/stop vnums.");

        auto conflict = zone_for(start);
        if(conflict.has_value())
            throw std::runtime_error(fmt::format("That vnum range conflicts with Zone {}", conflict.value()));
        conflict = zone_for(stop);
        if(conflict.has_value())
            throw std::runtime_error(fmt::format("That vnum range conflicts with Zone {}", conflict.value()));

        auto new_ent = make_zone(new_zone, start, stop);

        return new_ent;
    }

    entt::entity make_zone(vnum new_zone, vnum start, vnum stop) {
        auto new_ent = core::registry.create();
        auto &sh = core::registry.emplace<shared::EntityData>(new_ent);
        sh.entity = new_ent;
        sh.vn = new_zone;
        sh.vtype = shared::Zone;
        auto &zdata = core::registry.emplace<ZoneData>(new_ent, start, stop);
        core::registry.emplace<ZoneIndex>(new_ent);
        zones[new_zone] = new_ent;

        return new_ent;
    }

    void load_zone(vnum zone, nlohmann::json &j) {
        auto zent = make_zone(zone, j["start"], j["stop"]);
        auto &zdata = core::registry.get<ZoneData>(zent);
        auto &shdata = core::registry.get<shared::EntityData>(zent);
        shdata.name = j["name"];
        zdata.lifespan = j["lifespan"];
        zdata.reset_mode = j["reset_mode"];
        zdata.max_level = j["max_level"];
        zdata.min_level = j["min_level"];

        if(j.count("flags")) {
            for(const flag f : j["flags"]) {
                zdata.flags.insert(f);
            }
        }

        if(j.count("builders")) {
            for(const auto &j2 : j["builders"]) {
                //zdata.builders.
            }
        }

        if(j.count("commands")) {
            for(const auto &j2 : j["commands"]) {
                auto &zcmd = zdata.commands.emplace_back();
                zcmd.command = j2["name"];
                zcmd.if_flag = j2["if_flag"];
                zcmd.arg1 = j2["arg1"];
                zcmd.arg2 = j2["arg2"];
                zcmd.arg3 = j2["arg3"];
                zcmd.arg4 = j2["arg4"];
                zcmd.arg5 = j2["arg5"];
                zcmd.sarg1 = j2["sarg1"];
                zcmd.sarg2 = j2["sarg2"];
            }
        }


    }

    nlohmann::json save_zone(vnum zone) {
        auto ent = zones[zone];

        auto &zdata = core::registry.get<ZoneData>(ent);
        auto j = zdata.serialize();
        auto &shdata = core::registry.get<shared::EntityData>(ent);
        j.update(shdata.serialize());

        return j;
    }

    void save_zone_folder(const fs::path& zpath, vnum zone) {
        auto dir = zpath / std::to_string(zone);
        fs::create_directories(dir);

        auto ent = zones[zone];
        auto j = save_zone(zone);

        util::dump_json(dir, j, "info.json");

        auto &zindex = core::registry.get<ZoneIndex>(ent);

        nlohmann::json dg, rdata, mob, idata;

        for(const auto& v : zindex.dgscripts) {
            dg[std::to_string(v.first)] = dgscript::save_dgscript(v.first);
        }
        util::dump_json(dir, dg, "dgscripts.json");

        for(const auto& v : zindex.dgscripts) {
            rdata[std::to_string(v.first)] = room::save_room(v.first);
        }
        util::dump_json(dir, rdata, "rooms.json");

        for(const auto& v : zindex.mobiles) {
            mob[std::to_string(v.first)] = mobile::save_mobile(v.first);
        }
        util::dump_json(dir, mob, "mobiles.json");

        for(const auto& v : zindex.items) {
            idata[std::to_string(v.first)] = item::save_item(v.first);
        }
        util::dump_json(dir, idata, "items.json");

    }

    void load_zone_folder(const fs::path& zpath) {
        vnum zone_vnum = std::strtoull(zpath.end()->c_str(), nullptr, 10);
        if(zones.count(zone_vnum)) throw std::runtime_error(fmt::format("Load Zone {} called but zone already loaded.", zone_vnum));

        // First, load the zone itself.
        std::ifstream zone_file(zpath / "info.json");
        if(!zone_file.is_open()) throw std::runtime_error(fmt::format("No info.json accessible at {}", zpath.string()));

        nlohmann::json j;
        zone_file >> j;
        zone_file.close();
        load_zone(zone_vnum, j);
        auto ent = zones[zone_vnum];

        // Now we load scripts for the zone.
        auto script_path = zpath / "scripts.json";
        if(fs::exists(script_path)) {
            std::ifstream dgscript_file(script_path);
            if(!dgscript_file.is_open()) throw std::runtime_error(fmt::format("No scripts.json accessible at {}", zpath.string()));
            j.clear();
            dgscript_file >> j;
            dgscript_file.close();
            dgscript::load_dgscripts(zone_vnum, ent, j);
        }

        // next, load rooms.
        auto room_path = zpath / "rooms.json";
        if(fs::exists(room_path)) {
            std::ifstream room_file(room_path);
            if(!room_file.is_open()) throw std::runtime_error(fmt::format("No rooms.json accessible at {}", zpath.string()));
            j.clear();
            room_file >> j;
            room_file.close();
            room::load_rooms(zone_vnum, ent, j);
        }

        // next, load items.
        auto item_path = zpath / "items.json";
        if(fs::exists(item_path)) {
            std::ifstream item_file(item_path);
            if(!item_file.is_open()) throw std::runtime_error(fmt::format("No items.json accessible at {}", zpath.string()));
            j.clear();
            item_file >> j;
            item_file.close();
            item::load_items(zone_vnum, ent, j);
        }

        auto mobile_path = zpath / "mobiles.json";
        if(fs::exists(mobile_path)) {
            std::ifstream mobile_file(mobile_path);
            if(!mobile_file.is_open()) throw std::runtime_error(fmt::format("No mobiles.json accessible at {}", zpath.string()));
            j.clear();
            mobile_file >> j;
            mobile_file.close();
            mobile::load_mobiles(zone_vnum, ent, j);
        }

    }

    void load_all(const fs::path& zpath) {
        if(!zones.empty()) throw std::runtime_error("zone load called while zones already exist!");

        for(const auto& dir_entry : fs::directory_iterator(zpath)) {
            load_zone_folder(dir_entry.path());
        }

    }

}