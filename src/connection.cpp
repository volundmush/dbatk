//
// Created by volund on 11/26/21.
//

#include "connection.h"
#include "session.h"

namespace dbat::conn {

    std::map<uint64_t, entt::entity> connections;
    std::set<uint64_t> closing;

    ConnectionData::~ConnectionData() {
        if(conn.expired()) return;
        auto c = conn.lock();
        ring::net::manager.closeConn(c->conn_id);
    }

    void ConnectionData::process_input() {
        if(conn.expired()) return;
        auto c = conn.lock();

        c->in_queue_mutex.lock();
        input.splice(input.end(), c->queue_in);
        c->in_queue_mutex.unlock();

        if(input.empty())
            return;

        auto comm = input.front();


        input.pop_front();
    }

    void ConnectionData::send_text(const std::string &txt, ring::net::TextType mode) {
        if(conn.expired()) return;
        auto c = conn.lock();
        c->sendText(txt, mode);
    }

    bool ConnectionData::isColor() {
        if(conn.expired()) return false;
        auto c = conn.lock();
        return c->details.colorType > 0;
    }

    entt::entity make_connection(uint64_t conn_id) {
        auto ent = core::registry.create();
        auto wp = ring::net::manager.connections[conn_id];
        auto &cdata = core::registry.emplace<ConnectionData>(ent);
        cdata.entity = ent;
        cdata.conn_id = conn_id;
        cdata.conn = wp;
        conn::connections[conn_id] = ent;

        return ent;
    }

    entt::entity create_connection(uint64_t conn_id) {
        auto ent = make_connection(conn_id);

        return ent;
    }

    void close_connection(uint64_t conn_id) {
        auto ent = connections[conn_id];

        connections.erase(conn_id);
        core::registry.destroy(ent);
        closing.erase(conn_id);
    }

    nlohmann::json save_connection(uint64_t conn_id) {
        auto ent = connections[conn_id];
        nlohmann::json j;
        j["conn_id"] = conn_id;
        auto &sdata = core::registry.get<ConnectionData>(ent);
        j["state"] = sdata.state;
        if(sdata.user_id.has_value()) j["user_id"] = sdata.user_id.value();
        if(sdata.session.has_value()) j["session"] = sdata.session.value();
        return j;
    }

    nlohmann::json save_connections() {
        nlohmann::json j;
        for(auto &s : connections) {
            j[std::to_string(s.first)] = save_connection(s.first);
        }
        return j;
    }

    void load_connection(uint64_t conn_id, nlohmann::json& j) {
        auto ent = make_connection(conn_id);
        auto &sdata = core::registry.get<ConnectionData>(ent);
        sdata.state = j["state"];
        if(j.contains("user_id")) sdata.user_id = j["user_id"];
        if(j.contains("session")) sdata.session = j["session"];
    }

    void load_connections(nlohmann::json& j) {
        for(nlohmann::json::iterator it = j.begin(); it != j.end();it++) {
            uint64_t conn_id = std::strtoull(it.key().c_str(), nullptr, 10);
            load_connection(conn_id, it.value());
        }
    }

    void cb_handle_input(uint64_t conn_id, entt::entity ent, nlohmann::json& j) {
        auto &sdata = core::registry.get<ConnectionData>(ent);

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
        auto &sdata = core::registry.get<ConnectionData>(ent);
        if(!sdata.session.has_value()) return; // TODO: handle this actually being a thing...
        if(!session::sessions.count(sdata.session.value())) return;
        auto sess_ent = session::sessions[sdata.session.value()];
        auto &sessdata = core::registry.get<session::SessionData>(sess_ent);
        sessdata.input.push_back(txt);
    }

}