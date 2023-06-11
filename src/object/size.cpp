#include "dbatk/object/size.h"

namespace dbat::size {

    const std::vector<std::shared_ptr<Size>> sizes = {
            std::make_shared<Fine>(),
            std::make_shared<Diminutive>(),
            std::make_shared<Tiny>(),
            std::make_shared<Small>(),
            std::make_shared<Medium>(),
            std::make_shared<Large>(),
            std::make_shared<Huge>(),
            std::make_shared<Gargantuan>(),
            std::make_shared<Colossal>()
    };

}