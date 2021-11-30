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

    void cb_copyover_execute() {

    }

    void cb_copyover_fail() {

    }

    void cb_load_db() {

    }

    void cb_copyover_save(nlohmann::json& j) {

    }

    void cb_copyover_recover(nlohmann::json& j) {

    }

    void cb_heartbeat(uint64_t beat) {

    }

    void setup_cb() {
        //ringmud core
        ring::core::on_copyover_execute = cb_copyover_execute;
        ring::core::on_copyover_fail = cb_copyover_fail;
        ring::core::on_copyover_save = cb_copyover_save;
        ring::core::on_copyover_recover = cb_copyover_recover;
        ring::core::on_heartbeat = cb_heartbeat;
        ring::core::on_load_db = cb_load_db;

        dbat::zone::setup_cb();
        dbat::conn::setup_cb();
        dbat::session::setup_cb();
        dbat::dgscript::setup_cb();
        dbat::room::setup_cb();
        dbat::item::setup_cb();
        dbat::mobile::setup_cb();
    }

    void init() {
        setup_cb();
        ring::core::load_db();
    }

    void start(const std::string& hostname, uint16_t port) {
        init();
        ring::core::game_loop(hostname, port);
    }

}