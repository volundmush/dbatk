#pragma once
#include "dbatk/base.h"

namespace dbat {

    enum class MsgTargetType {
        Room,
        Region,
        Direct
    };

    struct Location;

    class MsgFormat {
    public:
        MsgFormat(entt::entity ent, const std::string &msg);
        MsgFormat& str(const std::string& msg);
        MsgFormat& direct(entt::entity recipient);
        MsgFormat& region(entt::entity reg);
        MsgFormat& set(const std::string& name, entt::entity ent);
        MsgFormat& exclude(entt::entity ent);
        MsgFormat& in(Location loc);
        void send();
        std::string msg;
        entt::entity actor{entt::null};
        std::vector<entt::entity> regions;
        std::vector<Location> recipients;
        std::set<entt::entity> toExclude;
        std::unordered_map<std::string, entt::entity> variables;
    protected:
        void sendTo(entt::entity recipient);
        std::function<bool(entt::entity)> filter;

        std::string callFunc(const std::string &func, const std::string &arg, entt::entity recipient, bool capitalized);

        std::string parseMessage(const std::string &txt, entt::entity recipient);

    };

}