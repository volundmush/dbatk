#include "dbatk/Core.h"
#include "kaizermud/Stats.h"
#include "dbatk/Senseis.h"
#include "dbatk/Races.h"
#include "dbatk/Sex.h"
#include "dbatk/Database.h"
#include "kaizermud/Database.h"
#include "kaizermud/startup.h"
#include "dbatk/ClientConnection.h"
#include "kaizermud/thermite.h"
#include "dbatk/Terrain.h"
#include "dbatk/Api.h"
#include "dbatk/Types.h"
#include "kaizermud/Config.h"


namespace dbat {

    void setConfig() {
        kaizer::config::startingRoom = 300;
    }

    void registerResources() {
        setConfig();
        kaizer::registerBaseResources();
        kaizer::makeConnection = dbat::makeClientConnection;
        sensei::registerSenseis();
        race::registerRaces();
        sex::registerSexes();
        terrain::registerTerrainTypes();
        registerDBAPI();
        registerDBFuncs();
        //types::registerDBTypes();
        kaizer::schema.insert(kaizer::schema.end(), extendSchema.begin(), extendSchema.end());

    }
}