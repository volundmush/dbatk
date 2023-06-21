#include "dbatk/core.h"
#include "dbatk/database.h"

using namespace dbat;

int main(int argc, char* argv[]) {
    std::cout << "Setting up..." << std::endl;
    setupGame();
    logger->info("Setup Game successful.");
    logger->info("Initializing database...");
    // let's time the database load. gimme a high resolution timestamp.
    auto start = std::chrono::high_resolution_clock::now();
    loadDatabase();
    auto end = std::chrono::high_resolution_clock::now();
    // Display the result in seconds as a double.
    std::cout << "Database loaded in " << std::chrono::duration<double>(end - start).count() << " seconds." << std::endl;

    // Please press enter to continue...
    std::cout << "Press enter to continue..." << std::endl;
    std::cin.get();
    return 0;
}