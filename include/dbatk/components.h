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
#include "dbatk/aspects/playerFlags.h"
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
        entt::entity data{entt::null};
    };

    struct ReverseEntity {
        std::vector<entt::entity> data{};
    };

    struct Inventory {
        std::vector<entt::entity> data;
    };

    struct Equipment {
        std::unordered_map<std::size_t, entt::entity> data;
    };

    struct Contents : ReverseEntity {};
    struct Parent : Entity {};
    struct Children : ReverseEntity {};
    struct Owner : Entity {};
    struct Assets : ReverseEntity {};

    struct SessionHolder {
        std::shared_ptr<Session> data;
        uint8_t sessionMode{0};
    };

    // An Area is a collection of Rooms indexed by their RoomId, used for very legacy
    // style MUD designs where rooms are linked by exits.
    // We don't actually define Exits as a component type here given how each MUD
    // may do those differently, but they are likely a Component attached to the entity
    // in the unordered_map of Area.
    struct Area {
        std::unordered_map<RoomId, entt::entity> data{};
    };

    // Meant to be used by the entt::entity within the Area component map.
    struct Room {
        ObjectId obj;
        RoomId id;
    };

    struct RoomContents {
        std::vector<entt::entity> data{};
    };

    // Used for Maps and Expanses to limit the size of their coordinates.
    struct AbstractGrid {
        GridLength minX = std::numeric_limits<GridLength>::min();
        GridLength maxX = std::numeric_limits<GridLength>::max();
        GridLength minY = std::numeric_limits<GridLength>::min();
        GridLength maxY = std::numeric_limits<GridLength>::max();
        GridLength minZ = std::numeric_limits<GridLength>::min();
        GridLength maxZ = std::numeric_limits<GridLength>::max();
    };

    // An Expanse is a kind of Grid, so it will use GridContents and GridLocation.
    // An Expanse is a kind of Grid where all points within the min/max coordinates
    // are valid locations for someone to be in. This allows for the illusion of a vast
    // area that's mostly empty but does have some cool things here and there in it.
    struct Expanse : AbstractGrid {
        std::unordered_map<GridPoint, entt::entity> poi{};
    };

    // A Map is a kind of Grid, so it will use GridContents and GridLocation.
    // It is the opposite of an Expanse; only points which are in poi are valid
    // locations. This system doesn't dictate how the points of interest are connected.
    // That's up to the MUD to decide. A common one might be to simply use compass point
    // directions such that if here.x +1 exists, one can go north.
    struct Map : AbstractGrid {
        std::unordered_map<GridPoint, entt::entity> poi{};
    };

    struct GridContents {
        std::unordered_map<GridPoint, std::vector<entt::entity>> data{};
    };

    // Used for Space.
    struct AbstractSector {
        SectorLength minX = std::numeric_limits<SectorLength>::min();
        SectorLength maxX = std::numeric_limits<SectorLength>::max();
        SectorLength minY = std::numeric_limits<SectorLength>::min();
        SectorLength maxY = std::numeric_limits<SectorLength>::max();
        SectorLength minZ = std::numeric_limits<SectorLength>::min();
        SectorLength maxZ = std::numeric_limits<SectorLength>::max();
    };

    // Space is like an Expanse, but it uses floating point (double) logic for its
    // coordinates. This allows for more complex positioning of things, but is harder
    // to administrate in a text-based game. The usefulness of this may be limited,
    // but it's provided because some games love 3D spaceship games moving around
    // complex coordinate planes even if that's hard to represent in text.
    struct Space : AbstractSector {
        std::unordered_map<SectorPoint, entt::entity> poi{};
    };

    struct SectorContents {
        std::unordered_map<SectorPoint, std::vector<entt::entity>> data{};
    };

    struct Item {

    };

    struct Character {

    };

    struct NPC {

    };

    struct Player {
        int64_t accountId{-1};
    };

    struct Prototype {
        std::string_view data;
    };

    struct Vehicle {};

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

    struct RoomFlags {
        std::bitset<rflags::numRoomFlags> data;
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

    struct CharacterStats {
        std::array<double, cstat::numCharStats> data;
    };

    struct NPCFlags {
        std::bitset<nflags::numNpcFlags> data;
    };

    struct PlayerFlags {
        std::bitset<pflags::countPlayerFlags> data;
    };

    struct PreferenceFlags {
        std::bitset<pref::numPrfFlags> data;
    };

    struct Gravity {
        double data{1.0};
    };

    struct CelestialBody {
        celtype::CelestialTypeId type{celtype::PLANET};
        std::bitset<celflags::numCelestialFlags> flags;
    };

    struct Structure {

    };

    struct ExtraDescriptions {
        std::vector<std::pair<std::string_view, std::string_view>> data{};
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

    struct Dimension {};

    struct ItemVnum {
        std::size_t data{0};
    };

    struct NPCVnum {
        std::size_t data{0};
    };

    struct PendingCommand {
        std::list<std::string> inputQueue;
        double waitTime{0.0};
    };

    struct LegacyLoadRoom {
        RoomId id;
    };


}