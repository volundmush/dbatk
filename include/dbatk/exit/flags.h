#pragma once
#include "dbatk/flags.h"

namespace dbat::eflags {

    enum EFlag : std::size_t {
        ISDOOR = 0,
        CLOSED = 1,
        LOCKED = 2,
        PICKPROOF = 3,
        SECRET = 4
    };

    constexpr std::size_t NUM_EXIT_FLAGS = 5;


    class ExitFlag : public BaseFlag {
    public:
        virtual void onLoad(Exit* ex) {};
        virtual void onSet(Exit* ex) {};
        virtual void onClear(Exit* ex) {};
    };

    class SimpleExitFlag : public ExitFlag {
    public:
        SimpleExitFlag(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getID() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    extern const std::vector<std::shared_ptr<ExitFlag>> exitFlags;

}