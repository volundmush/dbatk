#pragma once
#include "dbatk/aspects/flags.h"

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

    class SimpleObjectFlag : public ObjectFlag {
    public:
        SimpleObjectFlag(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    extern const std::vector<std::shared_ptr<ObjectFlag>> objectFlags;
}