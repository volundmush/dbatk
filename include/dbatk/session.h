//
// Created by volund on 11/27/21.
//

#ifndef DBATK_SESSION_H
#define DBATK_SESSION_H

#include "sysdep.h"

namespace dbat::session {

    // the key of sessions is the character id being played.
    extern std::map<uint64_t, entt::entity> sessions;
    extern std::set<uint64_t> closing;

    struct SessionData {
        uint64_t char_id;
        entt::entity entity;
        std::set<entt::entity> connections;
        std::list<std::string> output, input;
        std::time_t start_time;
        entt::entity character, puppet;

        void send_text(const std::string &txt);
        void writeToOutput(const char *txt, ...);
        void vwriteToOutput(const char *format, va_list args);
        void process_input();
        void process_output();
    };


    entt::entity make_session(uint64_t char_id);

    entt::entity create_session(uint64_t char_id);

    void end_session(uint64_t char_id);

    nlohmann::json save_session(uint64_t char_id);

    nlohmann::json save_sessions();

    void load_session(uint64_t char_id, nlohmann::json &j);

    void load_sessions(nlohmann::json& j);

}

#endif //DBATK_SESSION_H
