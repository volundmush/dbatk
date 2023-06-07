#include "kaizermud/Api.h"
#include "dbatk/Senseis.h"

namespace dbat::sensei {

    kaizer::OpResult<> Sensei::canSet(entt::entity ent) {
        auto race = kaizer::getAspect(ent, "race");
        if(race->getKey() == "android") {
            return {false, "Androids really had best learn from Sixteen."};
        }
        return {true, std::nullopt};
    }

    kaizer::OpResult<> Sixteen::canSet(entt::entity ent) {
        auto race = kaizer::getAspect(ent, "race");
        if(race->getKey() != "android") {
            return {false, "Only Androids can be trained by Sixteen."};
        }
        return {true, std::nullopt};
    }

    kaizer::OpResult<> Dabura::canSet(entt::entity ent) {
        auto race = kaizer::getAspect(ent, "race");
        if(race->getKey() != "demon") {
            return {false, "Only Demons can be trained by Dabura."};
        }
        return {true, std::nullopt};
    }

    kaizer::OpResult<> Jinto::canSet(entt::entity ent) {
        auto race = kaizer::getAspect(ent, "race");
        if(race->getKey() != "hoshijin") {
            return {false, "Jinto will only teach the secrets of the stars to Hoshijin."};
        }
        return {true, std::nullopt};
    }

    kaizer::OpResult<> Tsuna::canSet(entt::entity ent) {
        auto race = kaizer::getAspect(ent, "race");
        if(race->getKey() != "kanassan") {
            return {false, "These martial secrets are only taught to Kanassans."};
        }
        return {true, std::nullopt};
    }

    kaizer::OpResult<> Kurzak::canSet(entt::entity ent) {
        auto race = kaizer::getAspect(ent, "race");
        if(race->getKey() != "arlian") {
            return {false, "Only the people of Arlia may learn these arts."};
        }
        return {true, std::nullopt};
    }

    void registerSenseis() {
        auto untrained = std::make_shared<Untrained>();
        registerAspect(untrained);
        registerAspect(std::make_shared<Roshi>());
        registerAspect(std::make_shared<Piccolo>());
        registerAspect(std::make_shared<Krane>());
        registerAspect(std::make_shared<Nail>());
        registerAspect(std::make_shared<Bardock>());
        registerAspect(std::make_shared<Ginyu>());
        registerAspect(std::make_shared<Frieza>());
        registerAspect(std::make_shared<Tapion>());
        registerAspect(std::make_shared<Sixteen>());
        registerAspect(std::make_shared<Dabura>());
        registerAspect(std::make_shared<Kibito>());
        registerAspect(std::make_shared<Jinto>());
        registerAspect(std::make_shared<Tsuna>());
        registerAspect(std::make_shared<Kurzak>());

        kaizer::aspectSlotDefaults["sensei"] = untrained;

    }

}