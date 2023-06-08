#include "kaizermud/Api.h"
#include "dbatk/Components.h"
#include "dbatk/Senseis.h"
#include "dbatk/Races.h"

namespace dbat::sensei {

    kaizer::OpResult<> Sensei::canSet(entt::entity ent) {
        auto &c = kaizer::registry.get_or_emplace<dbat::components::Character>(ent);
        if(c.race == 10) {
            return {false, "Androids really had best learn from Sixteen."};
        }
        return {true, std::nullopt};
    }

    kaizer::OpResult<> Sixteen::canSet(entt::entity ent) {
        auto &c = kaizer::registry.get_or_emplace<dbat::components::Character>(ent);
        if(c.race != 10) {
            return {false, "Only Androids can be trained by Sixteen."};
        }
        return {true, std::nullopt};
    }

    kaizer::OpResult<> Dabura::canSet(entt::entity ent) {
        auto &c = kaizer::registry.get_or_emplace<dbat::components::Character>(ent);
        if(c.race != 11) {
            return {false, "Only Demons can be trained by Dabura."};
        }
        return {true, std::nullopt};
    }

    kaizer::OpResult<> Jinto::canSet(entt::entity ent) {
        auto &c = kaizer::registry.get_or_emplace<dbat::components::Character>(ent);
        if(c.race != 15) {
            return {false, "Jinto will only teach the secrets of the stars to Hoshijin."};
        }
        return {true, std::nullopt};
    }

    kaizer::OpResult<> Tsuna::canSet(entt::entity ent) {
        auto &c = kaizer::registry.get_or_emplace<dbat::components::Character>(ent);
        if(c.race != 7) {
            return {false, "These martial secrets are only taught to Kanassans."};
        }
        return {true, std::nullopt};
    }

    kaizer::OpResult<> Kurzak::canSet(entt::entity ent) {
        auto &c = kaizer::registry.get_or_emplace<dbat::components::Character>(ent);
        if(c.race != 21) {
            return {false, "Only the people of Arlia may learn these arts."};
        }
        return {true, std::nullopt};
    }

    std::vector<std::shared_ptr<Sensei>> senseiRegistry;

    void registerSensei(const std::shared_ptr<Sensei>& entry) {
        while(senseiRegistry.size() < entry->getID() + 1) {
            senseiRegistry.emplace_back(nullptr);
        }
        senseiRegistry[entry->getID()] = entry;
    }

    void registerSenseis() {
        registerSensei(std::make_shared<Kurzak>());
        registerSensei(std::make_shared<Untrained>());
        registerSensei(std::make_shared<Roshi>());
        registerSensei(std::make_shared<Piccolo>());
        registerSensei(std::make_shared<Krane>());
        registerSensei(std::make_shared<Nail>());
        registerSensei(std::make_shared<Bardock>());
        registerSensei(std::make_shared<Ginyu>());
        registerSensei(std::make_shared<Frieza>());
        registerSensei(std::make_shared<Tapion>());
        registerSensei(std::make_shared<Sixteen>());
        registerSensei(std::make_shared<Dabura>());
        registerSensei(std::make_shared<Kibito>());
        registerSensei(std::make_shared<Jinto>());
        registerSensei(std::make_shared<Tsuna>());

    }

}