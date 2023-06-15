#pragma once
#include "dbatk/object/object.h"

namespace dbat {
    class Dimension : public Object {
    public:
        using Object::Object;
        ~Dimension() override = default;
        std::string getTypeName() const override { return "dimension"; };
    };
}