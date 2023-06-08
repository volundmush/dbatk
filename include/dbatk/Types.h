#pragma once

#include "kaizermud/base/Types.h"

namespace dbat::types {
    struct Room : kaizer::base::Room {
        std::vector<std::string> getStatSlots() const override {return {"gravity"};};
    };

    void registerDBTypes();
}