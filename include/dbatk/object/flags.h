#pragma once
#include "dbatk/flags.h"

namespace dbat::oflags {

    enum OFlag : std::size_t {
        GLOBALROOMS = 0,
    };

    constexpr std::size_t NUM_OBJ_FLAGS = 1;

    class ObjectFlag : public BaseFlag {
    public:
        virtual void onLoad(const std::shared_ptr<Object>& obj) {};
        virtual void onSet(const std::shared_ptr<Object>& obj) {};
        virtual void onClear(const std::shared_ptr<Object>& obj) {};
    };

    class SimpleObjectFlag : public ObjectFlag {
    public:
        SimpleObjectFlag(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getID() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    extern const std::vector<std::shared_ptr<ObjectFlag>> objectFlags;
}