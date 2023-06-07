#pragma once

#include "kaizermud/ClientConnection.h"
#include "kaizermud/thermite.h"

namespace dbat {
    class Connection : public kaizer::ClientConnection {
    public:
        using kaizer::ClientConnection::ClientConnection;
        void onWelcome() override;
    };

    std::shared_ptr<kaizer::ClientConnection> makeClientConnection(kaizer::LinkManager& lm, uint64_t conn_id, kaizer::mpmc_channel<boost::json::value> chan);

}