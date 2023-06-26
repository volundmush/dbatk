#pragma once
#include "dbatk/base.h"

namespace dbat {

    std::string stripAnsi(std::string_view input);

    std::string renderAnsi(std::string_view input, ColorType color);

}