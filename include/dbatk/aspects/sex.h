#pragma once
#include "dbatk/base.h"

namespace dbat::sex {
    enum SexId : uint8_t {
        NEUTER = 0,
        MALE = 1,
        FEMALE = 2,
        PLURAL = 3
    };

    struct Sex {
        [[nodiscard]] virtual std::size_t getId() = 0;
        [[nodiscard]] virtual std::string getName() = 0;
    };

    Sex* findSex(std::size_t id, std::function<bool(std::shared_ptr<Sex>)> predicate = [](std::shared_ptr<Sex>) { return true; });
    Sex* findSex(const std::string &txt, std::function<bool(std::shared_ptr<Sex>)> predicate = [](std::shared_ptr<Sex>) { return true; });

    extern const std::vector<std::shared_ptr<Sex>> sexes;
}