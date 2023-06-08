#include "dbatk/Sex.h"


namespace dbat::sex {

    std::vector<std::shared_ptr<Sex>> sexRegistry;

    void registerSex(const std::shared_ptr<Sex>& entry) {
        while(sexRegistry.size() < entry->getID() + 1) {
            sexRegistry.emplace_back(nullptr);
        }
        sexRegistry[entry->getID()] = entry;
    }

    void registerSexes() {
        registerSex(std::make_shared<Female>());
        registerSex(std::make_shared<Neuter>());
        registerSex(std::make_shared<Male>());
    }
}