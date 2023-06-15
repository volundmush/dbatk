#pragma once
#include "dbatk/object/object.h"

namespace dbat {
    class Area : public Object {
    public:
        using Object::Object;
        ~Area() override = default;
        std::string getTypeName() const override { return "area"; };
    };
}