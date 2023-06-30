#pragma once
#include "dbatk/base.h"

namespace dbat::size {
    enum SizeId {
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
        [[nodiscard]] virtual std::size_t getId() = 0;
        [[nodiscard]] virtual std::string getName() = 0;
    };

    struct Fine : Size {
        [[nodiscard]] std::size_t getId() override {return FINE;};
        [[nodiscard]] std::string getName() override {return "Fine";}
    };

    struct Diminutive : Size {
        [[nodiscard]] std::size_t getId() override {return DIMINUTIVE;};
        [[nodiscard]] std::string getName() override {return "Diminutive";}
    };

    struct Tiny : Size {
        [[nodiscard]] std::size_t getId() override {return TINY;};
        [[nodiscard]] std::string getName() override {return "Tiny";}
    };

    struct Small : Size {
        [[nodiscard]] std::size_t getId() override {return SMALL;};
        [[nodiscard]] std::string getName() override {return "Small";}
    };

    struct Medium : Size {
        [[nodiscard]] std::size_t getId() override {return MEDIUM;};
        [[nodiscard]] std::string getName() override {return "Medium";}
    };

    struct Large : Size {
        [[nodiscard]] std::size_t getId() override {return LARGE;};
        [[nodiscard]] std::string getName() override {return "Large";}
    };

    struct Huge : Size {
        [[nodiscard]] std::size_t getId() override {return HUGE;};
        [[nodiscard]] std::string getName() override {return "Huge";}
    };

    struct Gargantuan : Size {
        [[nodiscard]] std::size_t getId() override {return GARGANTUAN;};
        [[nodiscard]] std::string getName() override {return "Gargantuan";}
    };

    struct Colossal : Size {
        [[nodiscard]] std::size_t getId() override {return COLOSSAL;};
        [[nodiscard]] std::string getName() override {return "Colossal";}
    };

    extern const std::vector<std::shared_ptr<Size>> sizes;

}