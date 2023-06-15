#include "dbatk/object/object.h"
#include "dbatk/room/room.h"
#include "dbatk/session.h"


namespace dbat {

    void Object::setMaterial(std::size_t id) {
        if(id <= mat::NUM_MATERIALS < 1)
            materialId = static_cast<mat::MaterialID>(id);
    }

    mat::MaterialID Object::getMaterialID() {
        return materialId;
    }

    RoomID Object::getNextRoomID() const {
        auto groom = checkObjFlag(oflags::GLOBALROOMS);
        for(std::size_t i = 0; true; i++) {
            if(rooms && rooms->contains(i)) continue;
            if(groom && legacyRooms.contains(i)) continue;
            return i;
        }
    }

    OpResult<Room*> Object::createRoom(std::optional<RoomID> id, std::optional<nlohmann::json> jdata) {
        auto groom = checkObjFlag(oflags::GLOBALROOMS);
        if(!id) {
            id = getNextRoomID();
        } else {
            if(rooms && rooms->contains(id.value())) {
                return {nullptr, "Room already exists."};
            }
            if(groom && legacyRooms.contains(id.value())) {
                return {nullptr, "Room already exists."};
            }
        }
        if(!rooms) {
            rooms = std::make_unique<std::unordered_map<RoomID, std::shared_ptr<Room>>>();
        }

        auto room = jdata.has_value() ? std::make_shared<Room>(id.value(), this, jdata.value()) : std::make_shared<Room>(id.value(), this);
        rooms->emplace(id.value(), room);
        if(groom) {
            legacyRooms.emplace(id.value(), room.get());
        }
    }

    double Object::getStatBase(const std::string &stat) {
        auto find = findStat(stat);
        if(find) {
            return getStatBase(find.value());
        }
        return 0.0;
    }

    double Object::getStat(const std::string &stat) {
        auto find = findStat(stat);
        if(find) {
            return getStat(find.value());
        }
        return 0.0;
    }

    void Object::setStat(const std::string &stat, double value) {
        auto find = findStat(stat);
        if(find) {
            setStat(find.value(), value);
        }
    }

    void Object::modStat(const std::string &stat, double value) {
        auto find = findStat(stat);
        if(find) {
            modStat(find.value(), value);
        }
    }

    size::SizeID Object::getSizeID() const {
        return sizeId.value_or(size::MEDIUM);
    }

    size::Size* Object::getSize() const {
        return size::sizes[getSizeID()].get();
    }

    Room* Object::getRoom(dbat::RoomID id) {
        if(!rooms) return nullptr;
        auto found = rooms->find(id);
        if(found != rooms->end()) {
            auto &room = found->second;
            return room.get();
        }
        return nullptr;
    }

    void Object::setObjFlag(std::size_t flag, bool value) {
        // Error out if flag > NUM_EXIT_FLAGS
        if(flag > objFlags.size() - 1) return;
        auto self = shared_from_this();
        if(value) {
            if(objFlags.test(flag)) return;
            objFlags.set(flag);
            oflags::objectFlags[flag]->onSet(self);
        } else {
            if(!objFlags.test(flag)) return;
            objFlags.reset(flag);
            oflags::objectFlags[flag]->onClear(self);
        }
        setDirty(self);
    }

    bool Object::checkObjFlag(std::size_t flag) const {
        // Error out if flag > NUM_OBJ_FLAGS
        if(flag > objFlags.size() - 1) return false;
        return objFlags.test(flag);
    }

    void Object::setAdmFlag(std::size_t flag, bool value) {
        return;
    }

    bool Object::checkAdmFlag(std::size_t flag) const {
        return false;
    }

    OpResult<> Object::setLocation(const std::shared_ptr<Object>& newLocation) {
        auto self = shared_from_this();
        if(newLocation && !gameIsLoading) {
            // Check for a circular relationship and error out if found...
            auto cur = newLocation;
            while(cur) {
                if(cur == self) {
                    return {false, "That would cause a circular relationship."};
                }
                cur = cur->getLocation();
            }
        }

        if(location) {
            location->removeFromContents(self);
        }
        if(newLocation) {
            location = newLocation;
            location->addToContents(self);
        } else {
            location.reset();
        }
        setDirty(self);
        return {true, std::nullopt};
    }

    void Object::removeFromContents(const std::shared_ptr<Object>& obj) {
        contents.remove(obj);
    }

    void Object::addToContents(const std::shared_ptr<Object> &obj) {
        contents.emplace_back(obj);
    }

    OpResult<> Object::setParent(const std::shared_ptr<Object>& newParent) {
        auto self = shared_from_this();
        if(newParent && !gameIsLoading) {
            // Check for a circular relationship and error out if found...
            auto cur = newParent;
            while(cur) {
                if(cur == self) {
                    return {false, "That would cause a circular relationship."};
                }
                cur = cur->getParent();
            }
        }
        if(parent) {
            parent->removeFromChildren(self);
        }
        if(newParent) {
            parent = newParent;
            parent->addToChildren(self);
        } else {
            parent.reset();
        }
        setDirty(self);
        return {true, std::nullopt};
    }

    void Object::removeFromChildren(const std::shared_ptr<Object>& obj) {
        children.remove(obj);
    }

    void Object::addToChildren(const std::shared_ptr<Object> &obj) {
        children.emplace_back(obj);
    }

    void Object::removeFromAssets(const std::shared_ptr<Object>& obj) {
        assets.remove(obj);
    }

    void Object::addToAssets(const std::shared_ptr<Object> &obj) {
        assets.emplace_back(obj);
    }

    OpResult<> Object::setOwner(const std::shared_ptr<Object> &newOwner) {
        auto self = shared_from_this();
        if(newOwner && !gameIsLoading) {
            // Check for a circular relationship and error out if found...
            auto cur = newOwner;
            while(cur) {
                if(cur == self) {
                    return {false, "That would cause a circular relationship."};
                }
                cur = cur->getOwner();
            }
        }
        if(owner) {
            owner->removeFromAssets(self);
        }
        if(newOwner) {
            owner = newOwner;
            owner->addToAssets(self);
        } else {
            owner.reset();
        }
        setDirty(self);
        return {true, std::nullopt};
    }

    void Object::atContentDeleted(const std::shared_ptr<Object>& obj) {
        removeFromContents(obj);
    }

    void Object::atChildDeleted(const std::shared_ptr<Object>& obj) {
        removeFromChildren(obj);
    }

    void Object::atAssetDeleted(const std::shared_ptr<Object>& obj) {
        removeFromAssets(obj);
    }

    void Object::atHolderDeleted(const std::shared_ptr<Object>& obj) {
        setLocation(nullptr);
    }

    void Object::atParentDeleted(const std::shared_ptr<Object>& obj) {
        setParent(nullptr);
    }

    void Object::atOwnerDeleted(const std::shared_ptr<Object>& obj) {
        setOwner(nullptr);
    }

    void Object::atDelete() {
        auto self = shared_from_this();
        if(location) {
            location->atContentDeleted(self);
            location.reset();
        }
        if(parent) {
            parent->atChildDeleted(self);
            parent.reset();
        }
        if(owner) {
            owner->atAssetDeleted(self);
            owner.reset();
        }

        // We must copy the lists while iterating to avoid iterator invalidation.
        std::list<std::shared_ptr<Object>> temp;

        temp = contents;
        for(auto &c : temp) {
            c->atHolderDeleted(self);
        }

        temp = children;
        for(auto &c : temp) {
            c->atParentDeleted(self);
        }

        temp = assets;
        for(auto &c : temp) {
            c->atOwnerDeleted(self);
        }
        setDirty(self);

    }

    void Object::setSession(const std::shared_ptr<Session> &newSession) {
        session = newSession;
        // This function should be overloaded for player objects, big time.
    }

    void Object::sendText(const std::string &text) {
        if(session) {
            session->sendText(text);
        }
        if(puppeteer) {
            auto header = puppeteer->renderPuppetHeader();
            if(!header.empty()) puppeteer->sendText(header);
            puppeteer->sendText(text);
            auto footer = puppeteer->renderPuppetFooter();
            if(!footer.empty()) puppeteer->sendText(footer);
        }
    }



    nlohmann::json Unit::serializeBase() const {
        nlohmann::json j;

        if(!name.empty()) j["name"] = name;
        if(!shortDescription.empty()) j["shortDescription"] = shortDescription;
        if(!roomDescription.empty()) j["roomDescription"] = roomDescription;
        if(!lookDescription.empty()) j["lookDescription"] = lookDescription;

        if(!extraDescriptions.empty()) {
            for(const auto &ed : extraDescriptions) {
                j["extraDescriptions"].push_back(std::make_pair(ed.keyword, ed.description));
            }
        }

        return j;
    }

    void Unit::deserializeBase(const nlohmann::json &j) {
        if(j.contains("name")) name = intern(j["name"].get<std::string>());
        if(j.contains("shortDescription")) shortDescription = intern(j["shortDescription"].get<std::string>());
        if(j.contains("roomDescription")) roomDescription = intern(j["roomDescription"].get<std::string>());
        if(j.contains("lookDescription")) lookDescription = intern(j["lookDescription"].get<std::string>());

        if(j.contains("extraDescriptions")) {
            for(const auto &ed : j["extraDescriptions"]) {
                extraDescriptions.emplace_back(intern(ed[0].get<std::string>()), intern(ed[1].get<std::string>()));
            }
        }
    }

    nlohmann::json Object::serializeShared() const {
        auto j = serializeBase();

        for(auto i = 0; i < objFlags.size();i++) {
            if(objFlags.test(i)) {
                j["objFlags"].push_back(i);
            }
        }

        if(rooms) for(auto &[id, r] : *rooms) {
            j["rooms"].push_back(std::make_pair(id, r->serialize()));
        }

        return j;
    }

    void Object::deserializeShared(const nlohmann::json &j) {
        deserializeBase(j);

        if(j.contains("objFlags")) {
            for(const auto &f : j["objFlags"]) {
                objFlags.set(f.get<int>());
            }
        }

        if(j.contains("rooms")) {
            for(const auto &r : j["rooms"]) {
                auto [ro, err] = createRoom(r[0], r[1]);
                // TODO: handle errors here. errors would be really bad here.
            }
        }
    }

    nlohmann::json Object::serializeProto() const {
        return serializeShared();
    }

    void Object::deserializeProto(const nlohmann::json &j) {
        deserializeShared(j);
    }

    nlohmann::json Object::serializeInstance() const {
        auto j = serializeShared();

        if(location) j["location"] = location->getID();
        if(parent) j["parent"] = parent->getID();
        if(owner) j["owner"] = owner->getID();


        return j;
    }

    void Object::deserializeInstancePrelude(const nlohmann::json &j) {
        deserializeShared(j);

        if(checkObjFlag(oflags::GLOBALROOMS)) {
            if(rooms) {
                // we must add all rooms to the legacyRooms...
                for(auto &[id, r] : *rooms) {
                    legacyRooms[id] = r.get();
                }
            }
        }

    }

    void Object::deserializeInstanceFinale(const nlohmann::json &j) {

        // Now that all objects have been instantiated, we can resolve and rebuild relationships.
        if(j.contains("location")) {
            ObjectID locID(j["location"]);
            auto loc = locID.getObject();
            if(loc) {
                setLocation(loc);
            }
        }

        if(j.contains("parent")) {
            ObjectID parentID(j["parent"]);
            auto p = parentID.getObject();
            if(p) {
                setParent(p);
            }
        }

        if(j.contains("owner")) {
            ObjectID ownerID(j["owner"]);
            auto o = ownerID.getObject();
            if(o) {
                setOwner(o);
            }
        }
    }

}