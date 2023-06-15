#pragma once
#include "dbatk/base.h"
#include "dbatk/object/flags.h"
#include "dbatk/object/size.h"
#include "dbatk/object/admin.h"
#include "dbatk/object/material.h"


namespace dbat {
    struct ExtraDescription {
        std::string_view keyword, description;
    };

    class Unit {
    public:
        [[nodiscard]] std::string getName() const {return std::string(name);};
        [[nodiscard]] std::string getShortDescription() const {return std::string(shortDescription);};
        [[nodiscard]] std::string getRoomDescription() const {return std::string(roomDescription);};
        [[nodiscard]] std::string getLookDescription() const {return std::string(lookDescription);};

        void setName(const std::string& txt) {name = intern(txt);};
        void setShortDescription(const std::string& txt) {shortDescription = intern(txt);};
        void getRoomDescription(const std::string& txt) {roomDescription = intern(txt);};
        void getLookDescription(const std::string& txt) {lookDescription = intern(txt);};
    protected:
        std::string_view name, shortDescription, roomDescription, lookDescription;
        std::vector<ExtraDescription> extraDescriptions;
        [[nodiscard]] nlohmann::json serializeBase() const;
        void deserializeBase(const nlohmann::json& j);
    };

    enum class SubLocType : std::size_t {
        INVENTORY = 0,
        EQUIPMENT = 1,
        ROOM = 2,
        GRID = 3,
        SECTOR = 4
    };


    // The sublocation is a variant of SubLocType.
    // Meaning, this object could be in something's inventory, a room, an equipment slot, a gridpoint,
    // or a sectorpoint. Inventory is represented by subLocation having an empty optional. Equipment
    // is a std::size_t.
    using SubLocation = std::optional<std::variant<Room*, std::size_t, GridPoint, SectorPoint>>;

    // AbsoluteLocation is used for transfers from one place to another.
    using AbsoluteLocation = std::tuple<ObjectID, SubLocType, SubLocation>;

    enum class TraverseType {
        // The character is voluntarily trying to move, an item is being picked up,
        // or a vehicle is moving under its own power. This should be used for all
        // cases of normal movement from place to place.
        NORMAL = 0,

        // The character is being moved by some kind of magical power.
        TELEPORT = 2,

        // The character is being moved by some kind of admin power.
        SYSTEM = 3

    };

    struct Traverse {
        TraverseType type{TraverseType::NORMAL};
        std::optional<AbsoluteLocation> from, to;
        std::optional<std::size_t> direction;
        bool forced{false};
        bool silent{false};
    };

    class Object : public Unit, public std::enable_shared_from_this<Object> {
    public:
        explicit Object(ObjectID id) : objectID(id) {};
        Object(ObjectID id, const nlohmann::json& j) : Object(id) {deserializeInstancePrelude(j);};
        virtual ~Object() = default;
        virtual std::string getTypeName() const = 0;

        const ObjectID& getID() const {return objectID;};

        std::shared_ptr<Object> getLocation() const {return location;};
        const std::list<std::shared_ptr<Object>>& getContents() const {return contents;};

        std::shared_ptr<Object> getParent() const {return parent;};
        const std::list<std::shared_ptr<Object>>& getChildren() const {return children;};
        std::shared_ptr<Object> getOwner() const {return owner;};
        const std::list<std::shared_ptr<Object>>& getAssets() const {return assets;};

        OpResult<> setLocation(const std::shared_ptr<Object>& newLocation);
        OpResult<> setParent(const std::shared_ptr<Object>& newParent);
        OpResult<> setOwner(const std::shared_ptr<Object>& newOwner);

        virtual void atDelete();
        virtual nlohmann::json serializeInstance() const;
        virtual nlohmann::json serializeProto() const;
        virtual void deserializeProto(const nlohmann::json& j);
        virtual void deserializeInstancePrelude(const nlohmann::json& j);
        virtual void deserializeInstanceFinale(const nlohmann::json& j);
        void setObjFlag(std::size_t flag, bool value);
        [[nodiscard]] bool checkObjFlag(std::size_t flag) const;
        Room* getRoom(RoomID id);

        virtual void setSession(const std::shared_ptr<Session>& newSession);

        virtual std::string renderPuppetHeader() {return "--- Your Puppet Saw ---";};
        virtual void sendText(const std::string& txt);
        virtual std::string renderPuppetFooter() {return "-----------------------";};

        virtual size::SizeID getSizeID() const;
        size::Size* getSize() const;

        virtual void setAdmFlag(std::size_t flag, bool value);
        [[nodiscard]] bool checkAdmFlag(std::size_t flag) const;

        virtual std::optional<std::size_t> findStat(const std::string& name) {return std::nullopt;};
        virtual double getStatBase(std::size_t stat) {return 0.0;};
        virtual double getStatBase(const std::string& stat);
        virtual double getStat(std::size_t stat) {return 0.0;};
        virtual double getStat(const std::string& stat);
        virtual void setStat(std::size_t stat, double value) {};
        virtual void setStat(const std::string& stat, double value);
        virtual void modStat(std::size_t stat, double value) {};
        virtual void modStat(const std::string& stat, double value);

        OpResult<Room*> createRoom(std::optional<RoomID> id = std::nullopt, std::optional<nlohmann::json> jdata = std::nullopt);

        virtual void setMaterial(std::size_t id);
        //virtual void setMaterial(const std::string& name);

        virtual mat::MaterialID getMaterialID();

        [[nodiscard]] virtual double getGravity() { return 1.0; };
        virtual void setGravity(double grav) {};

        virtual bool dirExistsFor(const std::shared_ptr<Object>& obj, std::size_t dir) {return false;};
        virtual OpResult<> checkDirection(const std::shared_ptr<Object>& obj, std::size_t dir, Traverse& traverse) { return {false, "No such direction."}; };


    protected:
        ObjectID objectID;
        std::shared_ptr<Session> session;

        std::shared_ptr<Object> location, parent, owner, puppet, puppeteer;
        std::list<std::shared_ptr<Object>> contents, children, assets;
        std::unique_ptr<std::unordered_map<RoomID, std::shared_ptr<Room>>> rooms;

        std::bitset<oflags::NUM_OBJ_FLAGS> objFlags;
        std::optional<size::SizeID> sizeId;

        SubLocation subLocation;

        // The serialize/deserialize routines are handling all save/load logic from disk for Objects and
        // subclasses of Object. Instances are actual active objects in the game world, while proto is for
        // prototypes used for spawning. Prototypes are not in the game world, don't have relations to
        // other Objects, and don't have ObjectIDs.
        virtual nlohmann::json serializeShared() const;
        virtual void deserializeShared(const nlohmann::json& j);
        virtual void addToContents(const std::shared_ptr<Object>& obj);
        virtual void atContentDeleted(const std::shared_ptr<Object>& obj);
        virtual void atHolderDeleted(const std::shared_ptr<Object>& obj);
        virtual void removeFromContents(const std::shared_ptr<Object>& obj);
        virtual void addToChildren(const std::shared_ptr<Object>& obj);
        virtual void atParentDeleted(const std::shared_ptr<Object>& obj);
        virtual void atChildDeleted(const std::shared_ptr<Object>& obj);
        virtual void removeFromChildren(const std::shared_ptr<Object>& obj);
        virtual void removeFromAssets(const std::shared_ptr<Object>& obj);
        virtual void addToAssets(const std::shared_ptr<Object>& obj);
        virtual void atOwnerDeleted(const std::shared_ptr<Object>& obj);
        virtual void atAssetDeleted(const std::shared_ptr<Object>& obj);

        mat::MaterialID materialId{mat::OTHER};

    private:
        RoomID getNextRoomID() const;
    };

}