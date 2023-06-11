#include "dbatk/session.h"
#include "dbatk/connection.h"

namespace dbat {

    void Session::sendLine(const std::string &txt) {
        if(!connections.empty()) {
            if(txt.ends_with("\n")) {
                sendText(txt);
            } else {
                sendText(txt + "\n");
            }
        }
    }

    void Session::sendText(const std::string &txt) {
        for(auto& [id, conn] : connections) {
            conn->sendText(txt);
        }
    }

}