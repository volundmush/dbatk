#pragma once
#include "kaizermud/base.h"
#include <set>
#include <map>
#include <boost/json.hpp>

namespace kaizermud::dbat {

    class Prototype {
    public:
        ObjectID vnum;
        std::set<entt::entity> instances;
        std::optional<boost::json::object> data;

        std::pair<entt::entity, std::string> spawn();

    };

    std::map<ObjectID, Prototype> npcs{}, items{};
}