#pragma once
#include "dbatk/base.h"

namespace dbat {

    enum class ColorType : uint8_t {
        NoColor = 0,
        Standard = 1,
        Xterm256 = 2,
        TrueColor = 3
    };

    std::string stripAnsi(std::string_view input);

    std::string renderAnsi(std::string_view input, ColorType color);

}