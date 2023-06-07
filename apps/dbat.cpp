#include <iostream>
#include <string>
#include <filesystem>
#include "kaizermud/base.h"
#include "kaizermud/game.h"
#include "kaizermud/Api.h"
#include "kaizermud/Database.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include "dbatk/Core.h"
#include "fmt/format.h"
#include "boost/asio.hpp"
#include "kaizermud/startup.h"


using namespace kaizer;


int main(int argc, char* argv[]) {
    std::cout << "starting program..." << std::endl;

    auto ip = boost::asio::ip::make_address("192.168.50.50");
    auto port = 7000;
    auto endpoint = boost::asio::ip::tcp::endpoint(ip, port);
    try {
        dbat::registerResources();
        kaizer::startup(endpoint);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(1);
    }

    std::cout << "Press Enter to continue...";
    std::cin.get();  // Program will wait here until Enter is pressed
    return 0;
}