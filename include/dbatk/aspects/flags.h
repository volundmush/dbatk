#pragma once
#include "dbatk/base.h"

namespace dbat {
    class BaseFlag {
    public:
        [[nodiscard]] virtual std::size_t getId() const = 0;
        [[nodiscard]] virtual std::string getName() const = 0;
    };

}