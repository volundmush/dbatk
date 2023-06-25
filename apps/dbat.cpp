#include "dbatk/core.h"
#include "core/core.h"

using namespace dbat;

int main(int argc, char* argv[]) {
    std::cout << "Setting up..." << std::endl;
    setupGame();
    logger->info("Game setup successful.");
    core::startup();
    return 0;
}