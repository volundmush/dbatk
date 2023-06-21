#include "dbatk/zone.h"
#include "dbatk/database.h"

namespace dbat {

    nlohmann::json reset_com::serialize() {
        nlohmann::json j;
        std::string cmd;
        cmd.push_back(command);
        j["command"] = cmd;
        if(if_flag) j["if_flag"] = if_flag;
        if(arg1) j["arg1"] = arg1;
        if(arg2) j["arg2"] = arg2;
        if(arg3) j["arg3"] = arg3;
        if(arg4) j["arg4"] = arg4;
        if(arg5) j["arg5"] = arg5;
        if(!sarg1.empty()) j["sarg1"] = sarg1;
        if(!sarg2.empty()) j["sarg2"] = sarg2;

        return j;
    }

    reset_com::reset_com(const nlohmann::json &j) {
        if(j.contains("command")) {
            std::string cmd = j["command"];
            command = cmd[0];
        }
        if(j.contains("if_flag")) if_flag = j["if_flag"];
        if(j.contains("arg1")) arg1 = j["arg1"];
        if(j.contains("arg2")) arg2 = j["arg2"];
        if(j.contains("arg3")) arg3 = j["arg3"];
        if(j.contains("arg4")) arg4 = j["arg4"];
        if(j.contains("arg5")) arg5 = j["arg5"];
        if(j.contains("sarg1")) sarg1 = j["sarg1"];
        if(j.contains("sarg2")) sarg2 = j["sarg2"];
    }

    nlohmann::json Zone::serialize() {
        nlohmann::json j;
        if(!name.empty()) j["name"] = name;
        if(!builders.empty()) j["builders"] = builders;
        if(lifespan) j["lifespan"] = lifespan;
        if(reset_mode) j["reset_mode"] = reset_mode;
        if(!min_level) j["min_level"] = min_level;
        if(!max_level) j["max_level"] = max_level;

        if(flags.any()) j["flags"] = bitsetToJson(flags);

        for(auto& cmd : cmds) {
            j["cmds"].push_back(cmd.serialize());
        }

        return j;
    }

    Zone::Zone(const nlohmann::json &j) {
        if(j.contains("name")) name = j["name"];
        if(j.contains("builders")) builders = j["builders"];
        if(j.contains("lifespan")) lifespan = j["lifespan"];
        if(j.contains("reset_mode")) reset_mode = j["reset_mode"];
        if(j.contains("min_level")) min_level = j["min_level"];
        if(j.contains("max_level")) max_level = j["max_level"];
        if(j.contains("flags")) jsonToBitset(j["flags"], flags);

        if(j.contains("cmds")) {
            for(auto& cmd : j["cmds"]) {
                cmds.emplace_back(cmd);
            }
        }
    }

    std::unordered_map<std::size_t, Zone> zones;
}