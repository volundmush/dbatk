#pragma once
#include "dbatk/base.h"


namespace dbat {

    class DgScripts;

    class DgScript;

    using DgVariant = std::variant<std::string, entt::entity>;
    using DgFunc = std::function<DgVariant(const std::shared_ptr<DgScript>&, const std::string&, bool, const std::string&)>;

    DgVariant dgGlobal(const std::shared_ptr<DgScript>& script, const std::string& member, bool call, const std::string& arg);
    DgVariant dgTime(const std::shared_ptr<DgScript>& script, const std::string& member, bool call, const std::string& arg);
    DgVariant dgFindmob(const std::shared_ptr<DgScript>& script, const std::string& member, bool call, const std::string& arg);
    DgVariant dgFindobj(const std::shared_ptr<DgScript>& script, const std::string& member, bool call, const std::string& arg);
    DgVariant dgRandom(const std::shared_ptr<DgScript>& script, const std::string& member, bool call, const std::string& arg);

    std::string serializeDgRef(entt::entity ent);

    DgVariant makeVar(const std::string& str);

    namespace mtrig {
        enum MTrigType : uint8_t {
            GLOBAL = 0,
            RANDOM = 1,
            COMMAND = 2,
            SPEECH = 3,
            ACT = 4,
            DEATH = 5,
            GREET = 6,
            GREET_ALL = 7,
            ENTRY = 8,
            RECEIVE = 9,
            FIGHT = 10,
            HITPRCNT = 11,
            BRIVE = 12,
            LOAD = 13,
            MEMORY = 14,
            CAST = 15,
            LEAVE = 16,
            DOOR = 17,
            TIME = 19
        };
    }

    namespace otrig {
        enum OTrigType : uint8_t {
            GLOBAL = 0,
            RANDOM = 1,
            COMMAND = 2,
            TIMER = 5,
            GET = 6,
            DROP = 7,
            GIVE = 8,
            WEAR = 9,
            REMOVE = 11,
            LOAD = 13,
            CAST = 15,
            LEAVE = 16,
            CONSUME = 18,
            TIME = 19
        };

        enum OTrigCmd : uint8_t {
            EQUIP = 0,
            INVEN = 1,
            ROOM = 2
        };

        enum OTrigCon : uint8_t {
            EAT = 1,
            DRINK = 2,
            QUAFF = 3
        };
    }

    namespace rtrig {
        enum RTrigType : uint8_t {
            GLOBAL = 0,
            RANDOM = 1,
            COMMAND = 2,
            SPEECH = 3,

            RESET = 5,
            ENTER = 6,
            DROP = 7,

            CAST = 15,
            LEAVE = 16,
            DOOR = 17,

            TIME = 19
        };
    }


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
        int narg;
        nlohmann::json serialize();
        std::string getLine(std::size_t line);
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

    struct HasDgVars {
        std::unordered_map<std::string, DgVariant> vars{};
        nlohmann::json serializeVars();
        void deserializeVars(const nlohmann::json &j);
        void setVar(const std::string& name, const DgVariant& var);
        void setVar(const std::string& name, const std::string& var);
        std::optional<DgVariant> getVar(const std::string& name);
        bool hasVar(const std::string& name);
        void clearVar(const std::string& name);
        void clearVars();
        void checkEntity();
    };

    extern HasDgVars globalDgVars;
    extern bool globalDgDirty;

    class DgScript : public HasDgVars, public std::enable_shared_from_this<DgScript> {
    public:
        DgScript(entt::entity ent, const std::shared_ptr<DgScriptPrototype>& prototype);
        entt::entity ent;
        std::shared_ptr<DgScriptPrototype> prototype;
        double waiting{0.0};
        DgState state{DgState::DORMANT};
        std::vector<std::pair<DgNest, std::size_t>> depth{};
        std::size_t currentLine{0}, loops{0};

        void reset();
        void setState(DgState st);
        DgScripts& getHandler();
        void onHeartbeat(double deltaTime);
        int64_t execute();
        int64_t executeBlock(std::size_t start, std::size_t end);

        bool executeIf(const std::string& arg);
        std::size_t findElseEnd(bool matchElseIf = true, bool matchElse = true);
        std::size_t findEnd();
        std::size_t findDone();
        std::size_t findCase(const std::string& arg);
        std::string evaluate(const std::string& arg);
        bool truthy(const std::string& arg);
        std::string maybeNegate(const std::string& arg);
        std::string varSubstitute(const std::string& arg);
        std::string evaluateLhsOpRhs(const std::string& arg);
        std::string evaluateOp(const std::string& lhs, const std::string& op, const std::string& rhs);
        std::string evaluateVar(const std::string& arg);
        std::string scriptInfo();
        void scriptError(const std::string& txt);

        double cmdWait(std::unordered_map<std::string, std::string>& matched);
        void cmdGlobal(std::unordered_map<std::string, std::string>& matched);
        void cmdContext(std::unordered_map<std::string, std::string>& matched);
        void cmdRdelete(std::unordered_map<std::string, std::string>& matched);
        void cmdRemote(std::unordered_map<std::string, std::string>& matched);
        void cmdSet(std::unordered_map<std::string, std::string>& matched);
        void cmdUnset(std::unordered_map<std::string, std::string>& matched);
        void cmdAttach(std::unordered_map<std::string, std::string>& matched);
        void cmdDetach(std::unordered_map<std::string, std::string>& matched);

    };

    class DgScripts : public HasDgVars {
    public:
        explicit DgScripts(entt::entity ent);
        DgScripts(entt::entity ent, const nlohmann::json& j);
        entt::entity ent;
        std::vector<std::shared_ptr<DgScript>> scripts{};
        OpResult<> addScript(std::size_t id);
        OpResult<> removeScript(std::size_t id);
        nlohmann::json serialize();
        DgVariant evalVar(const std::shared_ptr<DgScript>& script, const std::string& member, bool call, const std::string& arg);
    };

    extern std::unordered_map<std::size_t, std::shared_ptr<DgScriptPrototype>> dgScripts;

    extern std::set<std::shared_ptr<DgScript>> dgScriptInstances;

    // Let's make an exception just for DgScripts...
    class DgScriptException : public std::exception {
    private:
        std::string message;
    public:
        DgScriptException(const std::string& msg) : message(msg) {}

        // Override the what() method from std::exception
        const char* what() const noexcept override {
            return message.c_str();
        }
    };

}