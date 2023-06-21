#pragma once

#include "dbatk/base.h"
#include "dbatk/aspects/admFlags.h"
#include "dbatk/aspects/affect.h"
#include "dbatk/aspects/celFlags.h"
#include "dbatk/aspects/celType.h"
#include "dbatk/aspects/charStats.h"
#include "dbatk/aspects/direction.h"
#include "dbatk/aspects/effect.h"
#include "dbatk/aspects/exitFlags.h"
#include "dbatk/aspects/gridTile.h"
#include "dbatk/aspects/itemFlags.h"
#include "dbatk/aspects/liquid.h"
#include "dbatk/aspects/material.h"
#include "dbatk/aspects/npcFlags.h"
#include "dbatk/aspects/objFlags.h"
#include "dbatk/aspects/position.h"
#include "dbatk/aspects/pref.h"
#include "dbatk/aspects/race.h"
#include "dbatk/aspects/roomFlags.h"
#include "dbatk/aspects/sensei.h"
#include "dbatk/aspects/sex.h"
#include "dbatk/aspects/skill.h"
#include "dbatk/aspects/size.h"
#include "dbatk/aspects/terrain.h"
#include "dbatk/aspects/wear.h"


namespace dbat {

    struct StringView {
        StringView() = default;
        explicit StringView(const std::string& txt) { setData(txt); };
        std::string_view data;
        std::string_view clean;
        void setData(const std::string& txt);
    };

    struct Name : StringView {
        using StringView::StringView;
    };
    struct ShortDescription : StringView {
        using StringView::StringView;
    };
    struct RoomDescription : StringView {
        using StringView::StringView;
    };
    struct LookDescription : StringView {
        using StringView::StringView;
    };

    struct Entity {
        entt::entity data;
    };

    struct ReverseEntity {
        std::vector<entt::entity> data{};
    };

    struct Location : Entity {};
    struct Contents : ReverseEntity {};
    struct Parent : Entity {};
    struct Children : ReverseEntity {};
    struct Owner : Entity {};
    struct Assets : ReverseEntity {};

    struct Race {
        race::RaceId data{race::FORMLESS};
    };

    struct Sensei {
        sensei::SenseiId data{sensei::UNTRAINED};
    };

    struct Position {
        position::PositionId data{position::STANDING};
    };

    struct Sex {
        sex::SexId data{sex::NEUTER};
    };

    struct Terrain {
        terrain::TerrainId data{terrain::INSIDE};
    };

    struct AdminFlags {
        std::bitset<aflags::countAdminFlags> data;
    };

    struct ObjectFlags {
        std::bitset<oflags::numObjFlags> data;
    };

    struct SessionHolder {
        entt::entity session;
        uint8_t sessionMode{0};
    };

    using DestinationType = std::variant<RoomId, GridPoint, SectorPoint>;
    struct Destination {
        Destination() = default;
        explicit Destination(const nlohmann::json& j);
        // if ent is entt::null, then it's assumed to be a destination on the same Object.
        entt::entity ent{entt::null};
        DestinationType destination;
        nlohmann::json serialize();
    };

    struct Exits {
        std::unordered_map<dir::DirectionId, Destination> data{};
    };

    struct Door {
        Door() = default;
        explicit Door(const nlohmann::json& j);
        std::string_view keyword, description;
        std::bitset<eflags::numExitFlags> flags;
        std::optional<std::size_t> legacyKey;
        int dclock{0};
        int dchide{0};
        nlohmann::json serialize();
    };

    struct Doors {
        std::unordered_map<dir::DirectionId, Door> data{};
    };

    struct Rooms {
        std::unordered_map<RoomId, entt::entity> data{};
    };

    struct RoomFlags {
        std::bitset<rflags::numRoomFlags> data;
    };

    struct GridBounds {
        std::optional<GridLength> minX, maxX, minY, maxY, minZ, maxZ;
    };

    struct GridContents {
        std::unordered_map<GridPoint, std::vector<entt::entity>> data{};
    };

    struct GridPointsOfInterest {
        std::unordered_map<GridPoint, entt::entity> data{};
    };

    struct GridLocation {
        GridLocation() = default;
        explicit GridLocation(const GridPoint& d) : data(d) {};
        explicit GridLocation(const nlohmann::json& j) : data(j) {};
        GridPoint data;
    };

    struct SectorLocation {
        SectorLocation() = default;
        explicit SectorLocation(const SectorPoint& d) : data(d) {};
        explicit SectorLocation(const nlohmann::json& j) : data(j) {};
        SectorPoint data;
    };

    struct RoomLocation {
        RoomId id;
    };

    struct Size {
        size::SizeId data{size::MEDIUM};
    };

    struct Cost {
        int64_t data{0};
    };

    struct Money {
        int64_t data{0};
    };

    struct LevelRequirement {
        int64_t data{0};
    };

    struct Weight {
        double data{0.0};
    };

    struct LightData {
        double timeLeft{0.0};
        double hoursLeft{0.0};
    };

    struct WeaponData {
        int skill;    // e.g., VAL_WEAPON_SKILL
        int damDice;  // e.g., VAL_WEAPON_DAMDICE
        int damSize;  // e.g., VAL_WEAPON_DAMSIZE
        int damType;  // e.g., VAL_WEAPON_DAMTYPE
    };

    struct WornData {
        // The positions that the item can be worn on.
        std::bitset<wear::numWearFlags> wearFlags;
        // Where the item is currently worn, if anywhere.
        std::optional<int> wornOn;

    };

    struct FoodData {
        double nutrition{0.0}, maxNutrition{0.0};
        bool poisoned{false};
    };

    struct ContainerData {
        uint16_t capacity{0};
        std::bitset<4> containerFlags;
        std::optional<std::size_t> key;
    };

    struct DrinkData {
        int16_t capacity{0};
        int16_t current{0};
        bool poisoned{false};
        liq::LiquidId liquid{liq::WATER};
    };

    struct Durability {
        double durability{0.0}, maxDurability{0.0};
    };

    struct ItemFlags {
        std::bitset<iflags::numItemFlags> data;
    };

    struct ItemAffects {
        std::bitset<affect::countAffectFlags> data;
    };

    struct CharacterAffects {
        std::bitset<affect::countAffectFlags> data;
    };

    struct NPCFlags {
        std::bitset<nflags::numNpcFlags> data;
    };

    struct PlayerFlags {
        //std::bitset<pflags::NUM_PLAYER_FLAGS> data;
    };

    struct Item {
        std::optional<std::size_t> vnum;
    };
    struct Character {
        std::array<double, cstat::numCharStats> stats{};
    };
    struct NPC {
        std::optional<std::size_t> vnum;
    };

    struct Player {
        int64_t accountId{-1};
    };

    struct Room {
        ObjectId obj;
        RoomId id;
    };

    struct Dimension {};
    struct Expanse {};
    struct Area {};
    struct Vehicle {};

    struct CelestialBody {
        celtype::CelestialTypeId type{celtype::PLANET};
        std::bitset<celflags::numCelestialFlags> flags;
        std::optional<double> gravity;
    };

    struct ExtraDescriptions {
        std::vector<std::pair<std::string_view, std::string_view>> data{};
    };

    struct DgScripts {
        std::vector<std::size_t> data{};
    };

    // Just yoinking this straight from the old game.
    struct obj_affected_type {
        obj_affected_type() = default;
        obj_affected_type(const nlohmann::json& j);
        obj_affected_type(int loc, int spec, int mod) : location(loc), specific(spec), modifier(mod) {};
        int location;       /* Which ability to change (APPLY_XXX) */
        int specific;       /* Some locations have parameters      */
        int modifier;       /* How much it changes by              */
        nlohmann::json serialize();
    };

    struct ItemModifiers {
        std::vector<obj_affected_type> data{};
    };

    struct skill_data {
        skill_data() = default;
        skill_data(const nlohmann::json& j);
        int8_t level{0};
        int8_t bonus{0};
        int8_t perfection{0};
        nlohmann::json serialize();
    };

    struct Skills {
        std::unordered_map<skill::SkillId, skill_data> data{};
    };


}