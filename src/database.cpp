#include "dbatk/database.h"
#include "dbatk/components.h"
#include "dbatk/api.h"
#include "dbatk/conf.h"
#include "dbatk/zone.h"
#include "dbatk/dgscript.h"

namespace dbat {

    nlohmann::json serializeEntity(entt::entity ent, bool asPrototype) {
        nlohmann::json j;

        auto name = registry.try_get<Name>(ent);
        if (name) {
            j["Name"] = name->data;
        }

        auto shortDescription = registry.try_get<ShortDescription>(ent);
        if (shortDescription) {
            j["ShortDescription"] = shortDescription->data;
        }

        auto roomDescription = registry.try_get<RoomDescription>(ent);
        if (roomDescription) {
            j["RoomDescription"] = roomDescription->data;
        }

        auto lookDescription = registry.try_get<LookDescription>(ent);
        if(lookDescription) {
            j["LookDescription"] = lookDescription->data;
        }

        if(!asPrototype) {
            // Nab relationships...
            auto location = registry.try_get<Location>(ent);
            if (location) {
                j["Location"] = registry.get<ObjectId>(location->data);
            }

            auto parent = registry.try_get<Parent>(ent);
            if (parent) {
                j["Parent"] = registry.get<ObjectId>(parent->data);
            }

            auto owner = registry.try_get<Owner>(ent);
            if (owner) {
                j["Owner"] = registry.get<ObjectId>(owner->data);
            }
        }

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

        auto rooms = registry.try_get<Rooms>(ent);
        if(rooms) {
            for(auto &[rid, room] : rooms->data) {
                j["Rooms"].push_back(std::make_pair(rid, serializeEntity(room, asPrototype)));
            }
        }

        auto rflags = registry.try_get<RoomFlags>(ent);
        if(rflags && rflags->data.any()) j["RoomFlags"] = bitsetToJson(rflags->data);

        auto gbounds = registry.try_get<GridBounds>(ent);
        if(gbounds) {
            nlohmann::json g;
            if(gbounds->minX) g["minX"] = gbounds->minX.value();
            if(gbounds->minY) g["minY"] = gbounds->minY.value();
            if(gbounds->minZ) g["minZ"] = gbounds->minZ.value();
            if(gbounds->maxX) g["maxX"] = gbounds->maxX.value();
            if(gbounds->maxY) g["maxY"] = gbounds->maxY.value();
            if(gbounds->maxZ) g["maxZ"] = gbounds->maxZ.value();
            if(g.size() > 0) j["GridBounds"] = g;
        }

        auto gpoi = registry.try_get<GridPointsOfInterest>(ent);
        if(gpoi) {
            for(auto &[coor, poi] : gpoi->data) {
                nlohmann::json p;
                p.push_back(coor.serialize());
                p.push_back(serializeEntity(poi, asPrototype));
                j["GridPointsOfInterest"].push_back(p);
            }
        }

        auto gloc = registry.try_get<GridLocation>(ent);
        if(gloc) {
            j["GridLocation"] = gloc->data.serialize();
        }

        auto rloc = registry.try_get<RoomLocation>(ent);
        if(rloc) {
            j["RoomLocation"] = rloc->id;
        }

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

        auto character = registry.try_get<Character>(ent);
        if(character) {
            nlohmann::json c;
            for(auto i = 0; i < cstat::numCharStats; i++) {
                if(character->stats[i] != 0.0) {
                    c["stats"].push_back(std::make_pair(i, character->stats[i]));
                }
            }
            j["Character"] = c;
        }

        auto player = registry.try_get<Player>(ent);
        if(player) {
            j["Player"]["accountId"] = player->accountId;
        }

        auto item = registry.try_get<Item>(ent);
        if(item) {
            nlohmann::json it;
            if(item->vnum) it["vnum"] = item->vnum.value();
            j["Item"] = it;
        }

        auto npc = registry.try_get<NPC>(ent);
        if(npc) {
            nlohmann::json n;
            if(npc->vnum) n["vnum"] = npc->vnum.value();
            j["NPC"] = n;
        }

        auto room = registry.try_get<Room>(ent);
        if(room) {
            nlohmann::json r;
            r["id"] = room->id;
            r["obj"] = room->obj;
            j["Room"] = r;
        }
        if(registry.any_of<Dimension>(ent)) j["Dimension"] = true;
        if(registry.any_of<Expanse>(ent)) j["Expanse"] = true;
        if(registry.any_of<Area>(ent)) j["Area"] = true;
        if(registry.any_of<Vehicle>(ent)) j["Vehicle"] = true;

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

        return j;
    }

    void deserializeEntity(entt::entity ent, const nlohmann::json& j) {
        //logger->info("Deserializing a: {}", j.dump());
        if(j.contains("Name")) {
            registry.emplace<Name>(ent, j["Name"]);
        }
        if(j.contains("ShortDescription")) {
            registry.emplace<ShortDescription>(ent, j["ShortDescription"]);
        }
        if(j.contains("RoomDescription")) {
            registry.emplace<RoomDescription>(ent, j["RoomDescription"]);
        }
        if(j.contains("LookDescription")) {
            registry.emplace<LookDescription>(ent, j["LookDescription"]);
        }

        if(j.contains("Location")) {
            ObjectId loc(j["Location"]);
            setLocation(ent, loc.getObject());
        }

        if(j.contains("Parent")) {
            ObjectId parent(j["Parent"]);
            setParent(ent, parent.getObject());
        }

        if(j.contains("Owner")) {
            ObjectId owner(j["Owner"]);
            setOwner(ent, owner.getObject());
        }

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

        if(j.contains("Rooms")) {
            auto &rooms = registry.get_or_emplace<Rooms>(ent);
            auto &o = registry.get<ObjectId>(ent);
            auto of = registry.try_get<ObjectFlags>(ent);
            bool groom = of && of->data.test(oflags::GLOBALROOMS);
            for(auto &exdata : j["Rooms"]) {
                auto r = exdata[0].get<RoomId>();
                auto room = registry.create();
                rooms.data.emplace(r, room);
                deserializeEntity(room, exdata[1]);
                auto &rm = registry.get_or_emplace<Room>(room);
                rm.obj = o;
                rm.id = r;
                if(groom) legacyRooms[r] = room;
            }
        }

        if(j.contains("RoomFlags")) {
            auto &flags = registry.get_or_emplace<RoomFlags>(ent);
            jsonToBitset(j["RoomFlags"], flags.data);
        }

        if(j.contains("GridBounds")) {
            auto &bounds = registry.get_or_emplace<GridBounds>(ent);
            auto jb = j["GridBounds"];
            if(jb.contains("minX")) bounds.minX = jb["minX"];
            if(jb.contains("maxX")) bounds.maxX = jb["maxX"];
            if(jb.contains("minY")) bounds.minY = jb["minY"];
            if(jb.contains("maxY")) bounds.maxY = jb["maxY"];
            if(jb.contains("minZ")) bounds.minZ = jb["minZ"];
            if(jb.contains("maxZ")) bounds.maxZ = jb["maxZ"];
        }

        if(j.contains("GridPOintsOfInterest")) {
            auto &poi = registry.get_or_emplace<GridPointsOfInterest>(ent);
            for(auto &exdata : j["GridPointsOfInterest"]) {
                auto p = registry.create();
                poi.data.emplace(exdata[0], p);
                deserializeEntity(p, exdata[1]);
            }
        }

        if(j.contains("GridLocation")) {
            auto &gloc = registry.get_or_emplace<GridLocation>(ent, j["GridLocation"]);
        }

        if(j.contains("RoomLocation")) {
            auto &rloc = registry.get_or_emplace<RoomLocation>(ent);
            rloc.id = j["RoomLocation"];
            // TODO: Place ent in proper Room
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

        if(j.contains("Item")) {
            auto data = j["Item"];
            auto &item = registry.get_or_emplace<Item>(ent);
            if(data.contains("vnum")) item.vnum = data["vnum"];
        }

        if(j.contains("Character")) {
            auto data = j["Character"];
            auto &c = registry.get_or_emplace<Character>(ent);
            if(data.contains("stats")) {
                for(auto &statdat : data["stats"]) {
                    auto st = statdat[0].get<int>();
                    c.stats[st] = statdat[1].get<double>();
                }
            }

        }

        if(j.contains("NPC")) {
            auto data = j["NPC"];
            auto &npc = registry.get_or_emplace<NPC>(ent);
            if(data.contains("vnum")) npc.vnum = data["vnum"];
        }

        if(j.contains("Player")) {
            auto &pdata = j["Player"];
            auto &player = registry.get_or_emplace<Player>(ent);
            player.accountId = pdata["accountId"];
        }

        if(j.contains("Room")) {
            auto &rdata = j["Room"];
            auto &room = registry.get_or_emplace<Room>(ent);
            room.id = rdata["id"];
            room.obj = ObjectId(rdata["obj"]);
        }

        if(j.contains("Dimension")) {
            registry.get_or_emplace<Dimension>(ent);
        }

        if(j.contains("Expanse")) {
            registry.get_or_emplace<Expanse>(ent);
        }

        if(j.contains("Area")) {
            registry.get_or_emplace<Area>(ent);
        }

        if(j.contains("Vehicle")) {
            registry.get_or_emplace<Vehicle>(ent);
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

    }

    std::unique_ptr<SQLite::Database> db;

    std::vector<std::string> schema = {
            "CREATE TABLE IF NOT EXISTS objects ("
            "   id INTEGER PRIMARY KEY,"
            "   generation INTEGER NOT NULL,"
            "   data TEXT NOT NULL,"
            "   UNIQUE(id, generation)"
            ");",

            "CREATE TABLE IF NOT EXISTS prototypes ("
            "   id INTEGER PRIMARY KEY,"
            "   name TEXT NOT NULL UNIQUE COLLATE NOCASE,"
            "   data TEXT NOT NULL"
            ");",

            "CREATE TABLE IF NOT EXISTS accounts ("
            "   id INTEGER PRIMARY KEY,"
            "   username TEXT NOT NULL UNIQUE COLLATE NOCASE,"
            "   password TEXT NOT NULL DEFAULT '',"
            "   email TEXT NOT NULL DEFAULT '',"
            "   created INTEGER NOT NULL DEFAULT (strftime('%s','now')),"
            "   lastLogin INTEGER NOT NULL DEFAULT (strftime('%s','now')),"
            "   lastLogout INTEGER NOT NULL DEFAULT (strftime('%s','now')),"
            "   lastPasswordChanged INTEGER NOT NULL DEFAULT (strftime('%s','now')),"
            "   totalPlayTime REAL NOT NULL DEFAULT 0,"
            "   totalLoginTime REAL NOT NULL DEFAULT 0,"
            "   disabledReason TEXT NOT NULL DEFAULT '',"
            "   disabledUntil INTEGER NOT NULL DEFAULT 0,"
            "   adminLevel INTEGER NOT NULL DEFAULT 0"
            ");",

            "CREATE TABLE IF NOT EXISTS playerCharacters ("
            "   character INTEGER NOT NULL,"
            "   account INTEGER NOT NULL,"
            "   lastLogin INTEGER NOT NULL DEFAULT (strftime('%s','now')),"
            "   lastLogout INTEGER NOT NULL DEFAULT (strftime('%s','now')),"
            "   totalPlayTime REAL NOT NULL DEFAULT 0,"
            "   FOREIGN KEY(account) REFERENCES accounts(id) ON UPDATE CASCADE ON DELETE CASCADE,"
            "   FOREIGN KEY(character) REFERENCES objects(id) ON UPDATE CASCADE ON DELETE CASCADE,"
            "   PRIMARY KEY(character)"
            ");",

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

    // Presumably this will only be called if there ARE any dirties.
    void processDirty() {

        SQLite::Statement q1(*db, "INSERT OR REPLACE INTO objects (id, generation, data) VALUES (?, ?, ?);");
        SQLite::Statement q2(*db, "DELETE FROM objects WHERE id = ? AND generation = ?;");

        for(auto &obj : dirty) {
            auto ent = obj.getObject();
            if(registry.valid(ent)) {
                q1.bind(1, static_cast<int64_t>(obj.index));
                q1.bind(2, obj.generation);
                q1.bind(3, serializeEntity(ent).dump(4, ' ', false, nlohmann::json::error_handler_t::ignore));
                q1.exec();
                q1.reset();
            } else {
                q2.bind(1, static_cast<int64_t>(obj.index));
                q2.bind(2, obj.generation);
                q2.exec();
                q2.reset();
            }
        }

        dirty.clear();

    }

    void readyDatabase() {
        db = std::make_unique<SQLite::Database>(conf::dbName, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        SQLite::Transaction trans(*db);
        for(auto &s : schema) {
            db->exec(s);
        }

        trans.commit();
    }

    void loadLegacySpace() {
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

    void loadObjects() {
        // Step 1: get max id of objects table.
        SQLite::Statement q(*db, "SELECT MAX(id) FROM objects;");
        q.executeStep();
        auto count = q.getColumn(0).getInt64();

        // Reserve RAM for a single easy allocation to make this simple...
        objects.resize(count + 50, {0, entt::null});

        SQLite::Statement q1(*db, "SELECT id, generation FROM objects;");
        while(q1.executeStep()) {
            auto id = q1.getColumn(0).getInt64();
            auto gen = q1.getColumn(1).getInt64();
            auto ent = registry.create();
            registry.emplace<ObjectId>(ent, id, gen);
            objects[id] = {gen, ent};
        }

        // Now we're gonna need to select id and data from objects to deserialize.
        SQLite::Statement q2(*db, "SELECT id, data FROM objects;");
        while(q2.executeStep()) {
            auto id = q2.getColumn(0).getInt64();
            auto data = q2.getColumn(1).getText();
            auto ent = objects[id].second;
            deserializeEntity(ent, nlohmann::json::parse(data));
        }

    }

    void loadZones() {
        // let's retrieve the count of zones.
        SQLite::Statement q1(*db, "SELECT COUNT(*) FROM zones;");
        q1.executeStep();
        auto count = q1.getColumn(0).getInt64();
        zones.reserve(count);

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

        SQLite::Statement q(*db, "SELECT id, data from scripts;");
        while(q.executeStep()) {
            auto id = q.getColumn(0).getInt64();
            auto data = q.getColumn(1).getText();
            auto ent = registry.create();
            dgScripts[id] = std::make_shared<DgScriptPrototype>(nlohmann::json::parse(data));
        }
    }

    void loadDatabase() {
        loadZones();
        loadScripts();
        loadLegacySpace();
        loadObjects();
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

    void savePrototype(const std::string& name, const nlohmann::json& j) {
        SQLite::Statement q(*db, "INSERT OR REPLACE INTO prototypes (name, data) VALUES (?, ?) ON CONFLICT(name) DO UPDATE set data=VALUES(data)");
        q.bind(1, name);
        q.bind(2, j.dump(4, ' ', false, nlohmann::json::error_handler_t::ignore));
        q.exec();
        q.reset();
    }

    std::optional<nlohmann::json> getPrototype(const std::string& name) {
        SQLite::Statement q(*db, "SELECT data FROM prototypes WHERE name = ?;");
        q.bind(1, name);
        if(q.executeStep()) {
            auto data = q.getColumn(0).getText();
            return nlohmann::json::parse(data);
        }
        return std::nullopt;
    }

}