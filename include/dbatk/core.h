//
// Created by volund on 11/27/21.
//

#ifndef DBATK_CORE_H
#define DBATK_CORE_H

#include "sysdep.h"
#include "ringmud/core.h"

namespace dbat::core {

    void cb_copyover_execute();
    void cb_copyover_fail();
    void cb_load_db();

    void cb_copyover_save(nlohmann::json& j);
    void cb_copyover_recover(nlohmann::json& j);

    void cb_heartbeat(uint64_t beat);

    void setup_cb();

}

#endif //DBATK_CORE_H
