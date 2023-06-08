#pragma once

namespace dbat::position {
    struct Position {
        [[nodiscard]] virtual std::size_t getID() = 0;
        [[nodiscard]] virtual std::string_view getName() = 0;
    };

    struct Dead : Position {
        [[nodiscard]] std::size_t getID() override {return 0;};
        [[nodiscard]] std::string_view getName() override {return "Dead";}
    };

    struct MortallyWounded : Position {
        [[nodiscard]] std::size_t getID() override {return 1;};
        [[nodiscard]] std::string_view getName() override {return "Mortally Wounded";}
    };

    struct Incapacitated : Position {
        [[nodiscard]] std::size_t getID() override {return 2;};
        [[nodiscard]] std::string_view getName() override {return "Incapacitated";}
    };

    struct Stunned : Position {
        [[nodiscard]] std::size_t getID() override {return 3;};
        [[nodiscard]] std::string_view getName() override {return "Stunned";}
    };

    struct Sleeping : Position {
        [[nodiscard]] std::size_t getID() override {return 4;};
        [[nodiscard]] std::string_view getName() override {return "Sleeping";}
    };

    struct Resting : Position {
        [[nodiscard]] std::size_t getID() override {return 5;};
        [[nodiscard]] std::string_view getName() override {return "Resting";}
    };

    struct Sitting : Position {
        [[nodiscard]] std::size_t getID() override {return 6;};
        [[nodiscard]] std::string_view getName() override {return "Sitting";}
    };

    struct Fighting : Position {
        [[nodiscard]] std::size_t getID() override {return 7;};
        [[nodiscard]] std::string_view getName() override {return "Fighting";}
    };

    struct Standing : Position {
        [[nodiscard]] std::size_t getID() override {return 8;};
        [[nodiscard]] std::string_view getName() override {return "Standing";}
    };
}