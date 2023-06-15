#include "dbatk/base.h"
#include "dbatk/room/room.h"

namespace dbat {

    bool gameIsLoading{true};

    void setDirty(const std::shared_ptr<Object>& obj, bool override) {
        setDirty(obj->getID(), override);
    }
    void setDirty(const ObjectID& id, bool override) {
        if(!gameIsLoading || override) {
            dirty.insert(id);
        }
    }

    std::string ObjectID::toString() const {
        return fmt::format("#{}:{}", index, generation);
    }

    std::shared_ptr<Object> getObject(std::size_t index, int64_t generation) {
        if (index >= objects.size()) {
            return nullptr;
        }
        auto& obj = objects[index];
        if (obj.first != generation) {
            return nullptr;
        }
        return obj.second;
    }

    std::shared_ptr<Object> getObject(std::size_t index) {
        if (index >= objects.size()) {
            return nullptr;
        }
        auto& obj = objects[index];
        if (!obj.second) {
            return nullptr;
        }
        return obj.second;
    }

    std::shared_ptr<Object> ObjectID::getObject() const {
        return dbat::getObject(index, generation);
    }

    std::vector<std::pair<int64_t, std::shared_ptr<Object>>> objects;

    std::size_t getFreeObjectID() {
        std::size_t i = 0;
        for (; i < objects.size(); i++) {
            if (!objects[i].second) {
                return i;
            }
        }
        // We couldn't find any, so let's resize the vector to hold some more...
        objects.resize(i + 40);
        return i;
    }


    int64_t getUnixTimestamp() {
        return std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }

    ObjectID createObjectID() {
        return {getFreeObjectID(), getUnixTimestamp()};
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

    std::unordered_map<RoomID, Room*> legacyRooms;

}