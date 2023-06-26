#pragma once
#include "dbatk/base.h"

namespace dbat {

    void setupLogger();
    void setup();
    void startup();

    void shutdown(int exitCode);

    extern std::vector<std::function<async<void>()>> services;

    void handleGameShutdown();

    void handleGameRestart();

    void setConfig();

    void registerResources();

    void setupGame();

}