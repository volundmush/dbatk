#pragma once
#include "kaizermud/base.h"
#include <bitset>
#include <set>
#include <map>

namespace dbat {

    class DgScriptHandler;

    class DgScript;

    class DgScriptPrototype {
    public:
        kaizer::ObjectID id;
        std::string name;
        uint8_t scriptType;
        std::bitset<32> triggerType;
        std::vector<std::string> lines;
        std::string arglist;
        std::set<std::shared_ptr<DgScript>> instances;
    };

    enum class DgState : uint8_t {
        DORMANT = 0,
        RUNNING = 1,
        WAITING = 2,
        ERROR = 3,
        DONE = 4,
        PURGED = 5
    };

    enum class DgNest : uint8_t {
        IF = 0,
        WHILE = 1,
        SWITCH = 2
    };

    class DgScript {
    public:
        DgScript(DgScriptHandler *handler, const std::shared_ptr<DgScriptPrototype>& prototype);
        DgScriptHandler *handler;
        std::shared_ptr<DgScriptPrototype> prototype;
        double waiting{0.0};
        DgState state{DgState::DORMANT};
        std::vector<std::pair<DgNest, std::size_t>> depth{};
        std::size_t loops{0}, total_loops{0}, context{0};
        std::unordered_map<std::string, std::string> vars{};
    };

    class DgScriptHandler {
    public:
        explicit DgScriptHandler(entt::entity ent);
        entt::entity ent;
        std::vector<std::shared_ptr<DgScript>> scripts{};
    };

    extern std::map<kaizer::ObjectID, std::shared_ptr<DgScriptPrototype>> dgScripts;

    extern std::set<std::shared_ptr<DgScript>> dgScriptsWaiting;

}