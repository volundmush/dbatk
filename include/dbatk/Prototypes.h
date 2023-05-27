#pragma once
#include "kaizermud/base.h"
#include <set>
#include <map>
#include <boost/json.hpp>

namespace kaizermud::dbat {

    class Prototype {
    public:
        ObjectID vnum;
        std::set<std::shared_ptr<game::Object>> instances;
        std::optional<boost::json::object> data;

        std::pair<std::shared_ptr<game::Object>, std::string> spawn();

    };

    std::map<ObjectID, Prototype> npcs{}, items{};
}