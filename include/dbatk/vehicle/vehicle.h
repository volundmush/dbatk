#pragma once
#include "dbatk/object/object.h"

namespace dbat {
    class Vehicle : public Object {
    public:
        using Object::Object;
        ~Vehicle() override = default;
        std::string getTypeName() const override { return "vehicle"; };
    };
}