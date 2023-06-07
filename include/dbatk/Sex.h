#pragma once

#include "dbatk/Aspects.h"

namespace dbat::sex {
    struct Sex : dbat::aspect::DBATAspect {
        [[nodiscard]] std::string_view getSlot() override {return "sex";}
        [[nodiscard]] virtual std::string_view getName() = 0;
    };

    struct Male : Sex {
        [[nodiscard]] std::string_view getName() override {return "Male";}
        [[nodiscard]] std::string_view getKey() override {return "male";}
    };

    struct Female : Sex {
        [[nodiscard]] std::string_view getName() override {return "Female";}
        [[nodiscard]] std::string_view getKey() override {return "female";}
    };

    struct Neuter : Sex {
        [[nodiscard]] std::string_view getName() override {return "Neuter";}
        [[nodiscard]] std::string_view getKey() override {return "neuter";}
    };

    void registerSexes();
}