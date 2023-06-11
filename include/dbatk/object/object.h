#pragma once
#include "dbatk/base.h"
#include "dbatk/object/flags.h"
#include "dbatk/object/size.h"


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

    protected:
        ObjectID objectID;
        std::shared_ptr<Session> session;

        std::shared_ptr<Object> location, parent, owner, puppet, puppeteer;
        std::list<std::shared_ptr<Object>> contents, children, assets;
        std::unique_ptr<std::unordered_map<RoomID, std::shared_ptr<Room>>> rooms;

        std::bitset<oflags::NUM_OBJ_FLAGS> objFlags;
        std::optional<size::SizeID> sizeId;

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
    };
}