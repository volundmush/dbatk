#pragma once
#include "core/base.h"
#include "fmt/format.h"
#include <boost/regex.h>
#include <random>
#include <unordered_set>
#include <bitset>
#include <boost/algorithm/string.hpp>

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
    // Class for all Areas. An Area is basically just a collection of Rooms, with the Area itself
    // having no physical location. Areas are just containers for Rooms, which might or might not have another
    // object as their parent for a map hierarchy.
    class Area;
    // Class for all Items. Items are objects that can be picked up and moved around. They might also be furniture.
    class Item;
    class CelestialBody;
    class Vehicle;
    class Dimension;
    class Sector;
    class Grid;

    struct Room;
    struct Exit;

    struct ObjectID {
        ObjectID(std::size_t index, int64_t generation) : index(index), generation(generation) {};
        explicit ObjectID(const nlohmann::json& json) : index(json[0]), generation(json[1]) {};
        std::size_t index;
        int64_t generation;
        [[nodiscard]] std::string toString() const;
        [[nodiscard]] std::shared_ptr<Object> getObject() const;
    };

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

    extern std::unordered_set<std::string> stringPool;

    std::string_view intern(const std::string& str);
    std::string_view intern(std::string_view str);

    extern std::random_device randomDevice;
    extern std::default_random_engine randomEngine;

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