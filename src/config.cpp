#include "dbatk/config.h"

namespace dbat::config {
    using namespace std::chrono_literals;

    bool enableMultithreading{true};
    int threadsCount{0};
    bool usingMultithreading{false};
    std::chrono::milliseconds heartbeatInterval{100ms};
    std::string thermiteAddress{"127.0.0.1"};
    uint16_t thermitePort{7000};

    std::string dbName = "dbatkai.sqlite3";

    std::size_t startingObject = 70;
    RoomId startingRoom = 300;
    int64_t legacySpaceId = 7;
    bool testMode{false};

}