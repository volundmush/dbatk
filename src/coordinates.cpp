#include "dbatk/components.h"

namespace dbat {
    Location::Location(const nlohmann::json& j) {
        if(j.contains("data")) {
            ObjectId obj(j["data"]);
            data = obj.getObject();
        }
        if(j.contains("locationType")) {
            locationType = j["locationType"].get<LocationType>();
        }
        if(j.contains("x")) x = j["x"];
        if(j.contains("y")) y = j["y"];
        if(j.contains("z")) z = j["z"];
    }

    Location::Location(entt::entity ent) {
        if(auto r = registry.try_get<Room>(ent); r) {
            data = r->obj.getObject();
            locationType = LocationType::Area;
            x = r->id;
        } else {
            locationType = LocationType::Absolute;
            data = ent;
        }
    }

    nlohmann::json Location::serialize() {
        nlohmann::json j;
        if(data != entt::null) j["data"] = registry.get<ObjectId>(data);
        j["locationType"] = locationType;
        if(x != 0.0) j["x"] = x;
        if(y != 0.0) j["y"] = y;
        if(z != 0.0) j["z"] = z;

        return j;
    }

    std::string Location::roomString() {
        if(locationType == LocationType::Area) {
            return fmt::format("{}/{:.0f}", registry.get<ObjectId>(data).toString(), x);
        }
        if(locationType == LocationType::Expanse || locationType == LocationType::Map) {
            return fmt::format("{}/{:.0f}:{:.0f}:{:.0f}", registry.get<ObjectId>(data).toString(), x, y, z);
        }
        if(locationType == LocationType::Space) {
            return fmt::format("{}/{}:{}:{}", registry.get<ObjectId>(data).toString(), x, y, z);
        }

        return "";
    }

    entt::entity Location::findRoom(RoomId id) {
        if(locationType == LocationType::Area) {
            auto &area = registry.get<Area>(data);
            auto find = area.data.find(id);
            if(find != area.data.end()) return find->second;
        }
        return entt::null;
    }

    entt::entity Location::getRoom() {
        if(locationType == LocationType::Area) {
            return findRoom(x);
        }
        return entt::null;
    }

    GridPoint::GridPoint(const nlohmann::json& j) {
        x = j[0];
        y = j[1];
        z = j[2];
    }

    nlohmann::json GridPoint::serialize() const {
        nlohmann::json j;
        j[0] = x;
        j[1] = y;
        j[2] = z;
        return j;
    }

    SectorPoint::SectorPoint(const nlohmann::json& j) {
        x = j[0];
        y = j[1];
        z = j[2];
    }

    nlohmann::json SectorPoint::serialize() const {
        nlohmann::json j;
        j[0] = x;
        j[1] = y;
        j[2] = z;
        return j;
    }
}

namespace nlohmann {
    void to_json(json& j, const dbat::ObjectId& o) {
        j = json{o.index, o.generation};
    }

    void from_json(const json& j, dbat::ObjectId& o) {
        j.at(0).get_to(o.index);
        j.at(1).get_to(o.generation);
    }
}