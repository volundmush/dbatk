#include "dbatk/base.h"
#include "sodium.h"

namespace dbat {
    std::unique_ptr<boost::asio::io_context> executor;
    std::shared_ptr<spdlog::logger> logger;

    entt::registry registry;

    bool gameIsLoading{true};

    entt::entity createObject() {
        auto id = createObjectId();
        auto obj = registry.create();
        if(id.index >= objects.size())
            objects.resize(id.index + 40);
        objects[id.index] = std::make_pair(id.generation, obj);
        registry.emplace<ObjectId>(obj, id);
        return obj;
    }

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

    std::string ObjectId::toString() const {
        return fmt::format("#{}:{}", index, generation);
    }

    entt::entity getObject(std::size_t index, int64_t generation) {
        if (index >= objects.size()) {
            return entt::null;
        }
        auto& obj = objects[index];
        if (obj.first != generation) {
            return entt::null;
        }
        return obj.second;
    }

    entt::entity getObject(std::size_t index) {
        if (index >= objects.size()) {
            return entt::null;
        }
        auto& obj = objects[index];
        if (!registry.valid(obj.second)) {
            return entt::null;
        }
        return obj.second;
    }

    entt::entity ObjectId::getObject() const {
        return dbat::getObject(index, generation);
    }

    std::vector<std::pair<int64_t, entt::entity>> objects;

    std::unordered_map<std::string, std::set<entt::entity>> protoTracker;

    int64_t getProtoCount(const std::string& protoName) {
        auto it = protoTracker.find(protoName);
        if(it == protoTracker.end()) return 0;
        return it->second.size();
    }

    std::size_t getFreeObjectId() {
        std::size_t i = 0;
        for (; i < objects.size(); i++) {
            if (!registry.valid(objects[i].second)) {
                return i;
            }
        }
        // We couldn't find any, so let's resize the vector to hold some more...
        objects.resize(i + 40, {0, entt::null});
        return i;
    }


    int64_t getUnixTimestamp() {
        return std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }

    ObjectId createObjectId() {
        return {getFreeObjectId(), getUnixTimestamp()};
    }

    // the obj_regex is supposed to watch for patterns like #5 or #8721:1680642313 and capture the numbers.
    boost::regex obj_regex(R"(^#(?<id>\d+)(:(?<gen>\d+)?)?)");
    boost::regex dg_regex(R"(^#(?<id>\d+)(:(?<gen>\d+))(?<slash>\/(?<room>\d+)?)?)");

    // This is true if the string matches obj_regex but has no gen.
    bool isObjRef(const std::string& str) {
        boost::smatch match;
        return boost::regex_search(str, match, obj_regex) && !match["gen"].matched;
    }

    // This is true if the string matches obj_regex and has a gen.
    bool isObjId(const std::string& str) {
        boost::smatch match;
        return boost::regex_search(str, match, obj_regex) && match["gen"].matched;
    }

    bool isDgRef(const std::string& str) {
        boost::smatch match;
        return boost::regex_search(str, match, dg_regex);
    }

    entt::entity parseObjectId(const std::string& str) {
        // if the regex doesn't match, we'll return an error.
        // if it does match, we're going to use obj_regex and convert the id and optional gen
        // fields to numbers. Then we'll attempt to locate the object slot in objects...
        boost::smatch match;
        if (!boost::regex_search(str, match, obj_regex)) return entt::null;
        auto id = std::stoull(match["id"]);
        if(match["gen"].matched) {
            auto gen = std::stoull(match["gen"]);
            return getObject(id, gen);
        } else {
            return getObject(id);
        }
    }



    std::unordered_set<std::string> stringPool;

    std::string_view intern(const std::string& str) {
        auto pair = stringPool.insert(str);
        return std::string_view(*pair.first);
    }

    std::string_view intern(std::string_view str) {
        return intern(std::string(str));
    }

    std::random_device randomDevice;
    std::default_random_engine randomEngine(randomDevice());

    std::unordered_set<ObjectId> dirty;
    std::unordered_map<RoomId, Destination> legacyRooms;

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

    OpResult<> hashPassword(std::string_view password) {
        char hashed_password[crypto_pwhash_STRBYTES];

        if(password.empty()) {
            return {false, "Password cannot be empty"};
        }

        if(crypto_pwhash_str(hashed_password, password.data(), password.size(),
                             crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
            return {false, "Failed to hash password"};
        }

        return {true, std::string(hashed_password)};

    }

    OpResult<> checkPassword(std::string_view hash, std::string_view check) {
        if(crypto_pwhash_str_verify(hash.data(), check.data(), check.size()) != 0) {
            return {false, "Passwords do not match"};
        }
        return {true, std::nullopt};
    }

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