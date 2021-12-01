//
// Created by volund on 11/26/21.
//

#ifndef DBATK_CONNECTION_H
#define DBATK_CONNECTION_H

#include "core.h"
#include "ringnet/net.h"

namespace dbat::conn {

    extern std::map<uint64_t, entt::entity> connections;
    extern std::set<uint64_t> closing;

    enum ConnState : uint8_t {
        Login = 0,
        UserMenu = 1,
        Session = 2
    };

    struct ConnectionData {
        ~ConnectionData();
        std::weak_ptr<ring::net::connection_details> conn;
        std::list<nlohmann::json> input;
        entt::entity entity;
        uint64_t conn_id;
        ConnState state = Login;
        opt_type<int64_t> user_id;
        opt_type<uint64_t> session;

        void process_input();
        void send_text(const std::string& txt, ring::net::TextType mode);
        bool isColor();
    };

    entt::entity make_connection(uint64_t conn_id);

    entt::entity create_connection(uint64_t conn_id);

    void close_connection(uint64_t conn_id);

    nlohmann::json save_connection(uint64_t conn_id);

    nlohmann::json save_connections();

    void load_connection(uint64_t conn_id, nlohmann::json &j);

    void load_connections(nlohmann::json& j);

    void handle_cmd_login(uint64_t conn_id, entt::entity ent, const std::string& txt);

    void handle_cmd_menu(uint64_t conn_id, entt::entity ent, const std::string& txt);

    void handle_cmd_session(uint64_t conn_id, entt::entity ent, const std::string& txt);

}

#endif //DBATK_CONNECTION_H
