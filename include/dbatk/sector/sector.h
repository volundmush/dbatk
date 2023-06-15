#pragma once
#include "dbatk/object/object.h"

namespace dbat {
    class Sector : public Object {
    public:
        using Object::Object;
        ~Sector() override = default;
        std::string getTypeName() const override { return "sector"; };
    };
}