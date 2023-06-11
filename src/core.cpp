#include "dbatk/core.h"
#include "dbatk/connection.h"



namespace dbat {

    void setConfig() {

    }

    void registerResources() {
        setConfig();
        makeConnection = dbat::makeClientConnection;

    }
}