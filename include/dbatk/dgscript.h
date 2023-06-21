#pragma once
#include "dbatk/base.h"


namespace dbat {

    class DgScriptHandler;

    class DgScript;

    class DgScriptPrototype {
    public:
        DgScriptPrototype() = default;
        explicit DgScriptPrototype(const nlohmann::json& j);
        std::size_t id{};
        std::string name;
        uint8_t scriptType{};
        std::bitset<32> triggerType;
        std::vector<std::string> lines;
        std::string arglist;
        std::set<std::shared_ptr<DgScript>> instances;
        nlohmann::json serialize();
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
        DgScript(entt::entity ent, const std::shared_ptr<DgScriptPrototype>& prototype);
        entt::entity ent;
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

    extern std::unordered_map<std::size_t, std::shared_ptr<DgScriptPrototype>> dgScripts;

    struct DgScriptsWaiting {};

}