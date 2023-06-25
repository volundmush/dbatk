#include "dbatk/core.h"
#include "dbatk/connection.h"
#include "dbatk/database.h"
#include "core/core.h"
#include "core/color.h"
#include "core/database.h"
#include "dbatk/color.h"
#include "core/config.h"
#include "core/system.h"
#include "core/commands/connect.h"


namespace dbat {

    void setConfig() {
        config::thermiteAddress = "192.168.50.50";
        config::threadsCount = 2;
    }

    void registerResources() {
        setConfig();
        core::schema.insert(core::schema.end(), dbat::extraSchema.begin(), dbat::extraSchema.end());
        core::renderAnsi = dbat::circleRenderAnsi;
        core::serializeFuncs.push_back(dbat::serializeDbatEntity);
        core::deserializeFuncs.push_back(dbat::deserializeDbatEntity);
        core::preLoadFuncs.push_back(dbat::loadLegacySpace);
        core::preLoadFuncs.push_back(dbat::loadZones);
        core::preLoadFuncs.push_back(dbat::loadScripts);
        core::registerSystems();
        core::sortSystems();
        core::makeConnection = dbat::makeClientConnection;
        core::cmd::registerConnectCommands();

        core::expandCommands();
        readyDatabase();
    }

    void setupGame() {
        setup();
        logger->info("Initializing DBAT Kai Resources...");
        registerResources();
    }
}