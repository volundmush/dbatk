#pragma once
#include "core/base.h"
#include "fmt/format.h"
#include <boost/regex.h>
#include <random>
#include <unordered_set>
#include <bitset>
#include <boost/algorithm/string.hpp>
#include "SQLiteCpp/SQLiteCpp.h"
#include "entt/entt.hpp"

namespace dbat {
    using namespace core;

    class DBATConnection;

    class Session;

    // The Object class is Abstract.... not meant to be used directly.
    class Object;

    // The Character class is Abstract, not meant to be used directly.
    class Character;

    // Class for all Player Characters.
    class Player;

    // Class for all Non-Player Characters.
    class NPC;

    // Class for all Areas. An Area is basically just a collection of Rooms, with the Area itself possibly having no
    // having no physical location. Areas are just containers for Rooms, which might or might not have another
    // object as their parent for a map hierarchy.
    class Area;

    // Class for all Items. Items are objects that can be picked up and moved around and sometimes equipped.
    // They might also be furniture, trees, or other fixtures in a room..
    class Item;

    // A Celestial Body is something like a planet, asteroid, or star - some kind of (probably?) natural physical object
    // likely to be found in a space sector. A common design pattern for creating worlds would be to have
    // numerous Areas within a CelestialBody - a "planet which contains different regions".
    class CelestialBody;

    // A car, hovercycle, spaceship, maybe some kind of boat of a warmech? Is it a Gundam? it's a Gundam, isn't it?
    class Vehicle;

    // A Dimension is a logical container for other objects which serves as a kind of boundary for certain powers, abilities,
    // or possibly transit. A Dimension might be a pocket dimension, a plane of existence, or a universe.
    class Dimension;

    // A sector is a 3D space which contains celestial bodies and other objects. It's meant to make use of a 3D grid
    // A similar concept is found in games like the X series, or EVE Online - sectors linked by jumpgates.
    class Sector;

    // A Grid is the integer-based equivalent of a sector, where it presents a map of tiles characters can move across.
    // This could be useful for overworld maps and the like.
    class Grid;

    struct Room;
    struct Exit;

    using GridLength = int64_t;

    struct GridPoint {
        GridLength x, y, z;
        bool operator==(const GridPoint& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
    };

    using SectorLength = double;
    struct SectorPoint {
        SectorLength x, y, z;
        bool operator==(const SectorPoint& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
    };

    struct ObjectID {
        ObjectID(std::size_t index, int64_t generation) : index(index), generation(generation) {};

        explicit ObjectID(const nlohmann::json &json) : index(json[0]), generation(json[1]) {};
        std::size_t index;
        int64_t generation;

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::shared_ptr<Object> getObject() const;

        bool operator==(const ObjectID &other) const {
            return index == other.index && generation == other.generation;
        }
    };

}

namespace std {
    template <>
    struct hash<dbat::ObjectID> {
        std::size_t operator()(const dbat::ObjectID& id) const {
            return std::hash<std::size_t>()(id.index) ^ std::hash<int64_t>()(id.generation);
        }
    };

    template<>
    struct hash<dbat::GridPoint> {
        size_t operator()(const dbat::GridPoint& pt) const {
            return std::hash<dbat::GridLength>()(pt.x) ^ std::hash<dbat::GridLength>()(pt.y) ^ std::hash<dbat::GridLength>()(pt.z);
        }
    };

    template<>
    struct hash<dbat::SectorPoint> {
        size_t operator()(const dbat::SectorPoint& pt) const {
            return std::hash<dbat::SectorLength>()(pt.x) ^ std::hash<dbat::SectorLength>()(pt.y) ^ std::hash<dbat::SectorLength>()(pt.z);
        }
    };

}

namespace dbat {

    // Any object which needs to be updated in the database - it was created, modified, or deleted,
    // its ObjectID must be in the dirty set by the time the syncer runs.
    std::unordered_set<ObjectID> dirty;

    // If this is set, many operations which would set the dirty flag will not.
    // Other safeguards of the Object API may also be released. Do remember to
    // set it to false after the game is done loading.
    extern bool gameIsLoading;

    void setDirty(const std::shared_ptr<Object>& obj, bool override = false);
    void setDirty(const ObjectID& id, bool override = false);

    std::shared_ptr<Object> getObject(std::size_t index, int64_t generation);
    std::shared_ptr<Object> getObject(std::size_t index);

    using RoomID = std::size_t;

    // The objects vector serves as a generational arena for all objects in the game.
    // The first element of the pair is the generation (as a unix timestamp in seconds),
    // the second is the object itself.
    // A grave is an object with a generation of 0 and an empty pointer.
    // We COULD get away with just using a vector of shared_ptrs, but this way we can
    // use the ObjectID for serialization and not have to worry about the object being
    // deleted and the ID being reused.
    extern std::vector<std::pair<int64_t, std::shared_ptr<Object>>> objects;

    std::size_t getFreeObjectID();

    int64_t getUnixTimestamp();

    ObjectID createObjectID();

    template<typename T>
    std::shared_ptr<T> createObject() {
        auto id = createObjectID();
        auto obj = std::make_shared<T>(id);
        if(id.index >= objects.size())
            objects.resize(id.index + 40);
        objects[id.index] = std::make_pair(id.generation, obj);
        return obj;
    }

    extern std::unordered_set<std::string> stringPool;

    std::string_view intern(const std::string& str);
    std::string_view intern(std::string_view str);

    extern std::random_device randomDevice;
    extern std::default_random_engine randomEngine;

    // For backwards compatability with the old DBAT code, we have this map which effectively replicates
    // the old 'world' variable. It is filled with the rooms from Objects that are marked GLOBALROOM.
    // Beware of ID collisions when setting objects GLOBALROOM.
    extern std::unordered_map<RoomID, Room*> legacyRooms;

    template <typename Iterator, typename Key = std::function<std::string(typename std::iterator_traits<Iterator>::value_type)>>
    Iterator partialMatch(
            const std::string& match_text,
            Iterator begin, Iterator end,
            bool exact = false,
            Key key = [](const auto& val){ return std::to_string(val); }
    )
    {
        // Use a multimap to automatically sort by the transformed key.
        using ValueType = typename std::iterator_traits<Iterator>::value_type;
        std::multimap<std::string, ValueType> sorted_map;
        std::for_each(begin, end, [&](const auto& val) {
            sorted_map.insert({key(val), val});
        });

        for (const auto& pair : sorted_map)
        {
            if (boost::iequals(pair.first, match_text))
            {
                return std::find(begin, end, pair.second);
            }
            else if (!exact && boost::istarts_with(pair.first, match_text))
            {
                return std::find(begin, end, pair.second);
            }
        }
        return end;
    }


}

namespace nlohmann {
    void to_json(json& j, const dbat::ObjectID& o) {
        j = json{o.index, o.generation};
    }

    void from_json(const json& j, dbat::ObjectID& o) {
        j.at(0).get_to(o.index);
        j.at(1).get_to(o.generation);
    }
}