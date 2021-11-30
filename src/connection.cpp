//
// Created by volund on 11/26/21.
//

#include "connection.h"
#include "ringmud/session.h"

namespace dbat::conn {

    void cb_make_connection(uint64_t conn_id, entt::entity ent) {
        ring::core::registry.emplace<StateData>(ent);
    }

    void cb_create_connection(uint64_t conn_id, entt::entity ent) {
        // TODO: Display the welcome screen here!
    }

    void cb_close_connection(uint64_t conn_id, entt::entity ent) {
        // TODO: Notify and manage Session if necessary.
    }

    void cb_load_connection(uint64_t conn_id, entt::entity ent, nlohmann::json& j) {
        auto &sdata = ring::core::registry.get<StateData>(ent);
        sdata.state = j["state"];
        if(j.contains("user_id")) sdata.user_id = j["user_id"];
        if(j.contains("session")) sdata.session = j["session"];
    }

    void cb_save_connection(uint64_t conn_id, entt::entity ent, nlohmann::json& j) {
        auto &sdata = ring::core::registry.get<StateData>(ent);
        j["state"] = sdata.state;
        if(sdata.user_id.has_value()) j["user_id"] = sdata.user_id.value();
        if(sdata.session.has_value()) j["session"] = sdata.session.value();
    }

    void cb_handle_input(uint64_t conn_id, entt::entity ent, nlohmann::json& j) {
        auto &sdata = ring::core::registry.get<StateData>(ent);

        uint8_t mtype = j["msg_type"];
        if(mtype == 0) {
            auto jd = j["data"];
            std::string txt = jd["text"];
            switch(sdata.state) {
                case Login:
                    handle_cmd_login(conn_id, ent, txt);
                    break;
                case UserMenu:
                    handle_cmd_menu(conn_id, ent, txt);
                    break;
                case Session:
                    handle_cmd_session(conn_id, ent, txt);
                    break;
            }
        }
    }

    void handle_cmd_login(uint64_t conn_id, entt::entity ent, const std::string& txt) {

    }

    void handle_cmd_menu(uint64_t conn_id, entt::entity ent, const std::string& txt) {

    }

    void handle_cmd_session(uint64_t conn_id, entt::entity ent, const std::string& txt) {
        auto &sdata = ring::core::registry.get<StateData>(ent);
        if(!sdata.session.has_value()) return; // TODO: handle this actually being a thing...
        if(!ring::session::sessions.count(sdata.session.value())) return;
        auto sess_ent = ring::session::sessions[sdata.session.value()];
        auto &sessdata = ring::core::registry.get<ring::session::SessionData>(sess_ent);
        sessdata.input.push_back(txt);
    }

    void setup_cb() {
        ring::conn::on_make_connection = cb_make_connection;
        ring::conn::on_create_connection = cb_create_connection;
        ring::conn::on_close_connection = cb_close_connection;
        ring::conn::on_load_connection = cb_load_connection;
        ring::conn::on_save_connection = cb_save_connection;
        ring::conn::handle_input = cb_handle_input;
    }

}