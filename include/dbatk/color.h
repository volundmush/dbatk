#pragma once
#include "dbatk/base.h"

namespace dbat {

    std::string renderAnsiColor(std::string_view input, ColorType color);

    std::string stripAnsi(std::string_view input);

    extern std::function<std::string(std::string_view, ColorType)> renderAnsi;

}