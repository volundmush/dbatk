#pragma once

// sane C++ standard imports.
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <utility>
#include <random>
#include <unordered_set>
#include <limits>

// Our own libraries...
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <boost/algorithm/string.hpp>
#include "entt/entt.hpp"

namespace dbat {

    class Connection;
    class Link;
    class LinkManager;
    class Session;
    struct ObjectId;

    extern std::shared_ptr<spdlog::logger> logger;

    template<typename T = bool>
    using OpResult = std::pair<T, std::optional<std::string>>;

    class BaseFlag {
    public:
        [[nodiscard]] virtual std::size_t getId() const = 0;
        [[nodiscard]] virtual std::string getName() const = 0;
    };

    extern entt::registry registry;

    bool isObjRef(const std::string& str);
    bool isObjId(const std::string& str);
    bool isDgRef(const std::string& str);
    entt::entity parseObjectId(const std::string& str);

    // If this is set, many operations which would set the dirty flag will not.
    // Other safeguards of the Object API may also be released. Do remember to
    // set it to false after the game is done loading.
    extern bool gameIsLoading;

    void setDirty(entt::entity, bool override = false);
    void setDirty(const ObjectId& id, bool override = false);

    entt::entity getObject(std::size_t index, int64_t generation);
    entt::entity getObject(std::size_t index);

    using RoomId = std::size_t;

    // The objects vector serves as a generational arena for all objects in the game.
    // The first element of the pair is the generation (as a unix timestamp in seconds),
    // the second is the object itself.
    // A grave is an object with a generation of 0 and an empty pointer.
    // We COULD get away with just using a vector of shared_ptrs, but this way we can
    // use the ObjectId for serialization and not have to worry about the object being
    // deleted and the ID being reused.
    extern std::vector<std::pair<int64_t, entt::entity>> objects;

    std::size_t getFreeObjectId();

    int64_t getUnixTimestamp();

    ObjectId createObjectId();

    entt::entity createObject();

    extern std::unordered_set<std::string> stringPool;

    std::string_view intern(const std::string& str);
    std::string_view intern(std::string_view str);

    extern std::random_device randomDevice;
    extern std::default_random_engine randomEngine;

    template<typename T>
    T randomNumber(T min, T max) {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(randomEngine);
    }

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

    OpResult<> hashPassword(std::string_view password);

    OpResult<> checkPassword(std::string_view hash, std::string_view check);


}