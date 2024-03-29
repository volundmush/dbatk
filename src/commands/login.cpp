#include "dbatk/commands.h"
#include "dbatk/components.h"
#include "dbatk/api.h"
#include "dbatk/connection.h"
#include "dbatk/session.h"
#include "dbatk/database.h"

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

    void LoginCommandPlay::execute(const std::shared_ptr<Connection>& connection, std::unordered_map<std::string, std::string>& input) {
        auto acc = connection->getAccount();

        std::vector<entt::entity> characters;
        SQLite::Statement q1(*db, "SELECT character FROM playerCharacters WHERE account = ?;");
        q1.bind(1, acc);

        while(q1.executeStep()) {
            auto character = q1.getColumn(0).getInt64();
            auto found = getObject(character);
            if(registry.valid(found)) {
                characters.push_back(found);
            }
        }

        auto name = input["args"];
        if(name.empty()) {
            connection->sendText("Please enter a name.\n");
            return;
        }

        auto c = partialMatch(name, characters.begin(), characters.end(), false, [](auto &c) {
            return getDisplayName(c, c);
        });

        if(c == characters.end()) {
            connection->sendText("No character found with that name.\n");
            return;
        }

        connection->createOrJoinSession(*c);
    }

    void LoginCommandNew::execute(const std::shared_ptr<Connection> &connection,
                                  std::unordered_map<std::string, std::string> &input) {

    }

    void registerLoginCommands() {
        registerLoginCommand(std::make_shared<LoginCommandPlay>());
        registerLoginCommand(std::make_shared<LoginCommandNew>());
    }

}