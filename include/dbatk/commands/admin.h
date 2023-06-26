#pragma once
#include "dbatk/commands/object.h"

namespace dbat::cmd {
    struct AdmCmd : ObjCmd {
        bool isUsable(entt::entity ent) override {return true;};
    };

    struct AdmTeleport : AdmCmd {
        std::string getCmdName() override { return "@teleport"; };
        std::set<std::string> getAliases() override { return {"@tel"}; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    struct AdmGoto : AdmCmd {
        std::string getCmdName() override { return "goto"; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    void registerAdminCommands();
}