//
// Created by volund on 11/27/21.
//

#include "core.h"
#include "connection.h"
#include "session.h"
#include "dgscript.h"
#include "item.h"
#include "mobile.h"
#include "zone.h"
#include "room.h"

namespace dbat::core {
    entt::registry registry;
    std::mutex new_conn_mutex, closed_conn_mutex, ready_conn_mutex;
    std::set<int> new_connections, closed_connections, ready_connections;
    bool copyover_execute = false, sleeping = true, had_connections = false;
    bool copyover_after = false;
    asio::high_resolution_timer *loop_timer;
    std::chrono::milliseconds interval = asio::chrono::milliseconds(100);
    uint64_t pulse = 0;
    fs::path profile_path = fs::current_path();

    void load_db() {
        auto zpath = profile_path / "zones";
        if(fs::exists(zpath) && fs::is_directory(zpath))
            throw std::runtime_error("Profile Path has no Zones folder. World is empty!");
        zone::load_all(zpath);
    }

    void connection_ready(int conn_id) {
        new_conn_mutex.lock();
        new_connections.insert(conn_id);
        new_conn_mutex.unlock();
    }

    void connection_closed(int conn_id) {
        closed_conn_mutex.lock();
        closed_connections.insert(conn_id);
        closed_conn_mutex.unlock();
    }

    void connection_hasdata(int conn_id) {
        ready_conn_mutex.lock();
        ready_connections.insert(conn_id);
        ready_conn_mutex.unlock();
    }

    void game_loop(const std::string &host, uint16_t telnet_port) {
        ring::net::on_receive_cb = connection_hasdata;
        ring::net::on_ready_cb = connection_ready;
        ring::net::on_close_cb = connection_closed;

        // create timer here so it can link to ringnet's executor.
        loop_timer = new asio::high_resolution_timer(*ring::net::executor);
        loop_timer->expires_after(std::chrono::milliseconds(0));

        if(copyover_after) {
            copyover_recover();
        } else {
            if(!ring::net::manager.listenPlainTelnet(host, telnet_port)) {
                //log("Error! Cannot bind to socket!");
                exit(1);
            }
        }

        if(!copyover_after && !ring::net::manager.listenPlainTelnet(host, telnet_port)) {

        }

        loop_timer->async_wait([](auto ec){if(!ec) clock_loop();});

        ring::net::manager.run(-1);

        //log("Main loop finished.");

        // moved this logic here so that copyovers will wait until all threads have joined.
        if(copyover_execute) {
            auto cpath = profile_path / "copyover.json";
            std::ofstream jf(cpath);
            if(!jf.is_open())
                throw std::runtime_error("Cannot copyover - cannot open copyover.json");

            auto cj = ring::net::manager.copyover();
            if(!session::sessions.empty())
                cj["sessions"] = session::save_sessions();

            if(!conn::connections.empty())
                cj["game_connections"] = conn::save_connections();


            exit(1); /* too much trouble to try to recover! */
        }
    }

    void clock_loop() {

        auto start = std::chrono::high_resolution_clock::now();
        network_loop();

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        if(duration < interval) {
            loop_timer->expires_after(interval - duration);
        } else {
            //log("Main loop struggling to keep up!");
        }
        /* Update tics for deadlock protection */
        //tics_passed++;

        loop_timer->async_wait([](auto ec){if(!ec) clock_loop();});

    }

    void network_loop() {
        /* The Main Loop.  The Big Cheese.  The Top Dog.  The Head Honcho.  The.. */

        /* If there are new connections waiting, accept them. */
        new_conn_mutex.lock();
        ring::net::manager.conn_mutex.lock();
        auto new_ids = new_connections;
        new_connections.clear();
        new_conn_mutex.unlock();

        for(uint64_t conn_id : new_ids) {
            conn::create_connection(conn_id);
        }
        ring::net::manager.conn_mutex.unlock();

        if(!conn::connections.empty()) {
            if(!had_connections) {
                had_connections = true;
                //log("Got a new connection - waking up...");
                sleeping = false;
            }
        } else {
            if(had_connections) {
                //log("No connections.  Going to sleep.");
                had_connections = false;
                sleeping = true;
            }
            return;
        }

        /* Kick out the freaky folks in the exception set and marked for close */
        closed_conn_mutex.lock();
        auto closed_ids = closed_connections;
        closed_connections.clear();
        closed_conn_mutex.unlock();

        if(!closed_ids.empty()) {
            for(auto conn_id : closed_ids) {
                conn::close_connection(conn_id);
            }
        }

        /* Process connections with input pending */
        ready_conn_mutex.lock();
        auto ready_ids = ready_connections;
        ready_connections.clear();
        ready_conn_mutex.unlock();

        if(!ready_ids.empty()) {
            for(auto conn_id : ready_ids) {
                auto conn_ent = conn::connections[conn_id];
                if(!registry.valid(conn_ent)) continue;
                auto &cdata = registry.get<conn::ConnectionData>(conn_ent);
                cdata.process_input();
            }
        }

        /* Process sessions we just read from process_input */
        for (auto &s : session::sessions) {
            if(!registry.valid(s.second)) continue;
            auto &sdata = registry.get<session::SessionData>(s.second);
            sdata.process_input();
        }

        if(!sleeping)
            //on_heartbeat(++pulse);

        /* Send queued output out to the operating system (ultimately to user). */
        for (auto &s : session::sessions) {
            if(!registry.valid(s.second)) continue;
            auto &sdata = registry.get<session::SessionData>(s.second);
            sdata.process_output();
        }

        /* Kick out folks in the CON_CLOSE or CON_DISCONNECT state */
        for(auto c : conn::closing) {
            conn::close_connection(c);
        }
    }

    void copyover_recover() {
        auto cpath = profile_path / "copyover.json";

        if(!fs::exists(cpath)) {
            perror("copyover_recover: file not exists");
            //log("Copyover file not found. Exitting.\n\r");
            exit(1);
        }

        std::ifstream jf(cpath.string());
        if (!jf.is_open()) {
            perror("copyover_recover:fopen");
            //log("Copyover file not found. Exitting.\n\r");
            exit(1);
        }

        nlohmann::json j;
        jf >> j;
        jf.close();

        unlink(cpath.c_str());

        int pid = j["pid"];
        if(pid != getpid()) {
            //log("Copyover file does not have matching PID. Exitting...");
            exit(1);
        }

        // This will rebuild the networking.
        ring::net::manager.copyoverRecover(j);

        // reload sessions.
        if(j.count("sessions"))
            session::load_sessions(j["sessions"]);

        // rebuild game connections
        if(j.count("game_connections"))
            conn::load_connections(j["game_connections"]);

    }

    void init() {
        core::load_db();
    }

    void start(const std::string& hostname, uint16_t port) {
        init();
        core::game_loop(hostname, port);
    }

}