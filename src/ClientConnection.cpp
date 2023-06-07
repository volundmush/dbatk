#include "dbatk/ClientConnection.h"
#include <fstream>
#include <filesystem>
#include "spdlog/spdlog.h"

namespace dbat {

    static std::optional<std::string> welcomeScreen;

    void Connection::onWelcome() {
        if(!welcomeScreen.has_value()) {
            // Load welcome screen. It's in text/greetansi in cwd.
            std::filesystem::path p = std::filesystem::current_path() / "text" / "greetansi";
            std::ifstream f(p);
            if(!f) {
                spdlog::error("Failed to open welcome screen file {}", p.c_str());
                welcomeScreen = "Welcome to KaizerMUD!\n";
            } else {
                std::string s;
                std::getline(f, s, '\0');
                welcomeScreen = s;
            }
        }
        sendText(welcomeScreen.value());

    }

    std::shared_ptr<kaizer::ClientConnection> makeClientConnection(kaizer::LinkManager& lm, uint64_t conn_id, kaizer::mpmc_channel<boost::json::value> chan) {
        return std::make_shared<dbat::Connection>(lm, conn_id, std::move(chan));
    }


}