#pragma once

#include "dbatk/base.h"

namespace dbat::components {

    // used to flag that something needs to be saved.
    struct Dirty {

    };

    struct _StringView {
        std::string_view data;
        std::string_view clean;
        void setData(const std::string& txt);
    };

    struct _String {
        std::string data;
        std::string clean;
        void setData(const std::string& txt);
    };

    struct Name : _StringView {};
    struct ShortDescription : _StringView {};
    struct RoomDescription : _StringView {};
    struct LookDescription : _StringView {};

    struct _Entity {
        entt::entity data;
    };

    struct _ReverseEntity {
        std::vector<entt::entity> data;
    };

    struct Location : _Entity {};
    struct Contents : _ReverseEntity {};
    struct Parent : _Entity {};
    struct Children : _ReverseEntity {};
    struct Owner : _Entity {};
    struct Assets : _ReverseEntity {};





}