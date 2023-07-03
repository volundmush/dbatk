#pragma once
#include "dbatk/base.h"

namespace dbat {

    class System {
    public:
        virtual std::string getName() = 0;
        virtual int64_t getPriority() = 0;
        virtual async<bool> shouldRun(double deltaTime);
        virtual async<void> run(double deltaTime);
    };

    class ProcessConnections : public System {
    public:
        std::string getName() override {return "ProcessConnections";};
        int64_t getPriority() override {return -10000;};
        async<void> run(double deltaTime) override;
    };

    class ProcessSessions : public System {
    public:
        std::string getName() override {return "ProcessSessions";};
        int64_t getPriority() override {return -9000;};
        async<void> run(double deltaTime) override;
    };

    class ProcessOutput : public System {
    public:
        std::string getName() override {return "ProcessOutput";};
        int64_t getPriority() override {return 10000;};
        async<void> run(double deltaTime) override;
    };

    class ProcessCommands : public System {
    public:
        std::string getName() override {return "ProcessCommands";};
        int64_t getPriority() override {return 1000;};
        async<void> run(double deltaTime) override;
    };

    class ProcessZones : public System {
    public:
        std::string getName() override {return "ProcessZones";};
        int64_t getPriority() override {return 100;};
        async<void> run(double deltaTime) override;
    };

    class ProcessDgScripts : public System {
    public:
        std::string getName() override {return "ProcessDgScripts";};
        int64_t getPriority() override {return 50;};
        async<void> run(double deltaTime) override;
    };

    class ProcessDgRandom : public System {
    public:
        std::string getName() override {return "ProcessDgRandom";};
        int64_t getPriority() override {return 55;};
        async<bool> shouldRun(double deltaTime) override;
        async<void> run(double deltaTime) override;
    protected:
        double timer={13.0};
    };

    extern std::vector<std::shared_ptr<System>> sortedSystems;
    extern std::unordered_map<std::string, std::shared_ptr<System>> systemRegistry;
    void registerSystem(const std::shared_ptr<System>& system);
    void sortSystems();

    void registerSystems();

}