#include "dbatk/game.h"
#include "dbatk/system.h"
#include "dbatk/config.h"
#include "dbatk/core.h"
#include "dbatk/link.h"
#include "dbatk/database.h"

namespace dbat {
    std::vector<std::function<async<void>()>> gameStartupFuncs;

    async<void> game() {
        logger->info("Starting game");
        logger->info("Sorting systems...");
        sortSystems();
        if(sortedSystems.empty()) {
            logger->warn("No systems registered.");
        }

        logger->info("Running game startup functions...");
        for(auto& func : gameStartupFuncs) {
            co_await func();
        }

        co_await run();
        logger->info("Exited game run()...");

        linkManager->stop();



        co_return;
    }

    GameLoop gameLoopStatus{GameLoop::Running};

    async<void> run() {
        try {
            auto startdb = std::chrono::high_resolution_clock::now();
            loadDatabase();
            auto enddb = std::chrono::high_resolution_clock::now();
            broadcast(fmt::format("Database loaded in {:.3f} seconds", std::chrono::duration<double>(enddb - startdb).count()));
        }
        catch(std::exception& e) {
            logger->critical("Exception while loading database: {}", e.what());
            shutdown(EXIT_FAILURE);
        }
        gameIsLoading = false;

        auto previousTime = boost::asio::steady_timer::clock_type::now();
        boost::asio::steady_timer timer(co_await boost::asio::this_coro::executor, config::heartbeatInterval);
        broadcast("Let the games begin!");
        while(gameLoopStatus == GameLoop::Running) {
            auto timeStart = boost::asio::steady_timer::clock_type::now();
            co_await timer.async_wait(boost::asio::use_awaitable);
            auto timeEnd = boost::asio::steady_timer::clock_type::now();

            auto deltaTime = timeEnd - timeStart;
            double deltaTimeInSeconds = std::chrono::duration<double>(deltaTime).count();

            try {
                co_await heartbeat(deltaTimeInSeconds);
            } catch(std::exception& e) {
                logger->critical("Exception during heartbeat: {}", e.what());
                broadcast("Critical error detected in game simulation, commencing emergency shutdown!");
                shutdown(EXIT_FAILURE);
            }

            auto timeAfterHeartbeat = boost::asio::steady_timer::clock_type::now();
            auto elapsed = timeAfterHeartbeat - timeStart;
            auto nextWait = config::heartbeatInterval - elapsed;

            // If heartbeat takes more than 100ms, default to a very short wait
            if(nextWait.count() < 0) {
                nextWait = std::chrono::milliseconds(1);
            }

            timer.expires_from_now(nextWait);
        }

        switch(gameLoopStatus) {
            case GameLoop::Shutdown:
                logger->info("Shutting down...");
                co_await gameShutdown();
                break;
            case GameLoop::Restart:
                logger->info("Restarting...");
                co_await gameRestart();
                break;
        }

        co_return;
    }

    async<void> heartbeat(double deltaTime) {
        SQLite::Transaction transaction(*db);
        for(auto &sys : sortedSystems) {
            if(co_await sys->shouldRun(deltaTime))
                co_await sys->run(deltaTime);
        }
        processDirty();
        transaction.commit();
        co_return;
    }

    async<void> gameShutdown() {
        logger->info("Shutting down...");
        co_return;
    }

    async<void> gameRestart() {
        logger->info("Restarting...");
        co_return;
    }

}