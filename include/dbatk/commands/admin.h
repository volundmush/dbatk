#pragma once
#include "dbatk/commands/object.h"

namespace dbat::cmd {
    struct AdmCmd : ObjCmd {
        bool isUsable(entt::entity ent) override {return true;};
    };

    struct AdmTeleport : AdmCmd {
        std::string getCmdName() override { return ".teleport"; };
        std::set<std::string> getAliases() override { return {".tel"}; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    struct AdmGoto : AdmCmd {
        std::string getCmdName() override { return "goto"; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    struct AdmAt : AdmCmd {
        std::string getCmdName() override { return "at"; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    struct AdmList : AdmCmd {
        std::string getCmdName() override { return ".list"; };
        std::set<std::string> getAliases() override { return {".li"}; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    struct AdmZone : AdmCmd {
        std::string getCmdName() override { return ".zone"; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    struct AdmCheat : AdmCmd {
        std::string getCmdName() override { return ".cheat"; };
        std::set<std::string> getAliases() override { return {".ch"}; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    struct AdmExamine : AdmCmd {
        std::string getCmdName() override { return ".examine"; };
        std::set<std::string> getAliases() override { return {".ex"}; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    struct AdmWhere : AdmCmd {
        std::string getCmdName() override { return ".where"; };
        std::set<std::string> getAliases() override { return {".wh"}; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    struct AdmSpawn : AdmCmd {
        std::string getCmdName() override { return ".spawn"; };
        std::set<std::string> getAliases() override { return {".sp"}; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    struct AdmForce : AdmCmd {
        std::string getCmdName() override { return ".force"; };
        std::set<std::string> getAliases() override { return {".fo", "force"}; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    void registerAdminCommands();
}