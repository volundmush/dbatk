#pragma once
#include "dbatk/base.h"

namespace dbat::oflags {

    enum OFlagId : uint8_t {
        GLOBALROOMS = 0,
    };

    constexpr std::size_t numObjFlags = 1;

    class ObjectFlag : public BaseFlag {
    public:
        virtual void onLoad(entt::entity ent) {};
        virtual void onSet(entt::entity ent) {};
        virtual void onClear(entt::entity ent) {};
    };

    extern const std::vector<std::shared_ptr<ObjectFlag>> objectFlags;
}