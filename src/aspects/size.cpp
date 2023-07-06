#include "dbatk/aspects/size.h"

namespace dbat::size {

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