#pragma once
#include "dbatk/base.h"

namespace dbat {
    class Session : public std::enable_shared_from_this<Session> {
    public:

        void sendText(const std::string& txt);
        void sendLine(const std::string& txt);
    protected:
        std::unordered_map<int64_t, std::shared_ptr<DBATConnection>> connections;
        entt::entity player;
    };
}