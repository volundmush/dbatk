#include "dbatk/core.h"
#include "dbatk/components.h"
#include "dbatk/database.h"
#include "dbatk/api.h"
#include "dbatk/commands.h"
#include "dbatk/dgscript.h"
#include "dbatk/config.h"
#include "dbatk/zone.h"

using namespace dbat;


template<typename T>
std::size_t countVnum(std::size_t v) {
    auto view = registry.view<T>();

    std::size_t count = 0;

    for(auto e : view) {
        auto &t = registry.get<T>(e);
        if(t.data == v) {
            count++;
        }
    }
    return count;
}

template<typename T>
std::size_t countVnumHomeRoom(std::size_t v, std::size_t room) {
    auto view = registry.view<T, LegacyLoadRoom>();

    std::size_t count = 0;

    for(auto e : view) {
        auto &t = registry.get<T>(e);
        auto &loadRoom = registry.get<LegacyLoadRoom>(e);
        if(t.data == v && loadRoom.id == room) {
            count++;
        }
    }
    return count;
}

void runTestRoshoMag() {
    auto &z11 = zones[11];
    z11.reset();
    z11.reset();

    // There should be One instance of npc:1 and one instance of npc:2...

    auto countNpc1 = countVnum<NPCVnum>(1125);
    logger->info("Found {} instances of NPC 1125 (Master Roshi)", countNpc1);
    if(!countNpc1) {
        logger->error("There should be 1 instances of NPC 1125 (Master Roshi)");
        return;
    }

    auto countItem1107 = countVnum<ItemVnum>(1107);
    logger->info("Found {} instances of Item 1107 (Porno Mag)", countItem1107);
    if(countItem1107 != 2) {
        logger->error("There should be 2 instances of Item 1107 (Porno Mag)");
        return;
    }

    z11.reset();
    z11.reset();

    countItem1107 = countVnum<ItemVnum>(1107);
    logger->info("Found {} instances of Item 1107 (Porno Mag)", countItem1107);
    if(countItem1107 != 4) {
        logger->error("There should be 4 instances of Item 1107 (Porno Mag)");
        return;
    }

    z11.reset();
    z11.reset();

    countItem1107 = countVnum<ItemVnum>(1107);
    logger->info("Found {} instances of Item 1107 (Porno Mag)", countItem1107);
    if(countItem1107 != 5) {
        logger->error("There should be 5 instances of Item 1107 (Porno Mag)");
        return;
    }

    countVnum<NPCVnum>(1125);
    logger->info("Found {} instances of NPC 1125 (Master Roshi)", countNpc1);
    if(!countNpc1) {
        logger->error("There should be 1 instances of NPC 1125 (Master Roshi)");
        return;
    }

}

void runTestZoneResetE() {
    auto &z4 = zones[4];
    z4.reset();
}

void runTest() {

}


int main(int argc, char* argv[]) {

    // Check if kaitest.sqlite3 exists... if not, error out. Else,
    // copy kaitest.sqlite3 to kaitestrun.sqlite3 (overwrite it if it exists)

    if(!std::filesystem::exists("kaitest.sqlite3")) {
        std::cout << "kaitest.sqlite3 does not exist. Please create it." << std::endl;
        return 1;
    }

    if(std::filesystem::exists("kaitestrun.sqlite3")) {
        std::filesystem::remove("kaitestrun.sqlite3");
    }
    // Also delete any sqlite3 journal files for it...
    if(std::filesystem::exists("kaitestrun.sqlite3-journal")) {
        std::filesystem::remove("kaitestrun.sqlite3-journal");
    }

    // Do the copy...
    std::filesystem::copy("kaitest.sqlite3", "kaitestrun.sqlite3");

    std::cout << "Setting up test mode..." << std::endl;
    config::testMode = true;
    config::dbName = "kaitestrun.sqlite3";
    setupGame();
    logger->info("Game setup successful.");
    loadDatabase();

    runTest();

    return 0;
}