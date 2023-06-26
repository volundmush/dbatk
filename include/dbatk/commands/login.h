#pragma once

#include "dbatk/commands.h"

namespace dbat::cmd {
    struct LoginCommandPlay : LoginCommand {
        std::string getCmdName() override { return "play"; };
        std::set<std::string> getAliases() override { return {"select", "p"}; };
        void execute(const std::shared_ptr<Connection>& connection, std::unordered_map<std::string, std::string>& input) override;
    };

    struct LoginCommandNew : LoginCommand {
        std::string getCmdName() override { return "new"; };
        std::set<std::string> getAliases() override { return {"create", "register", "reg", "cr"}; };
        void execute(const std::shared_ptr<Connection>& connection, std::unordered_map<std::string, std::string>& input) override;
    };

    void registerLoginCommands();
}