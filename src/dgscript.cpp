#include "dbatk/dgscript.h"
#include "dbatk/database.h"

namespace dbat {
    std::unordered_map<std::size_t, std::shared_ptr<DgScriptPrototype>> dgScripts{};


    nlohmann::json DgScriptPrototype::serialize() {
        nlohmann::json j;

        j["id"] = id;
        if(!name.empty()) j["name"] = name;
        if(scriptType) j["scriptType"] = scriptType;
        if(triggerType.any()) j["triggerType"] = bitsetToJson(triggerType);
        if(!lines.empty()) j["lines"] = lines;
        if(!arglist.empty()) j["arglist"] = arglist;

        return j;
    }

    DgScriptPrototype::DgScriptPrototype(const nlohmann::json &j) {
        if(j.contains("id")) id = j["id"];
        if(j.contains("name")) name = j["name"];
        if(j.contains("scriptType")) scriptType = j["scriptType"];
        if(j.contains("triggerType")) jsonToBitset(j["triggerType"], triggerType);
        if(j.contains("lines")) lines = j["lines"];
        if(j.contains("arglist")) arglist = j["arglist"];
    }
}