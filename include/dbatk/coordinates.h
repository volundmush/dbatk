#pragma once
#include "dbatk/base.h"

namespace dbat {

    struct ObjectId {
        ObjectId() = default;
        ObjectId(std::size_t index, int64_t generation) : index(index), generation(generation) {};

        explicit ObjectId(const nlohmann::json &json) : index(json[0]), generation(json[1]) {};
        std::size_t index;
        int64_t generation;

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] entt::entity getObject() const;

        bool operator==(const ObjectId &other) const {
            return index == other.index && generation == other.generation;
        }
    };

    enum class LocationType : int8_t {
        Absolute = 0,
        Inventory = 1,
        Equipment = 2,
        Area = 3,
        Expanse = 4,
        Map = 5,
        Space = 6
    };

    /*
     * Since we have a lot of different types of locations, we need a way to represent them all.
     * As locations are also used for destinations and targeting of commands, render requests, or
     * the like, this struct can also be thought of as a 'Target' specifier.
     * */
    struct Location {
        Location() = default;

        explicit Location(entt::entity ent);

        explicit Location(const nlohmann::json &j);

        entt::entity data{entt::null};
        LocationType locationType{LocationType::Absolute};
        // These coordinates serve multiple purposes. For an Area/Expanse/Map/Space, they are
        // the coordinates of the object in that plane.
        // For Inventory and Absolute, they are unused (but I can see a case of using them for sorting or organization,
        // or maybe separate inventory pockets.) For equipment, x is used for the wear location.
        double x{0.0}, y{0.0}, z{0.0};

        nlohmann::json serialize();

        std::string roomString();

        entt::entity getRoom();

        entt::entity findRoom(RoomId id);
    };

    using GridLength = int64_t;

    struct GridPoint {
        GridPoint() = default;

        GridPoint(GridLength x, GridLength y, GridLength z) : x(x), y(y), z(z) {};

        explicit GridPoint(const nlohmann::json &j);

        GridLength x, y, z;

        bool operator==(const GridPoint &other) const {
            return x == other.x && y == other.y && z == other.z;
        }

        nlohmann::json serialize() const;
    };

    using SectorLength = double;

    struct SectorPoint {
        SectorPoint() = default;

        SectorPoint(SectorLength x, SectorLength y, SectorLength z) : x(x), y(y), z(z) {};

        explicit SectorPoint(const nlohmann::json &j);

        SectorLength x, y, z;

        bool operator==(const SectorPoint &other) const {
            return x == other.x && y == other.y && z == other.z;
        }

        nlohmann::json serialize() const;
    };
}

namespace std {
    template <>
    struct hash<dbat::ObjectId> {
        std::size_t operator()(const dbat::ObjectId& id) const {
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

namespace nlohmann {
    void to_json(json& j, const dbat::ObjectId& o);

    void from_json(const json& j, dbat::ObjectId& o);
}