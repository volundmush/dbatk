#pragma once
#include "dbatk/base.h"

namespace dbat {

    enum class GameLoop {
        Running = 0,
        Shutdown = 1,
        Restart = 2
    };

    extern GameLoop gameLoopStatus;

    extern std::vector<std::function<async<void>()>> gameStartupFuncs;

    async<void> game();

    async<void> run();

    async<void> heartbeat(double deltaTime);

    async<void> gameShutdown();

    async<void> gameRestart();
}