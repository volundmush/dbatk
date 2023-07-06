#include "dbatk/database.h"
#include "dbatk/components.h"
#include "dbatk/zone.h"
#include "dbatk/dgscript.h"
#include "dbatk/link.h"
#include "dbatk/api.h"
#include "dbatk/config.h"
#include "fmt/format.h"

namespace dbat {

    static std::unordered_set<ObjectId> dirty;

    void setDirty(entt::entity ent, bool override) {
        if(!registry.valid(ent)) return;
        auto objid = registry.try_get<ObjectId>(ent);
        if(!objid) {
            return;
        }
        setDirty(*objid, override);
    }

    void setDirty(const ObjectId& id, bool override) {
        if(gameIsLoading && !override) return;
        dirty.insert(id);
    }

    entt::entity PrototypeData::spawn() {
        auto ent = createObject();
        deserializeEntity(ent, data);
        return ent;
    }

    std::string PrototypeData::entityName() {
        if(data.contains("Name")) return data["Name"];
        return name;
    }

    std::unordered_map<std::string, std::shared_ptr<PrototypeData>> prototypes;

    nlohmann::json serializeEntity(entt::entity ent, bool asPrototype) {
        nlohmann::json j;

        if (auto name = registry.try_get<Name>(ent); name) {
            j["Name"] = name->data;
        }

        if (auto key = registry.try_get<Keywords>(ent); key) {
            j["Keywords"] = key->data;
        }

        if (auto roomDescription = registry.try_get<RoomDescription>(ent); roomDescription) {
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
                j["Location"] = location->serialize();
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


        auto area = registry.try_get<Area>(ent);
        if(area) {
            nlohmann::json rooms;
            for(auto &[rid, room] : area->data) {
                rooms.push_back(std::make_pair(rid, serializeEntity(room, asPrototype)));
            }
            j["Area"] = rooms;
        }

        auto expanse = registry.try_get<Expanse>(ent);
        if(expanse) {
            nlohmann::json e;
            e["minX"] = expanse->minX;
            e["minY"] = expanse->minY;
            e["minZ"] = expanse->minZ;
            e["maxX"] = expanse->maxX;
            e["maxY"] = expanse->maxY;
            e["maxZ"] = expanse->maxZ;

            for(auto &[coor, poi] : expanse->poi) {
                nlohmann::json p;
                p.push_back(coor.serialize());
                p.push_back(serializeEntity(poi, asPrototype));
                e["poi"].push_back(p);
            }
            j["Expanse"] = e;
        }

        auto map = registry.try_get<Map>(ent);
        if(map) {
            nlohmann::json e;
            e["minX"] = map->minX;
            e["minY"] = map->minY;
            e["minZ"] = map->minZ;
            e["maxX"] = map->maxX;
            e["maxY"] = map->maxY;
            e["maxZ"] = map->maxZ;

            for(auto &[coor, poi] : map->poi) {
                nlohmann::json p;
                p.push_back(coor.serialize());
                p.push_back(serializeEntity(poi, asPrototype));
                e["poi"].push_back(p);
            }
            j["Map"] = e;
        }

        auto space = registry.try_get<Space>(ent);
        if(space) {
            nlohmann::json e;
            e["minX"] = space->minX;
            e["minY"] = space->minY;
            e["minZ"] = space->minZ;
            e["maxX"] = space->maxX;
            e["maxY"] = space->maxY;
            e["maxZ"] = space->maxZ;

            for(auto &[coor, poi] : space->poi) {
                nlohmann::json p;
                p.push_back(coor.serialize());
                p.push_back(serializeEntity(poi, asPrototype));
                e["poi"].push_back(p);
            }
            j["Space"] = e;
        }

        auto player = registry.try_get<Player>(ent);
        if(player) {
            j["Player"]["accountId"] = player->accountId;
        }

        auto room = registry.try_get<Room>(ent);
        if(room) {
            nlohmann::json r;
            r["id"] = room->id;
            r["obj"] = room->obj;
            j["Room"] = r;
        }

        if(registry.any_of<Character>(ent)) j["Character"] = true;
        if(registry.any_of<NPC>(ent)) j["NPC"] = true;
        if(registry.any_of<Item>(ent)) j["Item"] = true;

        if(registry.any_of<Vehicle>(ent)) j["Vehicle"] = true;

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

        if(auto rflags = registry.try_get<RoomFlags>(ent); rflags) {
            if(rflags->data.any()) j["RoomFlags"] = bitsetToJson(rflags->data);
        }

        auto size = registry.try_get<Size>(ent);
        if(size) {
            j["Size"] = size->data;
        }

        auto weight = registry.try_get<Weight>(ent);
        if(weight) {
            j["Weight"] = weight->data;
        }


        if(auto cost = registry.try_get<Cost>(ent); cost) {
            j["Cost"] = cost->data;
        }

        if(auto cost = registry.try_get<CostPerDay>(ent); cost) {
            j["CostPerDay"] = cost->data;
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

        if(auto charworn = registry.try_get<CharWornData>(ent); charworn) {
            if(charworn->data.any()) j["CharWornData"] = bitsetToJson(charworn->data);
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
            j["CelestialBody"] = c;
        }

        auto grav = registry.try_get<Gravity>(ent);
        if(grav) {
            j["Gravity"] = grav->data;
        }

        auto exdesc = registry.try_get<ExtraDescriptions>(ent);
        if(exdesc && !exdesc->data.empty()) {
            for(auto &ex : exdesc->data) {
                j["ExtraDescriptions"].push_back(std::make_pair(ex.first, ex.second));
            }
        }


        if(auto dg = registry.try_get<DgScripts>(ent); dg) {
            j["DgScripts"] = dg->serialize();
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

        auto legload = registry.try_get<LegacyLoadRoom>(ent);
        if(legload) {
            j["LegacyLoadRoom"] = legload->id;
        }

        auto proto = registry.try_get<Prototype>(ent);
        if(proto) {
            j["Prototype"] = proto->data;
        }

        if(auto money = registry.try_get<Money>(ent); money) {
            j["Money"] = money->data;
        }

        if(auto money = registry.try_get<MoneyPile>(ent); money) {
            j["MoneyPile"] = money->data;
        }

        return j;
    }

    void deserializeEntity(entt::entity ent, const nlohmann::json& j) {
        if(j.contains("Name")) {
            registry.emplace<Name>(ent, j["Name"]);
        }
        if(j.contains("Keywords")) {
            auto &k = registry.get_or_emplace<Keywords>(ent);
            k.data = intern(j["Keywords"].get<std::string>());
        }
        if(j.contains("RoomDescription")) {
            registry.emplace<RoomDescription>(ent, j["RoomDescription"]);
        }
        if(j.contains("LookDescription")) {
            registry.emplace<LookDescription>(ent, j["LookDescription"]);
        }

        if(j.contains("Location")) {
            auto &loc = registry.emplace<Location>(ent, j["Location"]);
        }

        if(j.contains("Parent")) {
            ObjectId parent(j["Parent"]);
            setParent(ent, parent.getObject());
        }

        if(j.contains("Owner")) {
            ObjectId owner(j["Owner"]);
            setOwner(ent, owner.getObject());
        }

        if(j.contains("Area")) {
            auto &rooms = registry.get_or_emplace<Area>(ent);
            auto &o = registry.get<ObjectId>(ent);
            for(auto &exdata : j["Area"]) {
                auto r = exdata[0].get<RoomId>();
                auto room = registry.create();
                rooms.data.emplace(r, room);
                deserializeEntity(room, exdata[1]);
                auto &rm = registry.get_or_emplace<Room>(room);
                rm.obj = o;
                rm.id = r;
            }
        }

        if(j.contains("Expanse")) {
            auto &exp = registry.get_or_emplace<Expanse>(ent);
            auto &data = j["Expanse"];
            if(data.contains("minX")) exp.minX = data["minX"];
            if(data.contains("minY")) exp.minY = data["minY"];
            if(data.contains("minZ")) exp.minZ = data["minZ"];
            if(data.contains("maxX")) exp.maxX = data["maxX"];
            if(data.contains("maxY")) exp.maxY = data["maxY"];
            if(data.contains("maxZ")) exp.maxZ = data["maxZ"];
            if(data.contains("poi")) {
                for(auto &poi : data["poi"]) {
                    GridPoint gp(poi[0]);
                    auto p = registry.create();
                    exp.poi.emplace(gp, p);
                    deserializeEntity(p, poi[1]);
                }
            }
        }

        if(j.contains("Map")) {
            auto &exp = registry.get_or_emplace<Map>(ent);
            auto &data = j["Map"];
            if(data.contains("minX")) exp.minX = data["minX"];
            if(data.contains("minY")) exp.minY = data["minY"];
            if(data.contains("minZ")) exp.minZ = data["minZ"];
            if(data.contains("maxX")) exp.maxX = data["maxX"];
            if(data.contains("maxY")) exp.maxY = data["maxY"];
            if(data.contains("maxZ")) exp.maxZ = data["maxZ"];
            if(data.contains("poi")) {
                for(auto &poi : data["poi"]) {
                    GridPoint gp(poi[0]);
                    auto p = registry.create();
                    exp.poi.emplace(gp, p);
                    deserializeEntity(p, poi[1]);
                }
            }
        }

        if(j.contains("Space")) {
            auto &exp = registry.get_or_emplace<Space>(ent);
            auto &data = j["Space"];
            if(data.contains("minX")) exp.minX = data["minX"];
            if(data.contains("minY")) exp.minY = data["minY"];
            if(data.contains("minZ")) exp.minZ = data["minZ"];
            if(data.contains("maxX")) exp.maxX = data["maxX"];
            if(data.contains("maxY")) exp.maxY = data["maxY"];
            if(data.contains("maxZ")) exp.maxZ = data["maxZ"];
            if(data.contains("poi")) {
                for(auto &poi : data["poi"]) {
                    SectorPoint gp(poi[0]);
                    auto p = registry.create();
                    exp.poi.emplace(gp, p);
                    deserializeEntity(p, poi[1]);
                }
            }
        }

        if(j.contains("Item")) {
            registry.get_or_emplace<Item>(ent);
        }

        if(j.contains("Character")) {
            registry.get_or_emplace<Character>(ent);
        }

        if(j.contains("NPC")) {
            registry.get_or_emplace<NPC>(ent);
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

        if(j.contains("Vehicle")) {
            registry.get_or_emplace<Vehicle>(ent);
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

        if(j.contains("CharWornData")) {
            auto &flags = registry.get_or_emplace<CharWornData>(ent);
            jsonToBitset(j["CharWornData"], flags.data);
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
        }

        if(j.contains("Gravity")) {
            auto &g = registry.get_or_emplace<Gravity>(ent);
            g.data = j["Gravity"];
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
            auto &dg = registry.get_or_emplace<DgScripts>(ent, ent, j["DgScripts"]);

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

        if(j.contains("LegacyLoadRoom")) {
            auto &lr = registry.get_or_emplace<LegacyLoadRoom>(ent);
            lr.id = j["LegacyLoadRoom"];
        }

        if(j.contains("Prototype")) {
            auto &proto = registry.get_or_emplace<Prototype>(ent);
            std::string p = j["Prototype"];
            proto.data = intern(p);
            auto find = prototypes.find(p);
            if(find != prototypes.end()) {
                // this is a new prototype, so we need to create a new protoTracker entry for it.
                find->second->instanceCount++;
            }
        }

        if(j.contains("Money")) {
            auto &money = registry.get_or_emplace<Money>(ent);
            money.data = j["Money"];
        }

        if(j.contains("MoneyPile")) {
            auto &money = registry.get_or_emplace<MoneyPile>(ent);
            money.data = j["MoneyPile"];
        }

        if(j.contains("Cost")) {
            auto &cost = registry.get_or_emplace<Cost>(ent);
            cost.data = j["Cost"];
        }

        if(j.contains("CostPerDay")) {
            auto &cost = registry.get_or_emplace<Cost>(ent);
            cost.data = j["CostPerDay"];
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
        db = std::make_unique<SQLite::Database>(config::dbName, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        SQLite::Transaction trans(*db);
        for(auto &s : schema) {
            db->exec(s);
        }

        trans.commit();
    }

    void loadObjects() {
        // Step 1: get max id of objects table.
        SQLite::Statement q(*db, "SELECT MAX(id) FROM objects;");
        q.executeStep();
        auto count = q.getColumn(0).getInt64();

        // Reserve RAM for a single easy allocation to make this simple...
        objects.resize(count + 50, {0, entt::null});

        std::size_t counter = 0;
        broadcast("Preparing objects for loading...");
        SQLite::Statement q1(*db, "SELECT id, generation FROM objects;");
        while(q1.executeStep()) {
            auto id = q1.getColumn(0).getInt64();
            auto gen = q1.getColumn(1).getInt64();
            auto ent = registry.create();
            registry.emplace<ObjectId>(ent, id, gen);
            objects[id] = {gen, ent};
            counter++;
        }
        broadcast(fmt::format("Prepared {} objects.", counter));

        std::size_t hydrated = 0;
        broadcast("Hydrating objects...");
        // Now we're gonna need to select id and data from objects to deserialize.
        SQLite::Statement q2(*db, "SELECT id, data FROM objects;");
        while(q2.executeStep()) {
            auto id = q2.getColumn(0).getInt64();
            auto data = q2.getColumn(1).getText();
            auto ent = objects[id].second;
            deserializeEntity(ent, nlohmann::json::parse(data));
            hydrated++;
            if(hydrated % (count / 4) == 0) {
                broadcast(fmt::format("Hydrated {}/{} objects.", hydrated, counter));
            }
        }
        broadcast(fmt::format("Hydrated {}/{} objects.", hydrated, counter));

        broadcast(fmt::format("Finishing touches on objects..."));
        loadRelations();

    }

    void loadRelations() {
        auto view = registry.view<Location>();

        for(auto e : view) {
            auto &l = view.get<Location>(e);
            if(l.locationType == LocationType::Inventory) {
                auto &inv = registry.get_or_emplace<Inventory>(l.data);
                inv.data.push_back(e);
            } else if(l.locationType == LocationType::Equipment) {
                auto &eq = registry.get_or_emplace<Equipment>(l.data);
                eq.data[l.x] = e;
            } else if(l.locationType == LocationType::Area) {
                auto &ar = registry.get_or_emplace<Area>(l.data);
                auto &room = ar.data[l.x];
                auto &rc = registry.get_or_emplace<RoomContents>(room);
                rc.data.push_back(e);
            } else if(l.locationType == LocationType::Expanse || l.locationType == LocationType::Map) {
                auto &gcon = registry.get_or_emplace<GridContents>(l.data);
                GridPoint gp(l.x, l.y, l.z);
                gcon.data[gp].push_back(e);
            } else if(l.locationType == LocationType::Space) {
                auto &scon = registry.get_or_emplace<SectorContents>(l.data);
                SectorPoint sp(l.x, l.y, l.z);
                scon.data[sp].push_back(e);
            }
        }

        auto v2 = registry.view<ObjectId, ObjectFlags, Area>();
        for(auto e : v2) {
            auto &of = v2.get<ObjectFlags>(e);
            auto &o = v2.get<ObjectId>(e);
            auto &a = v2.get<Area>(e);

            if(of.data.test(oflags::GLOBALROOMS)) {
                for(auto &[id, en] : a.data) {
                    Location dest;
                    dest.locationType = LocationType::Area;
                    dest.x = id;
                    dest.data = e;
                    legacyRooms[id] = dest;
                }
            }
        }

    }

    void loadDatabase() {
        broadcast("Loading game database... please wait warmly...");
        loadZones();
        loadPrototypes();
        loadScripts();
        loadObjects();
        broadcast("Loaded Objects.");
        loadLegacySpace();
    }

    void savePrototype(const std::string& name, const nlohmann::json& j) {
        SQLite::Statement q(*db, "INSERT OR REPLACE INTO prototypes (name, data) VALUES (?, ?)");
        q.bind(1, name);
        q.bind(2, j.dump(4, ' ', false, nlohmann::json::error_handler_t::ignore));
        q.exec();
        q.reset();
    }

    void loadPrototypes() {
        SQLite::Statement q(*db, "SELECT name, data FROM prototypes;");
        while(q.executeStep()) {
            auto proto = std::make_shared<PrototypeData>();
            proto->name = q.getColumn(0).getText();
            auto data = q.getColumn(1).getText();
            proto->data = nlohmann::json::parse(data);
            prototypes[proto->name] = proto;
        }
    }

    std::optional<nlohmann::json> getPrototype(const std::string& name) {
        SQLite::Statement q(*db, "SELECT data FROM prototypes WHERE name = ?;");
        q.bind(1, name);
        if(q.executeStep()) {
            auto data = q.getColumn(0).getText();
            auto j = nlohmann::json::parse(data);
            j["Prototype"] = name;
            return j;
        }
        return std::nullopt;
    }

    void loadLegacySpace() {
        broadcast("Loading legacy space...  amoebas and all.");
        SQLite::Statement q(*db, "SELECT * from legacySpaceRooms;");
        auto obj = getObject(config::legacySpaceId);
        while(q.executeStep()) {
            Location dest;
            dest.data = obj;
            dest.locationType = LocationType::Expanse;
            auto id = q.getColumn("id").getInt64();
            dest.x = q.getColumn("x").getInt64();
            dest.y = q.getColumn("y").getInt64();
            dest.z = q.getColumn("z").getInt64();
            legacyRooms[id] = dest;
        }
    }

    void saveLegacySpace() {
        SQLite::Statement q(*db, "INSERT OR REPLACE INTO legacySpaceRooms (id, x, y, z) VALUES (?, ?, ?, ?);");
        for(auto &[id, d] : legacyRooms) {
            if(d.locationType != LocationType::Expanse) {
                continue;
            }
            q.bind(1, static_cast<int64_t>(id));
            q.bind(2, d.x);
            q.bind(3, d.y);
            q.bind(4, d.z);
            q.exec();
            q.reset();
        }
    }

    void loadZones() {
        // let's retrieve the count of zones.
        SQLite::Statement q1(*db, "SELECT COUNT(*) FROM zones;");
        q1.executeStep();
        auto count = q1.getColumn(0).getInt64();

        broadcast(fmt::format("Loading {} Legacy Zones...", count));

        SQLite::Statement q(*db, "SELECT id, data from zones;");
        while(q.executeStep()) {
            auto id = q.getColumn(0).getInt64();
            auto data = q.getColumn(1).getText();
            auto ent = registry.create();
            zones.emplace(id, nlohmann::json::parse(data));
        }

        int counter = 0;
        for(auto &[id, z] : zones) {
            z.id = id;
            // Stagger zone resets so they don't all happen entirely at once....
            z.age = (randomNumber(10, 300) * 1.0) + 0.3 * ++counter;
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