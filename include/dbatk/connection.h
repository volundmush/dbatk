#pragma once
#include "dbatk/base.h"
#include "core/connection.h"
#include "core/link.h"

namespace dbat {
    class DBATConnection : public core::Connection {
    public:
        using core::Connection::Connection;
        void onWelcome() override;
    };

    std::shared_ptr<core::Connection> makeClientConnection(int64_t conn_id, JsonChannel chan);


}