//
// Created by volund on 11/27/21.
//

#ifndef DBATK_CORE_H
#define DBATK_CORE_H

#include "sysdep.h"
#include <mutex>
#include "asio.hpp"


namespace dbat::core {

    extern entt::registry registry;
    extern std::set<int> new_connections, closed_connections, ready_connections;
    extern std::mutex new_conn_mutex, closed_conn_mutex, ready_conn_mutex;
    extern bool copyover_execute, sleeping, had_connections, copyover_after;
    extern asio::high_resolution_timer *loop_timer;
    extern fs::path profile_path;
    extern std::chrono::milliseconds interval;
    extern uint64_t pulse;

    void connection_ready(int conn_id);
    void connection_closed(int conn_id);
    void connection_hasdata(int conn_id);

    void load_db();
    void clock_loop();
    void network_loop();
    void game_loop(const std::string &host, uint16_t telnet_port);

    void copyover_recover();

    void cb_copyover_execute();
    void cb_copyover_fail();
    void cb_load_db();

    void cb_copyover_save(nlohmann::json& j);
    void cb_copyover_recover(nlohmann::json& j);

    void cb_heartbeat(uint64_t beat);

    void setup_cb();

    void init();

    void start(const std::string& hostname, uint16_t port);

}

#endif //DBATK_CORE_H
