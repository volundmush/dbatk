#include "dbatk/database.h"
#include "dbatk/components.h"
#include "dbatk/conf.h"
#include "dbatk/zone.h"
#include "dbatk/dgscript.h"
#include "core/link.h"

namespace dbat {

    void serializeDbatEntity(entt::entity ent, bool asPrototype, nlohmann::json& j) {

        auto race = registry.try_get<Race>(ent);
        if(race) j["Race"] = race->data;

        auto sensei = registry.try_get<Sensei>(ent);
        if(sensei) j["Sensei"] = sensei->data;

        auto position = registry.try_get<Position>(ent);
        if(position) j["Position"] = position->data;

        auto sex = registry.try_get<Sex>(ent);
        if(sex) j["Sex"] = sex->data;

        auto terrain = registry.try_get<Terrain>(ent);
        if(terrain) j["Terrain"] = terrain->data;

        auto aflags = registry.try_get<AdminFlags>(ent);
        if(aflags && aflags->data.any()) j["AdminFlags"] = bitsetToJson(aflags->data);

        auto oflags = registry.try_get<ObjectFlags>(ent);
        if(oflags && oflags->data.any()) j["ObjectFlags"] = bitsetToJson(oflags->data);

        auto door = registry.try_get<Door>(ent);
        if(door) {
            nlohmann::json d;
            if(door->flags.any()) d["flags"] = bitsetToJson(door->flags);
            if(door->legacyKey) d["legacyKey"] = *door->legacyKey;
            if(door->dclock) d["dclock"] = door->dclock;
            if(door->dchide) d["dchide"] = door->dchide;
            j["Door"] = d;
        }

        auto exits = registry.try_get<Exits>(ent);
        if(exits) {
            for(auto &[dir, dest] : exits->data) {
                j["Exits"].push_back(std::make_pair(dir, dest.serialize()));
            }
        }

        auto doors = registry.try_get<Doors>(ent);
        if(doors) {
            for(auto &[dir, dr] : doors->data) {
                j["Doors"].push_back(std::make_pair(dir, dr.serialize()));
            }
        }

        auto rflags = registry.try_get<RoomFlags>(ent);
        if(rflags && rflags->data.any()) j["RoomFlags"] = bitsetToJson(rflags->data);

        auto size = registry.try_get<Size>(ent);
        if(size) {
            j["Size"] = size->data;
        }

        auto weight = registry.try_get<Weight>(ent);
        if(weight) {
            j["Weight"] = weight->data;
        }

        auto cost = registry.try_get<Cost>(ent);
        if(cost) {
            j["Cost"] = cost->data;
        }

        auto lvlreq = registry.try_get<LevelRequirement>(ent);
        if(lvlreq) {
            j["LevelRequirement"] = lvlreq->data;
        }

        auto ldata = registry.try_get<LightData>(ent);
        if(ldata) {
            nlohmann::json l;
            l["timeLeft"] = ldata->timeLeft;
            l["hoursLeft"] = ldata->hoursLeft;
            j["LightData"] = l;
        }

        auto wdata = registry.try_get<WeaponData>(ent);
        if(wdata) {
            nlohmann::json w;
            w["skill"] = wdata->skill;
            w["damDice"] = wdata->damDice;
            w["damSize"] = wdata->damSize;
            w["damType"] = wdata->damType;
            j["WeaponData"] = w;
        }

        auto wear = registry.try_get<WornData>(ent);
        if(wear) {
            nlohmann::json w;
            if(wear->wearFlags.any()) w["wearFlags"] = bitsetToJson(wear->wearFlags);
            if(wear->wornOn) w["wornOn"] = wear->wornOn.value();
            j["WornData"] = w;
        }

        auto food = registry.try_get<FoodData>(ent);
        if(food) {
            nlohmann::json f;
            f["nutrition"] = food->nutrition;
            f["maxNutrition"] = food->maxNutrition;
            if(food->poisoned) j["poisoned"] = true;
            j["FoodData"] = f;
        }

        auto con = registry.try_get<ContainerData>(ent);
        if(con) {
            nlohmann::json c;
            c["capacity"] = con->capacity;
            if(con->containerFlags.any()) j["containerFlags"] = bitsetToJson(con->containerFlags);
            if(con->key) j["key"] = con->key.value();
            j["ContainerData"] = c;
        }

        auto dr = registry.try_get<DrinkData>(ent);
        if(dr) {
            nlohmann::json d;
            d["capacity"] = dr->capacity;
            d["current"] = dr->current;
            if(dr->poisoned) d["poisoned"] = dr->poisoned;
            d["liquid"] = dr->liquid;
            j["DrinkData"] = d;
        }

        auto dur = registry.try_get<Durability>(ent);
        if(dur) {
            nlohmann::json d;
            d["durability"] = dur->durability;
            d["maxDurability"] = dur->maxDurability;
            j["Durability"] = d;
        }

        auto iflags = registry.try_get<ItemFlags>(ent);
        if(iflags && iflags->data.any()) j["ItemFlags"] = bitsetToJson(iflags->data);

        auto iaflags = registry.try_get<ItemAffects>(ent);
        if(iaflags && iaflags->data.any()) j["ItemAffects"] = bitsetToJson(iaflags->data);

        auto caflags = registry.try_get<CharacterAffects>(ent);
        if(caflags && caflags->data.any()) j["CharacterAffects"] = bitsetToJson(caflags->data);


        auto nflags = registry.try_get<NPCFlags>(ent);
        if(nflags && nflags->data.any()) j["NPCFlags"] = bitsetToJson(nflags->data);


        auto cel = registry.try_get<CelestialBody>(ent);
        if(cel) {
            nlohmann::json c;
            c["type"] = cel->type;
            if(cel->flags.any()) c["flags"] = bitsetToJson(cel->flags);
            if(cel->gravity) c["gravity"] = cel->gravity.value();
            j["CelestialBody"] = c;
        }

        auto exdesc = registry.try_get<ExtraDescriptions>(ent);
        if(exdesc && !exdesc->data.empty()) {
            for(auto &ex : exdesc->data) {
                j["ExtraDescriptions"].push_back(std::make_pair(ex.first, ex.second));
            }
        }

        auto dg = registry.try_get<DgScripts>(ent);
        if(dg && !dg->data.empty()) {
            for(auto &d : dg->data) {
                j["DgScripts"].push_back(d);
            }
        }

        auto im = registry.try_get<ItemModifiers>(ent);
        if(im && !im->data.empty()) {
            for(auto &m : im->data) {
                j["ItemModifiers"].push_back({m.location, m.specific, m.modifier});
            }
        }

        auto ski = registry.try_get<Skills>(ent);
        if(ski && !ski->data.empty()) {
            for(auto &s : ski->data) {
                j["Skills"].push_back(std::make_pair(s.first, s.second.serialize()));
            }
        }

        if(registry.any_of<Dimension>(ent)) j["Dimension"] = true;

        auto npcv = registry.try_get<NPCVnum>(ent);
        if(npcv) {
            j["NPCVnum"] = npcv->data;
        }

        auto itemv = registry.try_get<ItemVnum>(ent);
        if(itemv) {
            j["ItemVnum"] = itemv->data;
        }

        auto charstats = registry.try_get<CharacterStats>(ent);
        if(charstats) {
            for(auto i = 0; i < cstat::numCharStats; i++) {
                if(charstats->data[i] != 0) j["CharacterStats"].push_back({i, charstats->data[i]});
            }
        }
    }

    void deserializeDbatEntity(entt::entity ent, const nlohmann::json& j) {

        if(j.contains("Race")) {
            auto &race = registry.get_or_emplace<Race>(ent);
            race.data = j["Race"];
        }

        if(j.contains("Sensei")) {
            auto &sensei = registry.get_or_emplace<Sensei>(ent);
            sensei.data = j["Sensei"];
        }

        if(j.contains("Position")) {
            auto &pos = registry.get_or_emplace<Position>(ent);
            pos.data = j["Position"];
        }

        if(j.contains("Sex")) {
            auto &sex = registry.get_or_emplace<Sex>(ent);
            sex.data = j["Sex"];
        }

        if(j.contains("Terrain")) {
            auto &terrain = registry.get_or_emplace<Terrain>(ent);
            terrain.data = j["Terrain"];
        }

        if(j.contains("AdminFlags")) {
            auto &flags = registry.get_or_emplace<AdminFlags>(ent);
            jsonToBitset(j["AdminFlags"], flags.data);
        }

        if(j.contains("ObjectFlags")) {
            auto &flags = registry.get_or_emplace<ObjectFlags>(ent);
            jsonToBitset(j["ObjectFlags"], flags.data);
        }

        if(j.contains("Exits")) {
            auto &exits = registry.get_or_emplace<Exits>(ent);
            for(auto &exdata : j["Exits"]) {
                auto d = exdata[0].get<dir::DirectionId>();
                exits.data.emplace(d, exdata[1]);
            }
        }

        if(j.contains("Doors")) {
            auto &doors = registry.get_or_emplace<Doors>(ent);
            for(auto &exdata : j["Doors"]) {
                auto d = exdata[0].get<dir::DirectionId>();
                doors.data.emplace(d, exdata[1]);
            }
        }

        if(j.contains("RoomFlags")) {
            auto &flags = registry.get_or_emplace<RoomFlags>(ent);
            jsonToBitset(j["RoomFlags"], flags.data);
        }

        if(j.contains("Size")) {
            auto &size = registry.get_or_emplace<Size>(ent);
            size.data = j["Size"];
        }

        if(j.contains("Weight")) {
            auto &weight = registry.get_or_emplace<Weight>(ent);
            weight.data = j["Weight"];
        }

        if(j.contains("LightData")) {
            auto &light = registry.get_or_emplace<LightData>(ent);
            auto ld = j["LightData"];
            if(ld.contains("timeLeft")) light.timeLeft = ld["timeLeft"];
            if(ld.contains("hoursLeft")) light.hoursLeft = ld["hoursLeft"];
        }

        if(j.contains("WeaponData")) {
            auto &w = registry.get_or_emplace<WeaponData>(ent);
            auto wd = j["WeaponData"];
            if(wd.contains("skill")) w.skill = wd["skill"];
            if(wd.contains("damDice")) w.damDice = wd["damDice"];
            if(wd.contains("damSize")) w.damSize = wd["damSize"];
            if(wd.contains("damType")) w.damType = wd["damType"];
        }

        if(j.contains("WornData")) {
            auto &w = registry.get_or_emplace<WornData>(ent);
            auto wd = j["WornData"];
            if(wd.contains("wearFlags")) jsonToBitset(wd["wearFlags"], w.wearFlags);
            if(wd.contains("wornOn")) w.wornOn = wd["wornOn"];
        }

        if(j.contains("FoodData")) {
            auto &f = registry.get_or_emplace<FoodData>(ent);
            auto fd = j["FoodData"];
            if(fd.contains("nutrition")) f.nutrition = fd["nutrition"];
            if(fd.contains("maxNutrition")) f.maxNutrition = fd["maxNutrition"];
            if(fd.contains("poisoned")) f.poisoned = fd["poisoned"];
        }

        if(j.contains("ContainerData")) {
            auto &c = registry.get_or_emplace<ContainerData>(ent);
            auto cd = j["ContainerData"];
            if(cd.contains("capacity")) c.capacity = cd["capacity"];
            if(cd.contains("containerFlags")) jsonToBitset(cd["containerFlags"], c.containerFlags);
            if(cd.contains("key")) c.key = cd["key"];
        }

        if(j.contains("DrinkData")) {
            auto &d = registry.get_or_emplace<DrinkData>(ent);
            auto dd = j["DrinkData"];
            if(dd.contains("capacity")) d.capacity = dd["capacity"];
            if(dd.contains("current")) d.current = dd["current"];
            if(dd.contains("liquid")) d.liquid = dd["liquid"];
            if(dd.contains("poisoned")) d.poisoned = dd["poisoned"];
        }

        if(j.contains("Durability")) {
            auto &d = registry.get_or_emplace<Durability>(ent);
            auto dd = j["Durability"];
            if(dd.contains("durability")) d.durability = dd["durability"];
            if(dd.contains("maxDurability")) d.maxDurability = dd["maxDurability"];
        }

        if(j.contains("ItemFlags")) {
            auto &flags = registry.get_or_emplace<ItemFlags>(ent);
            jsonToBitset(j["ItemFlags"], flags.data);
        }

        if(j.contains("ItemAffects")) {
            auto &affects = registry.get_or_emplace<ItemAffects>(ent);
            jsonToBitset(j["ItemAffects"], affects.data);
        }

        if(j.contains("CharacterAffects")) {
            auto &affects = registry.get_or_emplace<CharacterAffects>(ent);
            jsonToBitset(j["CharacterAffects"], affects.data);
        }

        if(j.contains("NPCFlags")) {
            auto &flags = registry.get_or_emplace<NPCFlags>(ent);
            jsonToBitset(j["NPCFlags"], flags.data);
        }

        if(j.contains("CelestialBody")) {
            auto &cb = registry.get_or_emplace<CelestialBody>(ent);
            auto cbd = j["CelestialBody"];
            if(cbd.contains("type")) cb.type = cbd["type"];
            if(cbd.contains("flags")) jsonToBitset(cbd["flags"], cb.flags);
            if(cbd.contains("gravity")) cb.gravity = cbd["gravity"];
        }

        if(j.contains("ExtraDescriptions")) {
            auto &eds = registry.get_or_emplace<ExtraDescriptions>(ent);
            auto ed = j["ExtraDescriptions"];
            // ed should be an iterable of two-element tuples/arrays which are both strings.
            // It will be iterated to generate the actual ExtraDescription components.
            for(auto &i : ed) {
                auto &desc = eds.data.emplace_back();
                desc.first = intern(i[0]);
                desc.second = intern(i[1]);
            }
        }

        if(j.contains("DgScripts")) {
            // This is an array of integers. fill up the DgScripts component's data vector.
            auto &dg = registry.get_or_emplace<DgScripts>(ent);
            auto dgj = j["DgScripts"];
            for(auto &i : dgj) {
                dg.data.push_back(i);
            }
        }

        if(j.contains("ItemModifiers")) {
            // This is an array of 3-element arrays corresponding to obj_affected_type's
            // location, specific, and modifier fields.
            auto &mods = registry.get_or_emplace<ItemModifiers>(ent);
            auto modj = j["ItemModifiers"];
            for(auto &m : modj) {
                mods.data.emplace_back(m);
            }
        }

        if(j.contains("Dimension")) {
            registry.get_or_emplace<Dimension>(ent);
        }

        if(j.contains("NPCVnum")) {
            auto &vnum = registry.get_or_emplace<NPCVnum>(ent);
            vnum.data = j["NPCVnum"];
        }

        if(j.contains("ItemVnum")) {
            auto &vnum = registry.get_or_emplace<ItemVnum>(ent);
            vnum.data = j["ItemVnum"];
        }

        if(j.contains("CharacterStats")) {
            auto &stats = registry.get_or_emplace<CharacterStats>(ent);
            // iterate through the pairs of stat ids and values to set stats.data...
            auto statj = j["CharacterStats"];
            for(auto &s : statj) {
                stats.data[s[0]] = s[1];
            }
        }

    }

    std::vector<std::string> extraSchema = {

            "CREATE TABLE IF NOT EXISTS zones ("
            "   id INTEGER PRIMARY KEY,"
            "   data TEXT NOT NULL"
            ");",

            "CREATE TABLE IF NOT EXISTS scripts ("
            "   id INTEGER PRIMARY KEY,"
            "   data TEXT NOT NULL"
            ");",

            "CREATE TABLE IF NOT EXISTS legacySpaceRooms ("
            "   id INTEGER PRIMARY KEY,"
            "   x INTEGER NOT NULL,"
            "   y INTEGER NOT NULL,"
            "   z INTEGER NOT NULL,"
            "   UNIQUE(x,y,z)"
            ");"
    };

    void loadLegacySpace() {
        broadcast("Loading legacy space...  amoebas and all.");
        SQLite::Statement q(*db, "SELECT * from legacySpaceRooms;");
        while(q.executeStep()) {
            auto id = q.getColumn("id").getInt64();
            auto x = q.getColumn("x").getInt64();
            auto y = q.getColumn("y").getInt64();
            auto z = q.getColumn("z").getInt64();
            legacySpaceRooms.emplace(id, GridPoint(x, y, z));

        }
    }

    void saveLegacySpace() {
        SQLite::Statement q(*db, "INSERT OR REPLACE INTO legacySpaceRooms (id, x, y, z) VALUES (?, ?, ?, ?);");
        for(auto &i : legacySpaceRooms) {
            q.bind(1, static_cast<int64_t>(i.first));
            q.bind(2, i.second.x);
            q.bind(3, i.second.y);
            q.bind(4, i.second.z);
            q.exec();
            q.reset();
        }
    }

    void loadZones() {
        // let's retrieve the count of zones.
        SQLite::Statement q1(*db, "SELECT COUNT(*) FROM zones;");
        q1.executeStep();
        auto count = q1.getColumn(0).getInt64();
        zones.reserve(count);
        broadcast(fmt::format("Loading {} Legacy Zones...", count));

        SQLite::Statement q(*db, "SELECT id, data from zones;");
        while(q.executeStep()) {
            auto id = q.getColumn(0).getInt64();
            auto data = q.getColumn(1).getText();
            auto ent = registry.create();
            zones.emplace(id, nlohmann::json::parse(data));
        }
    }

    void loadScripts() {
        // Let's retrieve the count of scripts.
        SQLite::Statement q1(*db, "SELECT COUNT(*) FROM scripts;");
        q1.executeStep();
        auto count = q1.getColumn(0).getInt64();
        dgScripts.reserve(count);
        broadcast(fmt::format("Loading {} Legacy DgScripts...", count));

        SQLite::Statement q(*db, "SELECT id, data from scripts;");
        while(q.executeStep()) {
            auto id = q.getColumn(0).getInt64();
            auto data = q.getColumn(1).getText();
            auto ent = registry.create();
            dgScripts[id] = std::make_shared<DgScriptPrototype>(nlohmann::json::parse(data));
        }
    }

    void saveZones() {
        SQLite::Statement q(*db, "INSERT OR REPLACE INTO zones (id, data) VALUES (?, ?);");
        for(auto &[id, z] : zones) {
            q.bind(1, static_cast<int64_t>(id));
            auto data = z.serialize();
            q.bind(2, data.dump(4, ' ', false, nlohmann::json::error_handler_t::ignore));
            q.exec();
            q.reset();
        }
    }

    void saveZone(std::size_t id) {
        SQLite::Statement q(*db, "INSERT OR REPLACE INTO zones (id, data) VALUES (?, ?);");
        auto &z = zones[id];
        q.bind(1, static_cast<int64_t>(id));
        auto data = z.serialize();
        q.bind(2, data.dump(4, ' ', false, nlohmann::json::error_handler_t::ignore));
        q.exec();
        q.reset();
    }

    void saveScripts() {
        SQLite::Statement q(*db, "INSERT OR REPLACE INTO scripts (id, data) VALUES (?, ?);");
        for(auto &[id, s] : dgScripts) {
            q.bind(1, static_cast<int64_t>(id));
            auto data = s->serialize();
            q.bind(2, data.dump(4, ' ', false, nlohmann::json::error_handler_t::ignore));
            q.exec();
            q.reset();
        }
    }

    void saveScript(std::size_t id) {
        SQLite::Statement q(*db, "INSERT OR REPLACE INTO scripts (id, data) VALUES (?, ?) ON CONFLICT(name) DO UPDATE set data=VALUES(data)");
        auto &s = dgScripts[id];
        q.bind(1, static_cast<int64_t>(id));
        auto data = s->serialize();
        q.bind(2, data.dump(4, ' ', false, nlohmann::json::error_handler_t::ignore));
        q.exec();
        q.reset();
    }

}