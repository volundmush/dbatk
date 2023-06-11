#pragma once
#include "dbatk/base.h"

namespace dbat::size {
    enum SizeID {
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
        [[nodiscard]] virtual std::size_t getID() = 0;
        [[nodiscard]] virtual std::string_view getName() = 0;
    };

    struct Fine : Size {
        [[nodiscard]] std::size_t getID() override {return FINE;};
        [[nodiscard]] std::string_view getName() override {return "Fine";}
    };

    struct Diminutive : Size {
        [[nodiscard]] std::size_t getID() override {return DIMINUTIVE;};
        [[nodiscard]] std::string_view getName() override {return "Diminutive";}
    };

    struct Tiny : Size {
        [[nodiscard]] std::size_t getID() override {return TINY;};
        [[nodiscard]] std::string_view getName() override {return "Tiny";}
    };

    struct Small : Size {
        [[nodiscard]] std::size_t getID() override {return SMALL;};
        [[nodiscard]] std::string_view getName() override {return "Small";}
    };

    struct Medium : Size {
        [[nodiscard]] std::size_t getID() override {return MEDIUM;};
        [[nodiscard]] std::string_view getName() override {return "Medium";}
    };

    struct Large : Size {
        [[nodiscard]] std::size_t getID() override {return LARGE;};
        [[nodiscard]] std::string_view getName() override {return "Large";}
    };

    struct Huge : Size {
        [[nodiscard]] std::size_t getID() override {return HUGE;};
        [[nodiscard]] std::string_view getName() override {return "Huge";}
    };

    struct Gargantuan : Size {
        [[nodiscard]] std::size_t getID() override {return GARGANTUAN;};
        [[nodiscard]] std::string_view getName() override {return "Gargantuan";}
    };

    struct Colossal : Size {
        [[nodiscard]] std::size_t getID() override {return COLOSSAL;};
        [[nodiscard]] std::string_view getName() override {return "Colossal";}
    };

    extern const std::vector<std::shared_ptr<Size>> sizes;

}