#include "dbatk/Sex.h"


namespace dbat::sex {
    void registerSexes() {
        registerAspect(std::make_shared<Male>());
        registerAspect(std::make_shared<Female>());
        registerAspect(std::make_shared<Neuter>());
    }
}