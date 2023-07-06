#pragma once
#include "dbatk/base.h"

namespace dbat::size {
    enum SizeId {
        FINE = 0,
        DIMINUTIVE = 1,
        TINY = 2,
        SMALL = 3,
        MEDIUM = 4,
        LARGE = 5,
        HUGE = 6,
        GARGANTUAN = 7,
        COLOSSAL = 8
    };

    struct Size {
        [[nodiscard]] virtual std::size_t getId() = 0;
        [[nodiscard]] virtual std::string getName() = 0;
    };

    extern const std::vector<std::shared_ptr<Size>> sizes;

}