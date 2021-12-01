//
// Created by volund on 12/1/21.
//

#ifndef DBATK_SHARED_H
#define DBATK_SHARED_H

#include "sysdep.h"

namespace dbat::shared {

    struct ExDesc {
        std::string keyword, description;
    };

    enum VnumType : uint8_t {
        Zone = 0,
        DgScript = 1,
        Room = 2,
        Item = 3,
        Mobile = 4,
        Shop = 5,
        Guild = 6
    };

    struct EntityData {
        vnum vn;
        VnumType vtype;
        entt::entity entity;
        std::string name;
        vnum zone;
        nlohmann::json serialize();
    };

}

#endif //DBATK_SHARED_H
