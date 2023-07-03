#pragma once

#include "dbatk/dgscript.h"
#include "dbatk/components.h"

namespace dbat {

    int64_t mobTriggerGreet(entt::entity actor, std::optional<dir::DirectionId> d);
    void triggerSpeech(entt::entity actor, const std::string &speech);
    void triggerRandom(entt::entity actor);

    int64_t mobTriggerEntry(entt::entity actor);
    int64_t roomTriggerEntry(entt::entity actor, entt::entity room, std::optional<dir::DirectionId> d);

    int64_t triggerCommand(entt::entity actor, const std::string& cmd, const std::string& arg);

    bool mobBribeTrigger(entt::entity actor, entt::entity target, int64_t amount, bool checkOnly = false);
    bool mobReceiveTrigger(entt::entity actor, entt::entity target, entt::entity item, bool checkOnly = false);

    // todo mob
    // bribe trigger
    // greet memory trigger?
    // entry trigger
    // entry memory trigger?


}