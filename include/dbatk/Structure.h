#pragma once
#include "dbatk/Object.h"

namespace kaizermud::dbat {

    class Structure : public Object {
    public:
        using Object::Object;
        [[nodiscard]] std::string_view getMainType() const override;
    };

}