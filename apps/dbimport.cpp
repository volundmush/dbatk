#include <fstream>
#include "dbatk/core.h"
#include "dbatk/database.h"
#include "dbatk/components.h"
#include "dbatk/color.h"
#include "dbatk/dgscript.h"
#include "dbatk/zone.h"
#include "dbatk/api.h"

using namespace dbat;
struct old_ship_data {
    std::string name;
    std::set<RoomId> vnums;
    std::optional<RoomId> hatch_room{};
    std::optional<RoomId> ship_obj{};
    std::optional<RoomId> location{};
};

static struct old_ship_data gships[] = {
        {"Falcon", {3900, 3901, 3902, 3903, 3904}, 3900, 3900, 408},
        {"Simurgh", {3905, 3996, 3997, 3998, 3999}, 3905, 3905, 12002},
        {"Zypher", {3906, 3907, 3908, 3909, 3910}, 3906, 3906, 4250},
        {"Valkyrie", {3911, 3912, 3913, 3914, 3915}, 3911, 3911, 2323},
        {"Phoenix", {3916, 3917, 3918, 3919, 3920}, 3916, 3916, 408},
        {"Merganser", {3921, 3922, 3923, 3924, 3925}, 3921, 3921, 2323},
        {"Wraith", {3926, 3927, 3928, 3929, 3930}, 3930, 3930, 11626},
        {"Ghost", {3931, 3932, 3933, 3934, 3935}, 3935, 3935, 8194},
        {"Wisp", {3936, 3937, 3938, 3939, 3940}, 3940, 3940, 12002},
        {"Eagle", {3941, 3942, 3943, 3944, 3945}, 3945, 3945, 4250},

        {"Spectral", {3946, 3947, 3948, 3949, 3950}, 3950, {}, {}},
        {"Raven", {3951, 3952, 3953, 3954, 3955, 3961}, 3955, {}, {}},
        {"Marauder", {3956, 3957, 3958, 3959, 3960}, 3960, {}, {}},
        {"Vanir", {3962, 3963, 3964, 3965}, 3965, {}, {}},
        {"Aesir", {3966, 3967, 3968, 3969, 3970}, 3970, {}, {}},
        {"Undine", {3971, 3972, 3973, 3974, 3975}, 3975, {}, {}},
        {"Banshee", {3976, 3977, 3978, 3979, 3980}, 3980, {}, {}},
        {"Hydra", {3981, 3982, 3983, 3984, 3985}, 3985, {}, {}},
        {"Medusa", {3986, 3987, 3988, 3989, 3990}, 3990, {}, {}},
        {"Pegasus", {3991, 3992, 3993, 3994, 3995}, 3995, {}, {}},
        {"Zel 1", {5824}, 5824, {}, {}},
        {"Zel 2", {5825}, 5825, {}, {}}
};

static struct old_ship_data customs[] = {
        {"Yun-Yammka", {19900, 19901, 19902}, 19901, 19900, {}},
        {"The Dark Archon", {19903, 19912, 19913, 19914}, 19912, 19916, {}},
        {"The Zafir Krakken", {19904, 19905, 19906, 19907}, 19905, 19905, {}},
        {"Crimson Talon", {19908, 19909, 19910, 19911}, 19908, 19910, {}},
        {"Rust Bucket", {19915, 19916, 19918, 19930}, 19915, 19921, {}},
        {"The Adamant", {19917, 19949, 19955, 19956}, 19949, 19966, {}},
        {"Vanguard", {19919, 19920, 19921, 19922}, 19949, 19926, {}},
        {"The Glacial Shadow", {19925, 19923, 19924, 19926}, 19923, 19931, {}},
        {"The Molecular Dynasty", {19927, 19928, 19929, 19954}, 19927, 19936, {}},
        {"Poseidon", {19931, 19933, 19932, 19934}, 19931, 19941, {}},
        {"Sakana Mirai", {19935, 19936, 19937, 19938}, 19935, 19946, {}},
        {"Earth Far Freighter Enterjection", {19939, 19940, 19941, 19942}, 19939, 19951, {}},
        {"Soaring Angel", {19943, 19944, 19945, 19946}, 19943, 19956, {}},
        {"The Grey Wolf", {19947, 19948, 19978, 19979}, 19947, 19961, {}},
        {"The Event Horizon", {19950, 19951, 19980, 19981}, 19950, 19971, {}},
        {"Fleeting Star", {19952, 19953, 19957, 19958}, 19952, 19976, {}},
        {"Makenkosappo", {19959, 19960, 19961, 19962}, 19959, 19981, {}},
        {"The Nightingale", {19963, 19964, 19965, 19982}, 19963, 19986, {}},
        {"The Honey Bee", {19966, 19967, 19968, 19969}, 19966, 19991, {}},
        {"The Bloodrose", {19970, 19971, 19972, 19973}, 19970, 19996, {}},
        {"The Stimato", {19974, 19975, 19976, 19977}, 19974, {}, {}},
        {"The Tatsumaki", {15805, 15806, 15807, 15808}, 15805, 15805, {}},
        {"Shattered Soul", {15800, 15801, 15802, 15803}, 15800, {}, {}}
};

std::unique_ptr<SQLite::Database> indb;

static std::string cleanString(const std::string &txt) {
    // Given a txt, we want to remove all trailing whitespace/newlines and also the sequence "@n".
    // Then, if the trimmed text contains a @ at all, we want to append a "@n" at the end.
    std::string str = txt;
    for(auto i = 0; i < 3; i++) {
        boost::trim_right(str);
        if(str.ends_with("@n")) {
            str = str.substr(0, str.size() - 2);
        }
    }

    // if '@' is still present in the text, append "@n"
    if (str.find('@') != std::string::npos) {
        str.append("@n");
    }

    return str;
}

void migrate_zones() {
    SQLite::Statement q1(*indb, "SELECT * FROM zones");
    SQLite::Statement q2(*indb, "SELECT flag_id FROM zones_zone_flags WHERE zone_id=?;");
    SQLite::Statement q3(*indb, "SELECT * from reset_commands WHERE zone_id=? ORDER BY line ASC;");

    // Iterate through the query, creating zones in the zones map.
    while(q1.executeStep()) {
        auto id = q1.getColumn("id").getInt64();
        auto &z = zones[id];
        z.name = q1.getColumn("name").getText();
        z.builders = q1.getColumn("builders").getText();
        z.lifespan = q1.getColumn("lifespan").getInt();
        z.bot = q1.getColumn("bot").getInt();
        z.top = q1.getColumn("top").getInt();
        z.reset_mode = q1.getColumn("reset_mode").getInt();
        z.min_level = q1.getColumn("min_level").getInt64();
        z.max_level = q1.getColumn("max_level").getInt64();

        q2.reset();
        q2.bind(1, id);
        while(q2.executeStep()) {
            z.flags.set(q2.getColumn("flag_id").getInt64());
        }

        q3.reset();
        q3.bind(1, id);
        while(q3.executeStep()) {
            auto &cmd = z.cmds.emplace_back();
            cmd.command = q3.getColumn("command").getText()[0];
            cmd.if_flag = q3.getColumn("if_flag").getInt64();
            cmd.arg1 = q3.getColumn("arg1").getInt64();
            cmd.arg2 = q3.getColumn("arg2").getInt64();
            cmd.arg3 = q3.getColumn("arg3").getInt64();
            cmd.arg4 = q3.getColumn("arg4").getInt64();
            cmd.arg5 = q3.getColumn("arg5").getInt64();
            cmd.sarg1 = q3.getColumn("sarg1").getText();
            cmd.sarg2 = q3.getColumn("sarg2").getText();
        }
    }
}


void migrate_scripts() {
    SQLite::Statement q1(*indb, "SELECT * from scripts;");
    while(q1.executeStep()) {
        auto id = q1.getColumn("id").getInt64();
        auto &s = dgScripts[id];
        s.reset(new DgScriptPrototype());
        s->name = q1.getColumn("name").getText();
        auto text = q1.getColumn("script_body").getText();
        boost::split(s->lines, text, boost::is_any_of("\n"));
        s->arglist = q1.getColumn("arglist").getText();
        s->scriptType = q1.getColumn("attach_type").getInt64();
        // Retrieve trigger_type and shove its bits into s->triggerType bitset.
        auto trigger_type = q1.getColumn("trigger_type").getInt64();
        for(int i = 0; i < 32; i++) {
            s->triggerType.set(i, trigger_type & (1 << i));
        }
    }
}

nlohmann::json migrate_item(int64_t id) {
    SQLite::Statement q1(*indb, "SELECT * from objects WHERE id=?;");
    SQLite::Statement q2(*indb, "SELECT position,value FROM object_values WHERE object_id=?;");
    SQLite::Statement q3(*indb, "SELECT location,specific,modifier FROM object_affects WHERE object_id=? ORDER BY idx ASC;");
    SQLite::Statement q4(*indb, "SELECT extra_flag_id FROM object_extra_flags WHERE object_id=?;");
    SQLite::Statement q5(*indb, "SELECT wear_flag_id FROM object_wear_flags WHERE object_id=?;");
    SQLite::Statement q6(*indb, "SELECT keyword, description FROM object_extra_desc WHERE object_id=?;");
    SQLite::Statement q7(*indb, "SELECT script_id FROM object_scripts WHERE object_id=?;");

    q1.bind(1, id);
    q1.executeStep();
    nlohmann::json j;
    j["Name"] = q1.getColumn("name").getText();
    auto room_description = q1.getColumn("room_description").getText();
    if(room_description && strlen(room_description)) j["RoomDescription"] = cleanString(room_description);
    auto look_description = q1.getColumn("look_description").getText();
    if(look_description && strlen(look_description)) j["LookDescription"] = cleanString(look_description);
    auto short_description = q1.getColumn("short_description").getText();
    if(short_description && strlen(short_description)) j["ShortDescription"] = cleanString(short_description);
    auto level = q1.getColumn("level").getInt64();
    if(level) {
        j["LevelRequirement"] = level;
    }
    auto weight = q1.getColumn("weight").getInt64();
    if(weight) {
        j["Weight"] = weight;
    }
    auto cost = q1.getColumn("cost").getInt64();
    if(cost) {
        j["Cost"] = cost;
    }
    auto size = q1.getColumn("size").getInt64();
    if(size > 0) {
        j["Size"] = size;
    }

    std::unordered_map<int64_t, int64_t> values;
    q2.reset();
    q2.bind(1, id);
    while(q2.executeStep()) {
        values[q2.getColumn("position").getInt64()] = q2.getColumn("value").getInt64();
    }

    auto type_flag = q1.getColumn("type_flag").getInt64();
    int i = 0;
    nlohmann::json l, w, c, dr, f;
    // Gotta convert the specific types of items and their shenanigans here...
    switch(type_flag) {
        // ITEM_LIGHT
        case 1:
            l["timeLeft"] = values[0];
            l["hoursLeft"] = values[2];
            j["LightData"] = l;
            break;
            // item weapon
        case 5:
            w["skill"] = values[0];
            w["damDice"] = values[1];
            w["damSize"] = values[2];
            w["damType"] = values[3];
            j["WeaponData"] = w;
            break;
            // item container
        case 15:
            c["capacity"] = values[0];
            // fill flags with the bit positions set of values[1]...
            for(i = 0; i < 4; i++) {
                if(values[1] & (1 << i)) {
                    c["flags"].push_back(i);
                }
            }
            if(values[2]) c["key"] = values[2];
            j["ContainerData"] = c;
            break;
            // drink con
        case 17:
            dr["capacity"] = values[0];
            dr["current"] = values[1];
            dr["liquid"] = values[2];
            if(values[3]) {
                dr["poison"] = true;
            }
            j["DrinkData"] = dr;
            break;
            // item food
        case 19:
            f["maxNutrition"] = values[0];
            f["nutrition"] = values[0];
            if(values[3]) {
                f["poisoned"] = true;
            }
            j["FoodData"] = f;
        default:
            break;
    }

    // Affects
    q3.reset();
    q3.bind(1, id);
    while(q3.executeStep()) {
        nlohmann::json affect;
        affect.push_back(q3.getColumn("location").getInt64());
        affect.push_back(q3.getColumn("specific").getInt64());
        affect.push_back(q3.getColumn("modifier").getInt64());
        j["ItemModifiers"].push_back(affect);
    }

    // run q4 and insert all flags into j["ItemFlags"]
    q4.reset();
    q4.bind(1, id);
    while(q4.executeStep()) {
        j["ItemFlags"].push_back(q4.getColumn("extra_flag_id").getInt64());
    }

    std::unordered_set<int64_t> wear_flags;
    q5.reset();
    q5.bind(1, id);
    while(q5.executeStep()) {
        wear_flags.insert(q5.getColumn("wear_flag_id").getInt64());
    }
    if(!wear_flags.contains(0)) {
        j["ItemFlags"].push_back(iflags::NOTAKE);
    }
    wear_flags.erase(0);
    if(!wear_flags.empty()) {
        nlohmann::json wf;
        for(auto& flag : wear_flags) {
            wf["wearFlags"].push_back(flag-1);
        }
        j["WornData"] = wf;
    }

    // lastly, extra descriptions.
    q6.reset();
    q6.bind(1, id);
    while(q6.executeStep()) {
        auto keyword = q6.getColumn("keyword").getText();
        auto description = q6.getColumn("description").getText();
        nlohmann::json ex;
        ex.push_back(cleanString(keyword));
        ex.push_back(cleanString(description));
        j["ExtraDescriptions"].push_back(ex);
    }

    nlohmann::json item;
    j["Item"] = item;

    q7.reset();
    q7.bind(1, id);
    while(q7.executeStep()) {
        j["DgScripts"].push_back(q7.getColumn("script_id").getInt64());
    }

    return j;

}

void migrate_items() {
    SQLite::Statement q1(*indb, "SELECT * from item_prototypes;");

    while(q1.executeStep()) {
        auto vnum = q1.getColumn("id").getInt64();
        auto id = q1.getColumn("object_id").getInt64();
        auto key = fmt::format("item:{}", vnum);
        auto j = migrate_item(id);
        j["Item"]["vnum"] = vnum;

        savePrototype(key, j);
    }

}

nlohmann::json migrate_npc(int64_t id, bool asPlayer=false) {
    SQLite::Statement q1(*indb, "SELECT * FROM characters WHERE id=?;");
    SQLite::Statement q2(*indb, "SELECT act_bit FROM character_act_bits WHERE character_id=?;");
    SQLite::Statement q3(*indb, "SELECT affected_bit FROM character_affected_bits WHERE character_id=?;");
    SQLite::Statement q4(*indb, "SELECT script_id FROM character_scripts WHERE character_id=?;");
    SQLite::Statement q5(*indb, "SELECT player_bit FROM character_player_bits WHERE character_id=?;");

    q1.bind(1, id);
    q1.executeStep();

    nlohmann::json j;
    j["Name"] = q1.getColumn("name").getText();
    auto room_description = q1.getColumn("room_description").getText();
    if(room_description && strlen(room_description)) j["RoomDescription"] = cleanString(room_description);
    auto look_description = q1.getColumn("look_description").getText();
    if(look_description && strlen(look_description)) j["LookDescription"] = cleanString(look_description);
    auto short_description = q1.getColumn("short_description").getText();
    if(short_description && strlen(short_description)) j["ShortDescription"] = cleanString(short_description);

    auto race = q1.getColumn("race_id").getInt64();
    switch(race) {
        case 23:
            race = 0;
            break;
        default:
            race++;
            break;
    }
    j["Race"] = race;

    auto sensei = q1.getColumn("sensei_id").getInt64();
    switch(sensei) {
        case 255:
            sensei = 0;
            break;
        default:
            sensei++;
            break;
    }
    j["Sensei"] = sensei;
    j["Sex"] = q1.getColumn("sex").getInt64();
    auto race_level = q1.getColumn("race_level").getInt64();
    nlohmann::json stats, character;
    if(race_level) {
        stats.push_back(std::make_pair(cstat::EXPERIENCE, (race_level - 1) * 1000.0));
    }

    std::unordered_map<cstat::StatId, const char*> charStats = {
            {cstat::STRENGTH, "str"}, {cstat::INTELLIGENCE, "intel"},
            {cstat::WISDOM, "wis"}, {cstat::AGILITY, "dex"},
            {cstat::CONSTITUTION, "con"}, {cstat::SPIRIT, "cha"},
            {cstat::ALIGNMENT, "alignment"}, {cstat::ARMOR, "armor"}
    };

    for(auto &[stat, statname] : charStats) {
        auto value = q1.getColumn(statname).getInt64();
        if(value) {
            stats.push_back(std::make_pair(stat, value));
        }
    }
    auto gold = q1.getColumn("gold").getInt64();
    if(gold) j["Money"] = gold;

    nlohmann::json npc;
    j["NPC"] = npc;
    character["stats"] = stats;
    j["Character"] = character;

    auto size = q1.getColumn("size").getInt64();
    if(size != -1) j["Size"] = size;

    if(!asPlayer) {
        q2.reset();
        q2.bind(1, id);
        while(q2.executeStep()) {
            j["NPCFlags"].push_back(q2.getColumn("act_bit").getInt64());
        }
    } else {
        q5.reset();
        q5.bind(1, id);
        while(q5.executeStep()) {
            j["PlayerFlags"].push_back(q5.getColumn("player_bit").getInt64());
        }
    }


    q3.reset();
    q3.bind(1, id);
    while(q3.executeStep()) {
        j["CharacterAffects"].push_back(q3.getColumn("affected_bit").getInt64());
    }

    q4.reset();
    q4.bind(1, id);
    while(q4.executeStep()) {
        j["DgScripts"].push_back(q4.getColumn("script_id").getInt64());
    }

    return j;

}


void migrate_npcs() {

    SQLite::Statement q1(*indb, "SELECT * FROM npc_prototypes;");

    while(q1.executeStep()) {
        auto vnum = q1.getColumn("id").getInt64();
        auto id = q1.getColumn("character_id").getInt64();

        auto j = migrate_npc(id);
        j["Position"] = q1.getColumn("default_pos").getInt64();
        j["NPC"]["vnum"] = vnum;
        auto key = fmt::format("npc:{}", vnum);

        savePrototype(key, j);
    }
}

struct StructureDef {
    std::string name;
    entt::entity parent{};
    std::optional<std::string> searchName;
    std::set<rflags::RFlagId> roomFlags{};
    std::optional<std::pair<std::size_t, std::size_t>> roomRange;
    std::set<RoomId> roomIDs{}, roomSkips{};
    bool global{true};
};

static std::set<int64_t> unknowns;

template<typename... ComponentTypes>
entt::entity assembleStructure(const StructureDef &def) {
    auto obj = createObject();
    setName(obj, def.name);
    auto &of = registry.get_or_emplace<ObjectFlags>(obj);
    of.data[oflags::GLOBALROOMS] = def.global;
    if(registry.valid(def.parent)) {
        setParent(obj, def.parent);
    }

    // Work some component magic here...
    (registry.emplace<ComponentTypes>(obj), ...);

    std::set<RoomId> rooms = def.roomIDs;

    if(def.roomRange) {
        for(auto i = def.roomRange.value().first; i <= def.roomRange.value().second; i++) {
            rooms.insert(i);
        }
    }


    std::vector<std::string> stringVec;
    std::string joined;
    auto tostr = [](const RoomId& i){ return std::to_string(i); };

    if(!def.roomFlags.empty()) {
        std::transform(def.roomFlags.begin(), def.roomFlags.end(), std::back_inserter(stringVec),tostr);
        joined = boost::algorithm::join(stringVec, ",");
        auto rf1q = fmt::format("SELECT DISTINCT room_id FROM room_flags WHERE flag_id IN ({});",joined);
        SQLite::Statement rf1(*indb, rf1q);

        while(rf1.executeStep()) {
            rooms.insert(rf1.getColumn("room_id").getInt64());
        }
    }
    stringVec.clear();


    if(def.searchName) {
        SQLite::Statement rf2(*indb, fmt::format(R"(SELECT id FROM rooms WHERE clean_name LIKE "%{}%";)", def.searchName.value()));
        while(rf2.executeStep()) {
            rooms.insert(rf2.getColumn("id").getInt64());
        }
    }
    auto objid = getObjectId(obj);

    logger->info("Assembling Structure: {}, Rooms: {}", def.name, rooms.size());



    SQLite::Statement rf4(*indb, "SELECT flag_id FROM room_flags WHERE room_id=?");
    SQLite::Statement rf5(*indb, "SELECT script_id FROM room_scripts WHERE room_id=?");
    SQLite::Statement rf6(*indb, "SELECT * FROM exits WHERE room_id=?");
    SQLite::Statement rf7(*indb, "SELECT flag_id FROM exits_info WHERE exit_id=?");
    if(!rooms.empty()) {
        auto &roomstorage = registry.get_or_emplace<Rooms>(obj);
        std::transform(rooms.begin(), rooms.end(), std::back_inserter(stringVec),tostr);
        joined = boost::algorithm::join(stringVec, ",");
        auto rf3q = fmt::format("SELECT * FROM rooms WHERE id IN ({})", joined);
        SQLite::Statement rf3(*indb, rf3q);

        while(rf3.executeStep()) {
            auto vn = rf3.getColumn("id").getInt64();
            if(legacyRooms.contains(vn)) continue;

            nlohmann::json j, jr;
            jr["id"] = vn;
            jr["obj"] = objid;
            j["Room"] = jr;
            j["Name"] = rf3.getColumn("name").getText();
            auto look_description = cleanString(rf3.getColumn("look_description").getText());
            if(!look_description.empty()) j["LookDescription"] = look_description;


            rf4.bind(1, vn);
            while(rf4.executeStep()) {
                j["RoomFlags"].push_back(rf4.getColumn("flag_id").getInt64());
            }
            rf4.reset();

            j["Terrain"] = rf3.getColumn("sector_type").getInt64();

            rf5.bind(1, vn);
            while(rf5.executeStep()) {
                j["DgScripts"].push_back(rf5.getColumn("script_id").getInt64());
            }
            rf5.reset();

            rf6.bind(1, vn);

            while(rf6.executeStep()) {
                auto i = static_cast<dir::DirectionId>(rf6.getColumn("direction").getInt64());

                nlohmann::json jdest;
                jdest["destination"] = rf6.getColumn("to_room").getInt64();
                j["Exits"].push_back(std::make_pair(i, jdest));

                nlohmann::json jopt;
                std::string keyword = rf6.getColumn("keyword").getText();
                std::string general_description = cleanString(rf6.getColumn("general_description").getText());
                if(!general_description.empty()) jopt["description"] = general_description;
                if(!keyword.empty()) jopt["keyword"] = keyword;
                auto id = rf6.getColumn("id").getInt64();
                rf7.bind(1, id);
                while(rf7.executeStep()) {
                    jopt["flags"].push_back(rf7.getColumn("flag_id").getInt64());
                }
                rf7.reset();
                auto key = rf6.getColumn("key").getInt64();
                if(key) jopt["legacyKey"] = key;
                auto dclock = rf6.getColumn("dclock").getInt64();
                auto dchide = rf6.getColumn("dchide").getInt64();
                if(dclock) jopt["dclock"] = dclock;
                if(dchide) jopt["dchide"] = dchide;
                if(jopt.size() > 0) j["Doors"].push_back(std::make_pair(i, jopt));
            }
            rf6.reset();

            auto rent = registry.create();
            roomstorage.data.emplace(vn, rent);
            deserializeEntity(rent, j);
            unknowns.erase(vn);
        }

    }

    return obj;

}

void migrate_structures() {
    SQLite::Statement rf(*indb, "SELECT id FROM rooms;");
    // insert all Ids into unknowns.
    while(rf.executeStep()) {
        unknowns.insert(rf.getColumn("id").getInt64());
    }

    StructureDef adef;
    adef.name = "Admin Land";
    adef.roomIDs = {0, 1, 2, 11, 14, 5, 6, 7, 16694, 16698, 8, 12, 9};
    logger->info("Assembling Admin Land...");
    auto admin_land = assembleStructure<Dimension, Area>(adef);

    StructureDef mudschooldef;
    mudschooldef.name = "MUD School";
    mudschooldef.roomRange = {100, 155};
    logger->info("Assembling MUD School...");
    auto mud_school = assembleStructure<Dimension, Area>(mudschooldef);

    StructureDef mvdef;
    mvdef.name = "Multiverse";
    logger->info("Assembling Multiverse...");
    auto multiverse = assembleStructure<Dimension>(mvdef);

    StructureDef xvdef;
    xvdef.name = "Xenoverse";
    logger->info("Assembling Xenoverse...");
    auto xenoverse = assembleStructure<Dimension>(xvdef);

    logger->info("Assembling Universe 7...");
    auto universe7 = assembleStructure<Dimension>({"Universe 7", multiverse});

    logger->info("Assembling Mortal Plane...");
    auto mortal_plane = assembleStructure<Dimension>({"Mortal Plane", universe7});
    logger->info("Assembling Celestial Plane...");
    auto celestial_plane = assembleStructure<Dimension>({"Celestial Plane", universe7});
    logger->info("Assembling Depths of Space...");
    auto space = assembleStructure<Expanse>({"@WDepths of Space@n", mortal_plane});
    auto spaceDesc = cleanString(" @DThis dark void has very little light.  The light it does have comes from\n"
                                 "distant astral bodies such as stars or planets.  Occasional clouds of gas,\n"
                                 "asteroids, or comets can be seen.  Other than that it is empty blackness for\n"
                                 "thousands upon thousands of miles in every direction.@n");
    SQLite::Statement qspace(*db, "INSERT INTO legacySpaceGrid (id, x, y, z) VALUES (?,?,?,?);");

    setLookDescription(space, spaceDesc);
    auto &poi = registry.emplace<GridPointsOfInterest>(space);
    auto &gbounds = registry.emplace<GridBounds>(space);
    gbounds.minX = -100;
    gbounds.maxX = 100;
    gbounds.minY = -100;
    gbounds.maxY = 100;
    gbounds.minZ = 0;
    gbounds.maxZ = 0;
    std::set<int64_t> spaceVnums;
    // now we need to iterate through oldSpace, and generate a new X-Y coordinate grid where the top-left is (-100, 100) and the bottom-right is (100, -100)
    // with the center at (0,0). This will mean there's now an extra column and row on the right and bottom and that's fine.
    // We create GridPoints and insert them into legacySpaceRooms[vnum] = GridPoint.
    logger->info("Assembling space grid...");
    std::size_t counter = 0;
    SQLite::Statement qs(*indb, "SELECT * from space;");

    while(qs.executeStep()) {
        counter++;
        if(counter % 1000 == 0) logger->info("Processed {} space rooms...", counter);
        auto vnum = qs.getColumn("id").getInt64();
        auto x = qs.getColumn("x").getInt64();
        auto y = qs.getColumn("y").getInt64();
        auto z = qs.getColumn("z").getInt64();
        GridPoint gp(x, y, z);
        spaceVnums.insert(vnum);
        unknowns.erase(vnum);
        legacySpaceRooms[vnum] = gp;
        qspace.bind(1, vnum);
        qspace.bind(2, x);
        qspace.bind(3, y);
        qspace.bind(4, z);
        qspace.exec();
        qspace.reset();
        std::string name = qs.getColumn("name").getText();
        std::string tile = qs.getColumn("tile").getText();
        std::string description = qs.getColumn("description").getText();
        std::string exits = qs.getColumn("exits").getText();
        if(!name.empty() || !tile.empty() || !description.empty() || !exits.empty()) {
            auto ent = registry.create();
            if(!name.empty()) setName(ent, name);
            if(!tile.empty()) setShortDescription(ent, tile);
            if(!description.empty()) setLookDescription(ent, description);
            if(!exits.empty()) {
                auto j = nlohmann::json::parse(exits);
                auto &exc = registry.get_or_emplace<Exits>(ent);
                for(auto &exdata : j) {
                    auto d = exdata[0].get<dir::DirectionId>();
                    exc.data.emplace(d, exdata[1]);
                }
            }
            poi.data.emplace(gp, ent);
        }
    }
    logger->info("Space grid assembled.");

    SQLite::Statement rf2(*indb, "SELECT id,sense_location FROM rooms WHERE sense_location != 'Unknown.';");

    std::unordered_map<std::string, std::set<RoomId>> areas;

    logger->info("Scanning for Areas...");
    while(rf2.executeStep()) {
        auto rv = static_cast<RoomId>(rf2.getColumn("id").getInt64());
        auto sense = rf2.getColumn("sense_location").getText();
        areas[sense].insert(rv);
    }

    std::unordered_map<std::string, entt::entity> areaObjects;

    logger->info("Creating Areas...");
    for(auto &[name, rooms] : areas) {
        StructureDef def;
        def.name = name;
        def.roomIDs = rooms;
        auto aent = assembleStructure<Area>(def);
        areaObjects[name] = aent;
    }

    logger->info("Creating planets...");
    StructureDef earthdef;
    earthdef.name = "Earth";
    earthdef.roomFlags = {rflags::EARTH};
    earthdef.parent = space;
    auto planet_earth = assembleStructure<CelestialBody>(earthdef);

    StructureDef vegetadef;
    vegetadef.name = "Vegeta";
    vegetadef.roomFlags = {rflags::VEGETA};
    vegetadef.parent = space;
    auto planet_vegeta = assembleStructure<CelestialBody>(vegetadef);

    StructureDef friezadef;
    friezadef.name = "Frigid";
    friezadef.roomFlags = {rflags::FRIGID};
    friezadef.parent = space;
    auto planet_frigid = assembleStructure<CelestialBody>(friezadef);

    StructureDef namekdef;
    namekdef.name = "Namek";
    namekdef.roomFlags = {rflags::NAMEK};
    namekdef.parent = space;
    auto planet_namek = assembleStructure<CelestialBody>(namekdef);

    StructureDef yardratdef;
    yardratdef.name = "Yardrat";
    yardratdef.roomFlags = {rflags::YARDRAT};
    yardratdef.parent = space;
    auto planet_yardrat = assembleStructure<CelestialBody>(yardratdef);

    StructureDef kondef;
    kondef.name = "Konack";
    kondef.roomFlags = {rflags::KONACK};
    kondef.parent = space;
    auto planet_konack = assembleStructure<CelestialBody>(kondef);

    StructureDef aetherdef;
    aetherdef.name = "Aether";
    aetherdef.roomFlags = {rflags::AETHER};
    aetherdef.parent = space;
    auto planet_aether = assembleStructure<CelestialBody>(aetherdef);

    StructureDef kanassadef;
    kanassadef.name = "Kanassa";
    kanassadef.roomFlags = {rflags::KANASSA};
    kanassadef.parent = space;
    auto planet_kanassa = assembleStructure<CelestialBody>(kanassadef);

    StructureDef arliadef;
    arliadef.name = "Arlia";
    arliadef.roomFlags = {rflags::ARLIA};
    arliadef.parent = space;
    auto planet_arlia = assembleStructure<CelestialBody>(arliadef);

    StructureDef cerriadef;
    cerriadef.name = "Cerria";
    cerriadef.roomFlags = {rflags::CERRIA};
    cerriadef.parent = space;
    auto planet_cerria = assembleStructure<CelestialBody>(cerriadef);

    auto moon_zenith = assembleStructure<CelestialBody>({"Zenith", space});
    for(const auto& name : {"Ancient Castle", "Utatlan City", "Zenith Jungle"}) {
        setParent(areaObjects[name], moon_zenith);
    }
    auto &cel = registry.get_or_emplace<CelestialBody>(moon_zenith);
    cel.type = celtype::MOON;

    StructureDef ucdef;
    ucdef.name = "Underground Cavern";
    ucdef.parent = moon_zenith;
    ucdef.roomRange = {62900, 63000};
    auto underground_cavern = assembleStructure<Area>(ucdef);

    for(auto &p : {planet_earth, planet_aether, planet_namek, moon_zenith}) {
        auto &cel = registry.get_or_emplace<CelestialBody>(p);
        cel.flags[celflags::ETHERSTREAM] = true;
    }

    StructureDef hbtcdef;
    hbtcdef.name = "Hyperbolic Time Chamber";
    hbtcdef.parent = universe7;
    hbtcdef.roomRange = {64000, 64098};
    auto hbtc = assembleStructure(hbtcdef);
    registry.emplace<Dimension>(hbtc);

    StructureDef bodef;
    bodef.name = "The Black Omen";
    bodef.parent = space;
    bodef.searchName = "Black Omen";
    auto black_omen = assembleStructure<Vehicle>(bodef);

    std::unordered_map<rflags::RFlagId, entt::entity> planetMap = {
            {rflags::EARTH, planet_earth},
            {rflags::VEGETA, planet_vegeta},
            {rflags::FRIGID, planet_frigid},
            {rflags::NAMEK, planet_namek},
            {rflags::YARDRAT, planet_yardrat},
            {rflags::KONACK, planet_konack},
            {rflags::AETHER, planet_aether},
            {rflags::KANASSA, planet_kanassa},
            {rflags::ARLIA, planet_arlia},
            {rflags::CERRIA, planet_cerria},
    };

    logger->info("Attempting to deduce Areas to Planets...");
    auto v = registry.view<Room, RoomFlags>();
    for(auto &ent : v) {
        // check for planetMap flags and, if found, bind the area this room belongs to, to the respective planet.
        auto &room = registry.get<Room>(ent);
        auto &flags = registry.get<RoomFlags>(ent);
        for(auto &p : planetMap) {
            if(flags.data[p.first]) {
                auto e = room.obj.getObject();
                setParent(e, p.second);
                break;
            }
        }
    }
    logger->info("Done deducing Areas to Planets.");

    StructureDef nodef;
    nodef.name = "Northran";
    nodef.parent = xenoverse;
    nodef.roomRange = {17900, 18000};
    auto northran = assembleStructure<Dimension>(nodef);

    StructureDef celdef;
    celdef.name = "Celestial Corp";
    celdef.parent = space;
    celdef.roomRange = {16305, 16399};
    celdef.searchName = "Celestial Corp";
    auto celestial_corp = assembleStructure<Vehicle>(celdef);

    StructureDef gneb;
    gneb.name = "Green Nebula Mall";
    gneb.parent = space;
    gneb.roomRange = {17264, 17277};
    auto green_nebula = assembleStructure<Vehicle>(gneb);

    StructureDef cooler;
    cooler.name = "Cooler's Ship";
    cooler.parent = space;
    cooler.searchName = "Cooler's Ship";
    auto cooler_ship = assembleStructure<Vehicle>(cooler);

    StructureDef alph;
    alph.name = "Alpharis";
    alph.parent = space;
    alph.searchName = "Alpharis";
    auto alpharis = assembleStructure<Vehicle>(alph);

    StructureDef dzone;
    dzone.name = "Dead Zone";
    dzone.parent = universe7;
    dzone.searchName = "Dead Zone";
    auto dead_zone = assembleStructure<Dimension>(dzone);

    StructureDef bast;
    bast.name = "Blasted Asteroid";
    bast.parent = space;
    bast.searchName = "Blasted Asteroid";
    auto blasted_asteroid = assembleStructure<CelestialBody>(bast);
    {
        auto &celbod = registry.get_or_emplace<CelestialBody>(blasted_asteroid);
        celbod.type = celtype::ASTEROID;
    }

    StructureDef listres;
    listres.name = "Lister's Restaurant";
    listres.parent = xenoverse;
    listres.searchName = "Lister's Restaurant";
    auto listers_restaurant = assembleStructure<Dimension,Area>(listres);

    StructureDef scasino;
    scasino.name = "Shooting Star Casino";
    scasino.parent = xenoverse;
    scasino.searchName = "Shooting Star Casino";
    auto shooting_star_casino = assembleStructure<Dimension,Area>(scasino);

    StructureDef outdef;
    outdef.name = "The Outpost";
    outdef.parent = celestial_plane;
    outdef.searchName = "The Outpost";
    auto outpost = assembleStructure<Area>(outdef);

    StructureDef kyem;
    kyem.name = "King Yemma's Domain";
    kyem.parent = celestial_plane;
    kyem.roomRange = {6000, 6031};
    kyem.roomSkips.insert(6017);
    auto king_yemma = assembleStructure<Area>(kyem);

    StructureDef snway;
    snway.name = "Snake Way";
    snway.parent = celestial_plane;
    snway.roomRange = {6031, 6100};
    snway.roomIDs.insert(6017);
    auto snake_way = assembleStructure<Area>(snway);

    StructureDef nkai;
    nkai.name = "North Kai's Planet";
    nkai.parent = celestial_plane;
    nkai.roomRange = {6100, 6139};
    auto north_kai = assembleStructure<CelestialBody>(nkai);
    {
        auto &celbod = registry.get_or_emplace<CelestialBody>(north_kai);
        celbod.gravity = 10.0;
    }

    StructureDef serp;
    serp.name = "Serpent's Castle";
    serp.parent = snake_way;
    serp.roomRange = {6100, 6167};
    auto serpents_castle = assembleStructure<Area>(serp);

    StructureDef gkai;
    gkai.name = "Grand Kai's Planet";
    gkai.parent = celestial_plane;
    gkai.roomRange = {6800, 6961};

    StructureDef maze;
    maze.name = "Maze of Echoes";
    maze.parent = celestial_plane;
    maze.roomRange = {7100, 7200};
    auto maze_of_echoes = assembleStructure<Area>(maze);

    StructureDef cat;
    cat.name = "Dark Catacomb";
    cat.parent = maze_of_echoes;
    cat.roomRange = {7200, 7245};
    auto dark_catacomb = assembleStructure<Area>(cat);

    StructureDef twi;
    twi.name = "Twilight Cavern";
    twi.parent = celestial_plane;
    twi.roomRange = {7300, 7500};
    auto twilight_cavern = assembleStructure<Area>(twi);

    StructureDef helldef;
    helldef.name = "Hell";
    helldef.parent = celestial_plane;
    auto hell = assembleStructure<Area>(helldef);

    StructureDef hfields;
    hfields.name = "Hell Fields";
    hfields.parent = hell;
    hfields.roomRange = {6200, 6300};

    StructureDef hsands;
    hsands.name = "Sands of Time";
    hsands.parent = hell;
    hsands.roomRange = {6300, 6349};
    auto sands_of_time = assembleStructure<Area>(hsands);

    StructureDef hchaotic;
    hchaotic.name = "Chaotic Spiral";
    hchaotic.parent = hell;
    hchaotic.roomRange = {6349, 6400};
    auto chaotic_spiral = assembleStructure<Area>(hchaotic);

    StructureDef hfirecity;
    hfirecity.name = "Hellfire City";
    hfirecity.parent = hell;
    hfirecity.roomRange = {6400, 6530};
    hfirecity.roomIDs = {6568, 6569, 6600, 6699};
    auto hellfire_city = assembleStructure<Area>(hfirecity);

    StructureDef fbagdojo;
    fbagdojo.name = "Flaming Bag Dojo";
    fbagdojo.parent = hellfire_city;
    fbagdojo.roomRange = {6530, 6568};
    auto flaming_bag_dojo = assembleStructure<Area>(fbagdojo);

    StructureDef etrailgrave;
    etrailgrave.name = "Entrail Graveyard";
    etrailgrave.parent = hellfire_city;
    etrailgrave.roomRange = {6601, 6689};
    auto entrail_graveyard = assembleStructure<Area>(etrailgrave);

    StructureDef psihnon;
    psihnon.name = "Planet Sihnon";
    psihnon.parent = space;
    psihnon.roomRange = {3600, 3700};
    auto planet_sihnon = assembleStructure<CelestialBody>(psihnon);

    StructureDef majdef;
    majdef.name = "Majinton";
    majdef.parent = planet_sihnon;
    majdef.roomRange = {3700, 3797};
    auto majinton = assembleStructure<Dimension, Area>(majdef);

    StructureDef wistower;
    wistower.name = "Wisdom Tower";
    wistower.parent = planet_namek;
    wistower.roomRange = {9600, 9667};
    auto wisdom_tower = assembleStructure<Area>(wistower);

    StructureDef machia;
    machia.name = "Machiavilla";
    machia.parent = planet_konack;
    machia.roomRange = {12743, 12798};
    auto machiavilla = assembleStructure<Area>(machia);

    StructureDef monbal;
    monbal.name = "Monastery of Balance";
    monbal.parent = planet_konack;
    monbal.roomRange = {9500, 9599};

    StructureDef futschool;
    futschool.name = "Future School";
    futschool.parent = xenoverse;
    futschool.roomRange = {15938, 16000};
    auto future_school = assembleStructure<Dimension,Area>(futschool);

    StructureDef udfhq;
    udfhq.name = "UDF Headquarters";
    udfhq.parent = space;
    udfhq.roomRange = {18000, 18059};
    auto udf_headquarters = assembleStructure<CelestialBody>(udfhq);
    {
        auto &celbod = registry.get_or_emplace<CelestialBody>(udf_headquarters);
        celbod.type = celtype::STATION;
    }

    StructureDef hspire;
    hspire.name = "The Haven Spire";
    hspire.parent = space;
    hspire.roomRange = {18300, 18341};
    auto haven_spire = assembleStructure<CelestialBody>(hspire);
    {
        auto &celbod = registry.get_or_emplace<CelestialBody>(haven_spire);
        celbod.type = celtype::STATION;
    }

    StructureDef knoit;
    knoit.name = "Kame no Itto";
    knoit.parent = space;
    knoit.roomRange = {18400, 18460};
    auto kame_no_itto = assembleStructure<CelestialBody>(knoit);
    {
        auto &celbod = registry.get_or_emplace<CelestialBody>(kame_no_itto);
        celbod.type = celtype::STATION;
    }

    StructureDef southgal;
    southgal.name = "South Galaxy";
    southgal.parent = mortal_plane;
    southgal.roomIDs = {64300, 64399};
    auto south_galaxy = assembleStructure<Area>(southgal);

    StructureDef shatplan;
    shatplan.name = "Shattered Planet";
    shatplan.parent = south_galaxy;
    shatplan.roomRange = {64301, 64399};
    auto shattered_planet = assembleStructure<CelestialBody>(shatplan);
    {
        auto &celbod = registry.get_or_emplace<CelestialBody>(shattered_planet);
        celbod.type = celtype::PLANET;
    }

    StructureDef wzdef;
    wzdef.name = "War Zone";
    wzdef.parent = xenoverse;
    wzdef.roomRange = {17700, 17703};
    auto war_zone = assembleStructure<Dimension, Area>(wzdef);

    StructureDef corlight;
    corlight.name = "Corridor of Light";
    corlight.parent = war_zone;
    corlight.roomRange = {17703, 17723};
    auto corridor_of_light = assembleStructure<Area>(corlight);

    StructureDef cordark;
    cordark.name = "Corridor of Darkness";
    cordark.parent = war_zone;
    cordark.roomRange = {17723, 17743};
    auto corridor_of_darkness = assembleStructure<Area>(cordark);

    StructureDef soisland;
    soisland.name = "South Ocean Island";
    soisland.parent = planet_earth;
    soisland.roomRange = {6700, 6758};
    auto south_ocean_island = assembleStructure<Area>(soisland);

    StructureDef hhouse;
    hhouse.name = "Haunted House";
    hhouse.parent = xenoverse;
    hhouse.roomRange = {18600, 18693};
    auto haunted_house = assembleStructure<Dimension, Area>(hhouse);

    StructureDef roc;
    roc.name = "Random Occurences, WTF?";
    roc.parent = xenoverse;
    roc.roomRange = {18700, 18776};
    auto random_occurences = assembleStructure<Dimension, Area>(roc);

    StructureDef galstrong;
    galstrong.name = "Galaxy's Strongest Tournament";
    galstrong.parent = space;
    galstrong.roomRange = {17875, 178894};
    auto galaxy_strongest_tournament = assembleStructure<Area>(galstrong);

    StructureDef arwater;
    arwater.name = "Arena - Water";
    arwater.parent = galaxy_strongest_tournament;
    arwater.roomRange = {17800, 17825};
    auto arena_water = assembleStructure<Area>(arwater);

    StructureDef arring;
    arring.name = "Arena - The Ring";
    arring.parent = galaxy_strongest_tournament;
    arring.roomRange = {17825, 17850};
    auto arena_ring = assembleStructure<Area>(arring);

    StructureDef arsky;
    arsky.name = "Arena - In the Sky";
    arsky.parent = galaxy_strongest_tournament;
    arsky.roomRange = {17850, 17875};
    auto arena_sky = assembleStructure<Area>(arsky);

    auto crunch_ship = [&](old_ship_data &data, bool g) {

        StructureDef sdata;
        sdata.name = data.name;
        sdata.roomIDs = data.vnums;
        sdata.global = g;
        auto ship = assembleStructure<Vehicle>(sdata);
        auto &rooms = registry.get<Rooms>(ship);
        if(data.hatch_room) {
            auto hroom = rooms.data[data.hatch_room.value()];
            auto &rmflags = registry.get_or_emplace<RoomFlags>(hroom);
            rmflags.data[rflags::HATCH] = true;
            rmflags.data[rflags::CONTROLS] = true;
        }

        if(data.ship_obj) {
            SQLite::Statement q(*indb, "SELECT name, short_description, room_description, look_description FROM item_prototypes_view WHERE vnum = ?;");
            q.bind(1, static_cast<int64_t>(data.ship_obj.value()));
            if(q.executeStep()) {
                std::string name = q.getColumn(0).getString();
                std::string short_description = q.getColumn(1).getString();
                std::string room_description = q.getColumn(2).getString();
                std::string look_description = q.getColumn(3).getString();
                setName(ship, name);
                if(!short_description.empty()) setShortDescription(ship, cleanString(short_description));
                if(!room_description.empty()) setRoomDescription(ship, cleanString(room_description));
                if(!look_description.empty()) setLookDescription(ship, cleanString(look_description));
            }
        }

        return ship;
    };

    logger->info("Crunching ships...");

    for(auto &sd : gships) {
        crunch_ship(sd, true);
    }

    for(auto &sd : customs) {
        crunch_ship(sd, false);
    }

    auto clear_rooms = [&](RoomId start, RoomId finish) {
        for(RoomId r = start; r < finish; r++) {
            unknowns.erase(r);
        }
    };

    logger->info("Clearing out unused rooms...");
    clear_rooms(19800, 19900);
    clear_rooms(46000, 46150);
    clear_rooms(18800, 19199);

    // A very luxurious player custom home
    StructureDef dunnoHouse;
    dunnoHouse.name = "Dunno's House";
    dunnoHouse.parent = xenoverse;
    dunnoHouse.roomIDs = {19009, 19010, 19011, 19012, 19013, 19014, 19015, 19016, 19017, 19018,
                          19019, 19020, 19021, 19022, 19023};
    auto dunno_house = assembleStructure<Area>(dunnoHouse);

    // This looks like an unused old player home, seems like it's attached to Cherry Blossom Mountain?
    StructureDef mountainFortress;
    mountainFortress.name = "Mountaintop Fortress";
    mountainFortress.parent = xenoverse;
    mountainFortress.roomIDs = {19025, 19026, 19027, 19028, 19029, 19030, 19031, 19032,
                                19033, 19034, 19035, 19036, 19037, 19038};
    auto mountain_fortress = assembleStructure<Area>(mountainFortress);


    // Now that all the areas are created, we need to scan through them all...
    // It'd be great to set their Destination component to properly target the
    // right ObjectId. In addition, those which target legacy space grid need
    // to point at space and the right coordinates.
    logger->info("Linking exits properly.");
    auto view = registry.view<Exits>();
    for (auto ent : view) {
        auto &ex = view.get<Exits>(ent);
        for(auto &[dir, e] : ex.data) {
            auto destId = std::get<RoomId>(e.destination);
            if(legacySpaceRooms.contains(destId)) {
                e.ent = space;
                e.destination = legacySpaceRooms[destId];
            } else {
                auto room = legacyRooms[destId];
                e.ent = registry.get<Room>(room).obj.getObject();
            }
        }
    }
    logger->info("Done linking exits.");

}

void migrate_accounts() {
    SQLite::Statement a1(*indb, "SELECT * from accounts;");
    SQLite::Statement a2(*indb, "SELECT c.* from accounts_characters AS ac LEFT JOIN player_characters AS pc ON ac.player_character_id=pc.id LEFT JOIN characters AS c on pc.character_id=c.id WHERE ac.account_id = ?");
    SQLite::Statement q1(*db, "INSERT INTO accounts (username, password, email, adminLevel) VALUES (?, ?, ?, ?)");
    SQLite::Statement q2(*db, "INSERT INTO playerCharacters (account, character) VALUES (?, ?)");

    while(a1.executeStep()) {
        auto oldid = a1.getColumn("id").getInt();
        auto username = a1.getColumn("username").getText();
        auto password = a1.getColumn("password").getText();
        auto email = a1.getColumn("email").getText();
        auto adminLevel = a1.getColumn("adminLevel").getInt();
        q1.bind(1, username);
        q1.bind(2, password);
        q1.bind(3, email);
        q1.bind(4, adminLevel);
        q1.exec();
        auto id = db->getLastInsertRowid();
        q1.reset();

        a2.bind(1, oldid);
        while(a2.executeStep()) {
            auto cid = a2.getColumn("id").getInt();
            auto data = migrate_npc(cid, true);
            nlohmann::json j;
            j["accountId"] = id;
            data["Player"] = j;
            auto obj = createObject();
            deserializeEntity(obj, data);
            q2.bind(1, id);
            auto objid = getObjectId(obj);
            q2.bind(2, static_cast<int64_t>(objid.index));
            q2.exec();
            q2.reset();
        }
        a2.reset();
    }

}


int main() {
    std::cout << "Beginning migration..." << std::endl;
    setupGame();
    logger->info("Setup Game successful.");
    indb = std::make_unique<SQLite::Database>("dbat.sqlite3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    auto transaction = SQLite::Transaction(*db);
    logger->info("Migrating Zones...");
    migrate_zones();
    logger->info("Migrating Scripts...");
    migrate_scripts();
    logger->info("Migrating Item Prototypes...");
    migrate_items();
    logger->info("Migrating NPC Prototypes...");
    migrate_npcs();
    logger->info("Building structures and migrating rooms...");
    migrate_structures();
    logger->info("Migrating accounts and player characters...");
    migrate_accounts();
    logger->info("Setting everything dirty...");
    gameIsLoading = false;
    for(auto &o : objects) {
        if(registry.valid(o.second)) setDirty(o.second);
    }
    logger->info("Saving all dirty objects to database...");
    processDirty();
    logger->info("Saving Scripts...");
    saveScripts();
    logger->info("Saving Zones...");
    saveZones();
    transaction.commit();
    logger->info("Transaction committed successfully.");


}