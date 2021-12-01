//
// Created by volund on 11/27/21.
//

#include "dgscript.h"
#include "core.h"
#include "shared.h"
#include "zone.h"

namespace dbat::dgscript {

    std::map<vnum, entt::entity> dgscripts;

    DgScriptEnv::DgScriptEnv(entt::entity ent) : script(ent) {}

    entt::entity make_dgscript(vnum new_dgscript) {
        auto zone = zone::zone_for(new_dgscript);
        if(!zone.has_value()) throw std::runtime_error(fmt::format("No Zone Exists for Vnum: {}", new_dgscript));
        auto new_ent = core::registry.create();
        core::registry.emplace<DgScriptData>(new_ent);
        auto &sh = core::registry.emplace<shared::EntityData>(new_ent);
        sh.vn = new_dgscript;
        sh.entity = new_ent;
        sh.vtype = shared::DgScript;
        sh.zone = zone.value();
        dgscripts[new_dgscript] = new_ent;

        auto zent = zone::zones[zone.value()];
        auto &zindex = core::registry.get<zone::ZoneIndex>(zent);
        zindex.dgscripts[new_dgscript] = new_ent;

        return new_ent;
    }

    entt::entity create(vnum new_dgscript) {
        if(dgscripts.count(new_dgscript))
            throw std::runtime_error(fmt::format("DgScript {} already exists!", new_dgscript));
        auto ent = make_dgscript(new_dgscript);

        return ent;
    }

    nlohmann::json save_dgscript(vnum dg) {
        auto ent = dgscripts[dg];
        nlohmann::json j;
        // TODO: export the dgscripts

        return j;
    }

    void load_dgscript(vnum dgvnum, nlohmann::json &j) {
        auto ent = make_dgscript(dgvnum);
        auto &dgdata = core::registry.get<DgScriptData>(ent);

    }

    void load_dgscripts(vnum zone, entt::entity zent, nlohmann::json &j) {
        for(nlohmann::json::iterator it = j.begin(); it != j.end();it++) {
            vnum dgvnum = std::strtoull(it.key().c_str(), nullptr, 10);
            load_dgscript(dgvnum, it.value());
        }
    }

}