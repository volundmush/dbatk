#pragma once
#include "dbatk/aspects/flags.h"

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

    class SimpleExitFlag : public ExitFlag {
    public:
        SimpleExitFlag(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    extern const std::vector<std::shared_ptr<ExitFlag>> exitFlags;

}