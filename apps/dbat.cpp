#include "dbatk/core.h"
using namespace dbat;

int main(int argc, char* argv[]) {
    std::cout << "Setting up..." << std::endl;
    setupGame();
    logger->info("Game setup successful.");
    startup();
    return 0;
}