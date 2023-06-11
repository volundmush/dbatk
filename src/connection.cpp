#include "dbatk/connection.h"
#include <fstream>
#include <filesystem>
#include "dbatk/color.h"

namespace dbat {

    static std::optional<std::string> welcomeScreen;

    void DBATConnection::onWelcome() {
        if(!welcomeScreen.has_value()) {
            // Load welcome screen. It's in text/greetansi in cwd.
            std::filesystem::path p = std::filesystem::current_path() / "text" / "greetansi";
            std::ifstream f(p);
            if(!f) {
                logger->error("Failed to open welcome screen file {}", p.c_str());
                welcomeScreen = "Welcome to KaizerMUD!\n";
            } else {
                std::string s;
                std::getline(f, s, '\0');
                welcomeScreen = s;
            }
        }
        sendText(welcomeScreen.value());

    }

    void DBATConnection::sendText(const std::string& msg) {
        std::string s = renderAnsi(msg, capabilities.colorType);
        core::Connection::sendText(s);
    }

    std::shared_ptr<core::Connection> makeClientConnection(int64_t conn_id, JsonChannel chan) {
        return std::make_shared<dbat::DBATConnection>(conn_id, std::move(chan));
    }


}