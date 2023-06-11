#include "dbatk/character/character.h"

namespace dbat::sensei {

    OpResult<> Sensei::canSet(const std::shared_ptr<Character>& target) {
        if(target->getRaceID() == race::ANDROID) {
            return {false, "Androids really had best learn from Sixteen."};
        }
        return {true, std::nullopt};
    }

    OpResult<> Sixteen::canSet(const std::shared_ptr<Character>& target) {
        if(target->getRaceID() != race::ANDROID) {
            return {false, "Only Androids can be trained by Sixteen."};
        }
        return {true, std::nullopt};
    }

    OpResult<> Dabura::canSet(const std::shared_ptr<Character>& target) {
        if(target->getRaceID() != race::DEMON) {
            return {false, "Only Demons can be trained by Dabura."};
        }
        return {true, std::nullopt};
    }

    OpResult<> Jinto::canSet(const std::shared_ptr<Character>& target) {
        if(target->getRaceID() != race::HOSHIJIN) {
            return {false, "Jinto will only teach the secrets of the stars to Hoshijin."};
        }
        return {true, std::nullopt};
    }

    OpResult<> Tsuna::canSet(const std::shared_ptr<Character>& target) {
        if(target->getRaceID() != race::KANASSAN) {
            return {false, "These martial secrets are only taught to Kanassans."};
        }
        return {true, std::nullopt};
    }

    OpResult<> Kurzak::canSet(const std::shared_ptr<Character>& target) {
        if(target->getRaceID() != race::ARLIAN) {
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