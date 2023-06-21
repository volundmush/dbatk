#include "dbatk/core.h"
#include "dbatk/connection.h"
#include "dbatk/database.h"
#include "core/core.h"
#include "sodium.h"



namespace dbat {

    void setConfig() {

    }

    void registerResources() {
        setConfig();
        makeConnection = dbat::makeClientConnection;
        readyDatabase();
    }

    void setupGame() {
        setup();
        logger->info("Initializing libsodium...");
        if (sodium_init() == -1) {
            logger->critical("Could not initialize libsodium!");
            ::core::shutdown(EXIT_FAILURE);
        }
        logger->info("Initializing DBAT Kai Resources...");
        registerResources();
    }
}