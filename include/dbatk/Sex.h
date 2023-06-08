#pragma once
#include "kaizermud/base.h"

namespace dbat::sex {
    struct Sex {
        [[nodiscard]] virtual std::size_t getID() = 0;
        [[nodiscard]] virtual std::string_view getName() = 0;
    };

    struct Neuter : Sex {
        [[nodiscard]] std::size_t getID() override {return 0;};
        [[nodiscard]] std::string_view getName() override {return "Neuter";}
    };

    struct Male : Sex {
        [[nodiscard]] std::size_t getID() override {return 1;};
        [[nodiscard]] std::string_view getName() override {return "Male";}
    };

    struct Female : Sex {
        [[nodiscard]] std::size_t getID() override {return 2;};
        [[nodiscard]] std::string_view getName() override {return "Female";}
    };

    extern std::vector<std::shared_ptr<Sex>> sexRegistry;

    void registerSex(const std::shared_ptr<Sex>& entry);
    void registerSexes();
}