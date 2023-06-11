#pragma once
#include "dbatk/flags.h"

namespace dbat::rflags {

    enum RFlag : std::size_t {
        DARK = 0,          // Dark
        DEATH = 1,         // Death trap
        NOMOB = 2,         // MOBs not allowed
        INDOORS = 3,       // Indoors
        PEACEFUL = 4,      // Violence not allowed
        SOUNDPROOF = 5,    // Shouts, gossip blocked
        NOTRACK = 6,       // Track won't go through
        NOINSTANT = 7,     // IT not allowed
        TUNNEL = 8,        // room for only 1 pers
        PRIVATE = 9,       // Can't teleport in
        GODROOM = 10,      // LVL_GOD+ only allowed
        HOUSE = 11,        // (R) Room is a house
        HOUSE_CRASH = 12,  // (R) House needs saving
        ATRIUM = 13,       // (R) The door to a house
        OLC = 14,          // (R) Modifyable/!compress
        BFS_MARK = 15,     // (R) breath-first srch mrk
        VEHICLE = 16,      // Requires a vehicle to pass
        UNDERGROUND = 17,  // Room is below ground
        CURRENT = 18,      // Room move with random currents
        TIMED_DT = 19,     // Room has a timed death trap
        EARTH = 20,        // Room is on Earth
        VEGETA = 21,       // Room is on Vegeta
        FRIGID = 22,       // Room is on Frigid
        KONACK = 23,       // Room is on Konack
        NAMEK = 24,        // Room is on Namek
        NEO = 25,          // Room is on Neo
        AL = 26,           // Room is on AL
        SPACE = 27,        // Room is on Space
        HELL = 28,         // Room is Punishment Hell
        REGEN = 29,        // Better regen
        RHELL = 30,        // Room is HELLLLLLL
        GRAVITYX10 = 31,   // For rooms that have 10x grav
        AETHER = 32,       // Room is on Aether
        HBTC = 33,         // Room is extra special training area
        PAST = 34,         // Inside the pendulum room
        CBANK = 35,        // This room is a clan bank
        SHIP = 36,         // This room is a private ship room
        YARDRAT = 37,      // This room is on planet Yardrat
        KANASSA = 38,      // This room is on planet Kanassa
        ARLIA = 39,        // This room is on planet Arlia
        AURA = 40,         // This room has an aura around it
        EORBIT = 41,       // Earth Orbit
        FORBIT = 42,       // Frigid Orbit
        KORBIT = 43,       // Konack Orbit
        NORBIT = 44,       // Namek  Orbit
        VORBIT = 45,       // Vegeta Orbit
        AORBIT = 46,       // Aether Orbit
        YORBIT = 47,       // Yardrat Orbit
        KANORB = 48,       // Kanassa Orbit
        ARLORB = 49,       // Arlia Orbit
        NEBULA = 50,       // Nebulae
        ASTERO = 51,       // Asteroid
        WORMHO = 52,       // Wormhole
        STATION = 53,      // Space Station
        STAR = 54,         // Is a star
        CERRIA = 55,       // This room is on planet Cerria
        CORBIT = 56,       // This room is in Cerria's Orbit
        BEDROOM = 57,      // +25% regen
        WORKOUT = 58,      // Workout Room
        GARDEN1 = 59,      // 8 plant garden
        GARDEN2 = 60,      // 20 plant garden
        FERTILE1 = 61,
        FERTILE2 = 62,
        FISHING = 63,
        FISHFRESH = 64,
        CANREMODEL = 65
    };

    constexpr std::size_t NUM_ROOM_FLAGS = 66;


    class RoomFlag : public BaseFlag {
    public:
        virtual void onLoad(Room* room) {};
        virtual void onSet(Room* room) {};
        virtual void onClear(Room* room) {};
    };

    class SimpleRoomFlag : public RoomFlag {
    public:
        SimpleRoomFlag(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getID() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    extern const std::vector<std::shared_ptr<RoomFlag>> roomFlags;

}