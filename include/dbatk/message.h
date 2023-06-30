#pragma once
#include "dbatk/base.h"

namespace dbat {

    enum class MsgTargetType {
        Room,
        Region,
        Direct
    };

    class MsgFormat {
    public:
        MsgFormat(entt::entity ent, const std::string &msg) : actor(ent), msg(msg) {};
        MsgFormat& str(const std::string& msg);
        MsgFormat& direct(entt::entity recipient);
        MsgFormat& region(entt::entity reg);
        MsgFormat& room(entt::entity room);
        MsgFormat& set(const std::string& name, entt::entity ent);
        void send();
        std::string msg;
        entt::entity actor{entt::null};
        std::vector<std::pair<MsgTargetType, entt::entity>> recipients;
        std::unordered_map<std::string, entt::entity> variables;
    protected:
        void sendTo(entt::entity recipient);
        std::function<bool(entt::entity)> filter;

        std::string callFunc(const std::string &func, const std::string &arg, entt::entity recipient, bool capitalized);

        std::string parseMessage(const std::string &txt, entt::entity recipient);

    };

}