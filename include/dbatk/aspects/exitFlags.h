#pragma once
#include "dbatk/base.h"

namespace dbat::eflags {

    enum EFlagId : uint8_t {
        ISDOOR = 0,
        CLOSED = 1,
        LOCKED = 2,
        PICKPROOF = 3,
        SECRET = 4
    };

    constexpr std::size_t numExitFlags = 5;


    class ExitFlag : public BaseFlag {
    public:
        virtual void onLoad(entt::entity ent) {};
        virtual void onSet(entt::entity ent) {};
        virtual void onClear(entt::entity ent) {};
    };



    extern const std::vector<std::shared_ptr<ExitFlag>> exitFlags;

}