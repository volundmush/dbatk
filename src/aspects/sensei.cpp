#include "dbatk/aspects/sensei.h"
#include "dbatk/aspects/race.h"
#include "dbatk/components.h"

namespace dbat::sensei {
    struct Untrained : Sensei {
        [[nodiscard]] std::size_t getId() override {return UNTRAINED;}
        [[nodiscard]] std::string getName() override {return "Untrained";}
        [[nodiscard]] std::string getStyleName() override {return "Like a Bum";}
        [[nodiscard]] RoomId getStartRoom() override {return 300;}
        [[nodiscard]] RoomId getLocation() override {return 300;}
    };

    struct Roshi : Sensei {
        [[nodiscard]] std::size_t getId() override {return ROSHI;}
        [[nodiscard]] std::string getName() override {return "Roshi";}
        [[nodiscard]] std::string getStyleName() override {return "Kame Arts";}
        [[nodiscard]] RoomId getStartRoom() override {return 1130;}
        [[nodiscard]] RoomId getLocation() override {return 1131;}
    };

    struct Piccolo : Sensei {
        [[nodiscard]] std::size_t getId() override {return PICCOLO;}
        [[nodiscard]] std::string getName() override {return "Piccolo";}
        [[nodiscard]] std::string getStyleName() override {return "Demon Taijutsu";}
        [[nodiscard]] RoomId getStartRoom() override {return 1659;}
        [[nodiscard]] RoomId getLocation() override {return 1662;}
    };

    struct Krane : Sensei {
        [[nodiscard]] std::size_t getId() override {return KRANE;}
        [[nodiscard]] std::string getName() override {return "Krane";}
        [[nodiscard]] std::string getStyleName() override {return "Crane Arts";}
        [[nodiscard]] RoomId getStartRoom() override {return 13009;}
        [[nodiscard]] RoomId getLocation() override {return 13012;}
    };

    struct Nail : Sensei {
        [[nodiscard]] std::size_t getId() override {return NAIL;}
        [[nodiscard]] std::string getName() override {return "Nail";}
        [[nodiscard]] std::string getStyleName() override {return "Tranquil Palm";}
        [[nodiscard]] RoomId getStartRoom() override {return 11683;}
        [[nodiscard]] RoomId getLocation() override {return 11683;}
    };

    struct Bardock : Sensei {
        [[nodiscard]] std::size_t getId() override {return BARDOCK;}
        [[nodiscard]] std::string getName() override {return "Bardock";}
        [[nodiscard]] std::string getStyleName() override {return "Brutal Beast";}
        [[nodiscard]] RoomId getStartRoom() override {return 2268;}
        [[nodiscard]] RoomId getLocation() override {return 2267;}
    };

    struct Ginyu : Sensei {
        [[nodiscard]] std::size_t getId() override {return GINYU;}
        [[nodiscard]] std::string getName() override {return "Ginyu";}
        [[nodiscard]] std::string getStyleName() override {return "Flaunted Style";}
        [[nodiscard]] RoomId getStartRoom() override {return 4289;}
        [[nodiscard]] RoomId getLocation() override {return 4290;}
    };

    struct Frieza : Sensei {
        [[nodiscard]] std::size_t getId() override {return FRIEZA;}
        [[nodiscard]] std::string getName() override {return "Frieza";}
        [[nodiscard]] std::string getStyleName() override {return "Frozen Fist";}
        [[nodiscard]] RoomId getStartRoom() override {return 4282;}
        [[nodiscard]] RoomId getLocation() override {return 4283;}
    };

    struct Tapion : Sensei {
        [[nodiscard]] std::size_t getId() override {return TAPION;}
        [[nodiscard]] std::string getName() override {return "Tapion";}
        [[nodiscard]] std::string getStyleName() override {return "Shadow Grappling";}
        [[nodiscard]] RoomId getStartRoom() override {return 8231;}
        [[nodiscard]] RoomId getLocation() override {return 8233;}
    };

    struct Sixteen : Sensei {
        [[nodiscard]] std::size_t getId() override {return SIXTEEN;}
        [[nodiscard]] std::string getName() override {return "Sixteen";}
        [[nodiscard]] std::string getAbbreviation() override {return "16";}
        [[nodiscard]] std::string getStyleName() override {return "Iron Hand";}
        [[nodiscard]] RoomId getStartRoom() override {return 1713;}
        [[nodiscard]] RoomId getLocation() override {return 1714;}
        [[nodiscard]] OpResult<> canSet(entt::entity target) override;
    };

    struct Dabura : Sensei {
        [[nodiscard]] std::size_t getId() override {return DABURA;}
        [[nodiscard]] std::string getName() override {return "Dabura";}
        [[nodiscard]] std::string getStyleName() override {return "Devil Dance";}
        [[nodiscard]] RoomId getStartRoom() override {return 6487;}
        [[nodiscard]] RoomId getLocation() override {return 6486;}
        [[nodiscard]] OpResult<> canSet(entt::entity target) override;
    };

    struct Kibito : Sensei {
        [[nodiscard]] std::size_t getId() override {return KIBITO;}
        [[nodiscard]] std::string getName() override {return "Kibito";}
        [[nodiscard]] std::string getStyleName() override {return "Gentle Fist";}
        [[nodiscard]] RoomId getStartRoom() override {return 12098;}
        [[nodiscard]] RoomId getLocation() override {return 12098;}
    };

    struct Jinto : Sensei {
        [[nodiscard]] std::size_t getId() override {return JINTO;}
        [[nodiscard]] std::string getName() override {return "Jinto";}
        [[nodiscard]] std::string getStyleName() override {return "Star's Radiance";}
        [[nodiscard]] RoomId getStartRoom() override {return 3499;}
        [[nodiscard]] RoomId getLocation() override {return 3499;}
        [[nodiscard]] OpResult<> canSet(entt::entity target) override;
    };

    struct Tsuna : Sensei {
        [[nodiscard]] std::size_t getId() override {return TSUNA;}
        [[nodiscard]] std::string getName() override {return "Tsuna";}
        [[nodiscard]] std::string getStyleName() override {return "Sacred Tsunami";}
        [[nodiscard]] RoomId getStartRoom() override {return 15009;}
        [[nodiscard]] RoomId getLocation() override {return 15009;}
        [[nodiscard]] OpResult<> canSet(entt::entity target) override;
    };

    struct Kurzak : Sensei {
        [[nodiscard]] std::size_t getId() override {return KURZAK;}
        [[nodiscard]] std::string getName() override {return "Kurzak";}
        [[nodiscard]] std::string getStyleName() override {return "Adaptive Taijutsu";}
        [[nodiscard]] RoomId getStartRoom() override {return 16100;}
        [[nodiscard]] RoomId getLocation() override {return 16100;}
        [[nodiscard]] OpResult<> canSet(entt::entity target) override;
    };

    OpResult<> Sensei::canSet(entt::entity target) {
        auto &r = registry.get_or_emplace<Race>(target);
        if(r.data == race::ANDROID) {
            return {false, "Androids really had best learn from Sixteen."};
        }
        return {true, std::nullopt};
    }

    OpResult<> Sixteen::canSet(entt::entity target) {
        auto &r = registry.get_or_emplace<Race>(target);
        if(r.data != race::ANDROID) {
            return {false, "Only Androids can be trained by Sixteen."};
        }
        return {true, std::nullopt};
    }

    OpResult<> Dabura::canSet(entt::entity target) {
        auto &r = registry.get_or_emplace<Race>(target);
        if(r.data != race::DEMON) {
            return {false, "Only Demons can be trained by Dabura."};
        }
        return {true, std::nullopt};
    }

    OpResult<> Jinto::canSet(entt::entity target) {
        auto &r = registry.get_or_emplace<Race>(target);
        if(r.data != race::HOSHIJIN) {
            return {false, "Jinto will only teach the secrets of the stars to Hoshijin."};
        }
        return {true, std::nullopt};
    }

    OpResult<> Tsuna::canSet(entt::entity target) {
        auto &r = registry.get_or_emplace<Race>(target);
        if(r.data != race::KANASSAN) {
            return {false, "These martial secrets are only taught to Kanassans."};
        }
        return {true, std::nullopt};
    }

    OpResult<> Kurzak::canSet(entt::entity target) {
        auto &r = registry.get_or_emplace<Race>(target);
        if(r.data != race::ARLIAN) {
            return {false, "Only the people of Arlia may learn these arts."};
        }
        return {true, std::nullopt};
    }

    std::vector<std::shared_ptr<Sensei>> senseis = {
        std::make_shared<Untrained>(),
        std::make_shared<Roshi>(),
        std::make_shared<Piccolo>(),
        std::make_shared<Krane>(),
        std::make_shared<Nail>(),
        std::make_shared<Bardock>(),
        std::make_shared<Ginyu>(),
        std::make_shared<Frieza>(),
        std::make_shared<Tapion>(),
        std::make_shared<Sixteen>(),
        std::make_shared<Dabura>(),
        std::make_shared<Kibito>(),
        std::make_shared<Jinto>(),
        std::make_shared<Tsuna>(),
        std::make_shared<Kurzak>()
    };

    Sensei* findSensei(std::size_t id, std::function<bool(std::shared_ptr<Sensei>)> predicate) {
        // First, if the id is not within the range of races, return nullptr.
        if(id > senseis.size() - 1) {
            return nullptr;
        }
        auto r = senseis[id];
        // check predicate...
        if(predicate(r)) {
            return r.get();
        }
        return nullptr;
    }

    Sensei* findSensei(const std::string &txt, std::function<bool(std::shared_ptr<Sensei>)> predicate) {
        std::vector<std::shared_ptr<Sensei>> filtered;
        std::copy_if(senseis.begin(), senseis.end(), std::back_inserter(filtered), predicate);
        auto res = partialMatch(txt, filtered.begin(), filtered.end(), false, [](auto r) { return r->getName(); });
        return res == filtered.end() ? nullptr : (*res).get();
    }



}