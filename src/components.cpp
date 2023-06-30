#include "dbatk/components.h"
#include "dbatk/database.h"
#include "dbatk/color.h"

namespace dbat {

    void StringView::setData(const std::string& txt) {
        data = intern(txt);
        clean = intern(stripAnsi(txt));
    }
    
    nlohmann::json Door::serialize() {
        nlohmann::json d;
        if(flags.any()) d["flags"] = bitsetToJson(flags);
        if(legacyKey) d["legacyKey"] = legacyKey.value();
        if(dclock) d["dclock"] = dclock;
        if(dchide) d["dchide"] = dchide;
        return d;
    }

    Door::Door(const nlohmann::json &j) {
        if(j.contains("flags")) jsonToBitset(j["flags"], flags);
        if(j.contains("legacyKey")) legacyKey.emplace(j["legacyKey"]);
        if(j.contains("dclock")) dclock = j["dclock"];
        if(j.contains("dchide")) dchide = j["dchide"];
    }

    obj_affected_type::obj_affected_type(const nlohmann::json& j) {
        if(j.contains("location")) location = j["location"];
        if(j.contains("modifier")) modifier = j["modifier"];
        if(j.contains("specific")) specific = j["specific"];
    }

    nlohmann::json obj_affected_type::serialize() {
        nlohmann::json j;
        if(location) j["location"] = location;
        if(modifier) j["modifier"] = modifier;
        if(specific) j["specific"] = specific;
        return j;
    }

    skill_data::skill_data(const nlohmann::json& j) {
        if(j.contains("level")) level = j["level"];
        if(j.contains("bonus")) bonus = j["bonus"];
        if(j.contains("perfection")) perfection = j["perfection"];
    }

    nlohmann::json skill_data::serialize() {
        nlohmann::json j;
        if(level) j["level"] = level;
        if(bonus) j["bonus"] = bonus;
        if(perfection) j["perfection"] = perfection;
        return j;
    }



}