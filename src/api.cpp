#include "dbatk/api.h"
#include "dbatk/components.h"
#include "dbatk/color.h"
#include "dbatk/session.h"
#include "dbatk/operations/movement.h"
#include "dbatk/operations/information.h"
#include "dbatk/commands.h"
#include "dbatk/triggers.h"
#include "dbatk/grammar.h"
#include "dbatk/message.h"

namespace dbat {

    void setName(entt::entity ent, const std::string &txt) {
        setBaseText<Name>(ent, txt);
    }

    void setRoomDescription(entt::entity ent, const std::string &txt) {
        setBaseText<RoomDescription>(ent, txt);
    }

    void setLookDescription(entt::entity ent, const std::string &txt) {
        setBaseText<LookDescription>(ent, txt);
    }

    std::string getName(entt::entity ent) {
        auto name = getBaseText<Name>(ent);
        if(!name.empty()) return std::string(name);
        return "Unnamed Object";
    }

    std::string getRoomDescription(entt::entity ent) {
        return std::string(getBaseText<RoomDescription>(ent));
    }

    std::string getLookDescription(entt::entity ent) {
        return std::string(getBaseText<LookDescription>(ent));
    }

    std::string getDisplayName(entt::entity ent, entt::entity looker, bool matrix) {
        if(registry.any_of<Item, NPC>(ent)) {
            return getName(ent);
        }

        if(registry.any_of<Character>(ent)) {
            if(registry.any_of<Player>(ent) && registry.any_of<Player>(looker)) {
                // We need to use the dub system here.
                return getName(ent);
            }
            return getName(ent);
        }
        return getName(ent);
    }

    std::string getRoomLine(entt::entity ent, entt::entity looker, bool matrix) {
        if(registry.any_of<Item, NPC>(ent)) {
            return getRoomDescription(ent);
        }

        if(registry.any_of<Character>(ent)) {
            if(registry.any_of<Player>(ent) && registry.any_of<Player>(looker)) {
                // We need to use the dub system here... but a placeholder is used for now.
                return getDisplayName(ent, looker) + " is here.";
            }
            return getDisplayName(ent, looker) + " is here.";
        }
        return getDisplayName(ent, looker) + " is here.";
    }

    // A proper implementation of this will probably use switching logic to run different functions
    // based on the type of entity. This is just a mockup.
    std::string renderAppearance(entt::entity ent, entt::entity looker) {
        std::vector<std::string> out;
        out.push_back(getDisplayName(ent, looker));
        auto rdesc = getRoomDescription(ent);
        if(!rdesc.empty()) out.push_back(rdesc);
        return boost::join(out, "\n");
    }

    ObjectId getObjectId(entt::entity ent) {
        return registry.get<ObjectId>(ent);
    }

    OpResult<> setParent(entt::entity ent, entt::entity target) {
        if(registry.valid(target)) {
            // We must look up the parent chain to ensure no shenanigans.
            auto parent = target;
            while(registry.valid(parent)) {
                if(parent == ent) {
                    return {false, "That would cause a recursive relationship... very bad."};
                }
                parent = getParent(parent);
            }
        }

        entt::entity oldParent = getParent(ent);
        if(registry.valid(oldParent)) {
            removeFromChildren(oldParent, ent);
        }

        if(registry.valid(target)) {
            addToChildren(target, ent);
            auto &par = registry.get_or_emplace<Parent>(ent);
            par.data = target;
        } else {
            registry.remove<Parent>(ent);
        }
        return {true, std::nullopt};

    }

    entt::entity getParent(entt::entity ent) {
        if(auto par = registry.try_get<Parent>(ent)) {
            return par->data;
        }
        return entt::null;
    }

    void addToChildren(entt::entity ent, entt::entity child) {
        if(registry.valid(ent)) {
            auto &children = registry.get_or_emplace<Children>(ent);
            children.data.push_back(child);
        }
    }

    void removeFromChildren(entt::entity ent, entt::entity child) {
        if(registry.valid(ent)) {
            auto &children = registry.get_or_emplace<Children>(ent);
            children.data.erase(std::remove(children.data.begin(), children.data.end(), child), children.data.end());
        }
    }

    std::vector<entt::entity> getChildren(entt::entity ent) {
        if(registry.valid(ent)) {
            if(auto children = registry.try_get<Children>(ent)) {
                return children->data;
            }
        }
        return {};
    }

    void atChildDeleted(entt::entity ent, entt::entity target) {
        removeFromChildren(ent, target);
    }

    void atParentDeleted(entt::entity ent, entt::entity target) {
        setParent(ent, entt::null);
    }

    OpResult<> setOwner(entt::entity ent, entt::entity target) {
        if(registry.valid(target)) {
            // We must look up the parent chain to ensure no shenanigans.
            auto parent = target;
            while(registry.valid(parent)) {
                if(parent == ent) {
                    return {false, "That would cause a recursive relationship... very bad."};
                }
                parent = getOwner(parent);
            }
        }

        entt::entity oldOwner = getOwner(ent);
        if(registry.valid(oldOwner)) {
            removeFromAssets(oldOwner, ent);
        }

        if(registry.valid(target)) {
            addToAssets(target, ent);
            auto &par = registry.get_or_emplace<Owner>(ent);
            par.data = target;
        } else {
            registry.remove<Owner>(ent);
        }
        return {true, std::nullopt};

    }

    entt::entity getOwner(entt::entity ent) {
        if(auto par = registry.try_get<Owner>(ent)) {
            return par->data;
        }
        return entt::null;
    }

    void addToAssets(entt::entity ent, entt::entity child) {
        if(registry.valid(ent)) {
            auto &children = registry.get_or_emplace<Assets>(ent);
            children.data.push_back(child);
        }
    }

    void removeFromAssets(entt::entity ent, entt::entity child) {
        if(registry.valid(ent)) {
            auto &children = registry.get_or_emplace<Assets>(ent);
            children.data.erase(std::remove(children.data.begin(), children.data.end(), child), children.data.end());
        }
    }

    std::vector<entt::entity> getAssets(entt::entity ent) {
        if(registry.valid(ent)) {
            if(auto children = registry.try_get<Assets>(ent)) {
                return children->data;
            }
        }
        return {};
    }

    void atAssetDeleted(entt::entity ent, entt::entity target) {
        removeFromAssets(ent, target);
    }

    void atOwnerDeleted(entt::entity ent, entt::entity target) {
        setOwner(ent, entt::null);
    }

    OpResult<> canUnequip(entt::entity ent, entt::entity item, MoveParams& params) {
        return {true, std::nullopt};
    }

    OpResult<> canRemoveFromInventory(entt::entity ent, entt::entity item, MoveParams& params) {
        return {true, std::nullopt};
    }

    OpResult<> canLeaveRoom(entt::entity ent, entt::entity room, MoveParams& params) {
        return {true, std::nullopt};
    }

    OpResult<> canLeavePOI(entt::entity ent, entt::entity poi, MoveParams& params) {
        return {true, std::nullopt};
    }

    OpResult<> canEquip(entt::entity ent, entt::entity item, MoveParams& params) {
        return {true, std::nullopt};
    }

    OpResult<> canAddToInventory(entt::entity ent, entt::entity item, MoveParams& params) {
        if(registry.all_of<NPC, DgScripts>(ent)) {
            if(params.moveType == MoveType::Give) {
                // The NPC is being given something. We need to check if they can accept it.
                if(auto mp = registry.try_get<MoneyPile>(item); mp) {
                    // Avoid accidentally giving money to a mob who won't accept it.
                    if(!mobBribeTrigger(params.mover, ent, mp->data, true)) {
                        return {false, "They don't need it."};
                    }
                } else {
                    // It's a normal item. Check if they can accept it.
                    if(!mobReceiveTrigger(params.mover, ent, item, true)) {
                        return {false, "They don't need it."};
                    }
                }
            }
        }

        return {true, std::nullopt};
    }

    OpResult<> canEnterRoom(entt::entity ent, entt::entity room, MoveParams& params) {
        return {true, std::nullopt};
    }

    OpResult<> canEnterPOI(entt::entity ent, entt::entity poi, MoveParams& params) {
        return {true, std::nullopt};
    }

    void unequip(entt::entity ent, entt::entity item, MoveParams& params) {
        registry.erase<Location>(item);
    }

    void removeFromInventory(entt::entity ent, entt::entity item, MoveParams& params) {



        if(auto i = registry.try_get<Inventory>(ent); i) {
            i->data.erase(std::remove(i->data.begin(), i->data.end(), item), i->data.end());
            if(i->data.empty()) {
                registry.remove<Inventory>(ent);
            }
        }
        registry.erase<Location>(item);
    }

    void leaveRoom(entt::entity ent, entt::entity room, MoveParams& params) {
        Location l;
        auto &r = registry.get<Room>(room);
        l.data = r.obj.getObject();
        l.x = r.id;
        l.locationType = LocationType::Area;

        if(!params.quiet) {
            if(params.traverseType == TraverseType::Physical) {
                if(params.moveType == MoveType::Traverse) {
                    // this is probably a character moving in a direction...
                    if(params.dir) {
                        MsgFormat m(ent, fmt::format("$You() $conj(leave) {}.", dir::directions[params.dir.value()].getName()));
                        m.in(l);
                        m.exclude(ent);
                        m.send();
                    } else {
                        MsgFormat m(ent, "$You() $conj(leave) this place.");
                        m.in(l);
                        m.send();
                    }
                }
                else if(params.moveType == MoveType::Get) {
                    // this is an item being gotten from a room...
                    // it is covered by addToInventory's announce.
                }
            }
            else if(params.traverseType == TraverseType::Teleport) {
                MsgFormat m(ent, "$You() vanishes in a flash of light!");
                m.in(l);
                m.exclude(ent);
                m.send();
            }
            else if(params.traverseType == TraverseType::System) {
                MsgFormat m(ent, "$You() vanishes in a flash of light!");
                m.in(l);
                m.exclude(ent);
                m.send();
            }
        }


        auto &rc = registry.get_or_emplace<RoomContents>(room);
        std::erase_if(rc.data, [&](auto e) { return e == ent; });
        if(rc.data.empty()) registry.erase<RoomContents>(room);
        registry.erase<Location>(ent);
    }

    void leavePOI(entt::entity ent, entt::entity poi, MoveParams& params) {
        registry.erase<Location>(ent);
    }

    void equip(entt::entity ent, entt::entity item, MoveParams& params) {

    }


    void addToInventory(entt::entity ent, entt::entity item, MoveParams& params) {
        if(registry.all_of<NPC, DgScripts>(ent)) {
            if(params.moveType == MoveType::Give) {
                // The NPC is being given something. Call the appropriate triggers.
                if(auto mp = registry.try_get<MoneyPile>(item); mp) {
                    mobBribeTrigger(params.mover, ent, mp->data);
                } else {
                    mobReceiveTrigger(params.mover, ent, item);
                }
            }
        }

        if(!params.quiet) {
            if(params.moveType == MoveType::Get) {
                auto prev = params.previousLocation.value();
                if(prev.locationType == LocationType::Area) {
                    // it was in a room...
                    MsgFormat m(ent, "$You() $conj(get) $you(item).");
                    m.set("item", item);
                    m.in(prev);
                    m.send();
                }
                else if(prev.locationType == LocationType::Inventory) {
                    // it was in a container...
                    MsgFormat m(ent, "$You() $conj(get) $you(item) from $you(container).");
                    m.set("item", item);
                    m.set("container", prev.data);
                    if(auto loc = registry.try_get<Location>(ent); loc) {
                        m.in(*loc);
                    }
                    m.send();
                }
            }
            else if(params.moveType == MoveType::Give) {
                MsgFormat m(params.mover, "$You() $conj(give) $you(item) to $you(target).");
                m.set("item", item);
                m.set("target", ent);
                if(auto loc = registry.try_get<Location>(ent); loc) {
                    m.in(*loc);
                }
                m.send();
            }
            else if(params.moveType == MoveType::Put) {
                MsgFormat m(params.mover, "$You() $conj(put) $you(item) in $you(container).");
                m.set("item", item);
                m.set("container", ent);
                if(auto loc = registry.try_get<Location>(ent); loc) {
                    m.in(*loc);
                }
                m.send();
            }
        }

        auto &i = registry.get_or_emplace<Inventory>(ent);
        i.data.emplace_back(item);

        auto &l = registry.get_or_emplace<Location>(item);
        l.data = ent;
        l.locationType = LocationType::Inventory;

    }

    void enterRoom(entt::entity ent, entt::entity room, MoveParams& params) {
        auto &loc = registry.get_or_emplace<Location>(ent);
        auto &d = params.dest.value();
        loc.data = d.data;
        loc.x = d.x;
        loc.locationType = d.locationType;

        auto &rc = registry.get_or_emplace<RoomContents>(room);
        rc.data.push_back(ent);

        if(!params.quiet) {
            if(params.moveType == MoveType::Traverse) {
                // This is normal movement from room to room. It's probably directional, and probably
                // a person or vehicle, though an item isn't out of the question...
                if(params.dir) {
                    // We will generate a message like "Bob walks in from the north."
                    auto opposite = dir::directions[params.dir.value()].getRevName();
                    MsgFormat m(ent, fmt::format("$You() $conj(walk) in from the {}.", opposite));
                    m.in(loc);
                    m.exclude(ent);
                    m.send();
                } else {
                    // We will need to make something generic... check if traverseType is teleport though.
                    if(params.traverseType == TraverseType::Teleport) {
                        MsgFormat m(ent, "$You() $conj(appear) in a flash of light!");
                        m.in(loc);
                        m.exclude(ent);
                        m.send();
                    } else {
                        MsgFormat m(ent, "$You() $conj(walk) in.");
                        m.in(loc);
                        m.exclude(ent);
                        m.send();
                    }
                }
            }
            else if(params.moveType == MoveType::Drop) {
                // This is an item being dropped by params.mover.
                MsgFormat m(params.mover, "$You() $conj(drop) $you(item).");
                m.set("item", ent);
                m.in(loc);
                m.send();
            }
        }



        // triggers.
        bool reverse = false;
        if(params.traverseType != TraverseType::System) {
            reverse = !mobTriggerGreet(ent, params.dir);
            if(!reverse) reverse = !mobTriggerEntry(ent);
            if(!reverse) reverse = !roomTriggerEntry(ent, room, params.dir);
        }

        if(reverse) {
            if(params.previousLocation) {
                MoveParams mp = params;
                mp.previousLocation = loc;
                mp.dest = params.previousLocation;
                mp.force = true;
                mp.dir.reset(); // maybe need to reverse directions here...
                // should probably set some kind of safeguard on MoveParams to prevent infinite ping-ponging.
                moveTo(ent, mp);
                return;
            }
        }

        if(registry.any_of<SessionHolder>(ent)) {
            sendLine(ent, op::renderLocation(loc, ent));
        }

    }

    void enterPOI(entt::entity ent, entt::entity poi, MoveParams& params) {

    }


    OpResult<> moveTo(entt::entity ent, MoveParams& params) {

        auto loc = registry.try_get<Location>(ent);
        if(loc) params.previousLocation = *loc;

        if(params.dest) {
            auto &d = params.dest.value();
            // verify that d is valid.
            if(!registry.valid(d.data)) {
                return {false, "Invalid destination."};
            }

            // Avoid self-references...
            if(d.data == ent) {
                return {false, "Can't move to self. The universe might implode!"};
            }

            // Check each LocationType to make sure it's valid.
            if(d.locationType == LocationType::Inventory) {
                // Inventory is only valid if d.data is a Character, Item, or Vehicle...
                if(!registry.any_of<Character,Item,Vehicle>(d.data)) {
                    return {false, "Invalid destination. Only Characters, Items, and Vehicles possess inventories."};
                }
                // And if ent is NOT a Character.
                if(!registry.any_of<Item>(ent)) {
                    return {false, "Invalid destination. Only Items may inhabit inventories."};
                }
            } else if(d.locationType == LocationType::Equipment) {
                // Only Characters, Items, and Vehicles may have equipment.
                if(!registry.any_of<Character,Item,Vehicle>(d.data)) {
                    return {false, "Invalid destination. Only Characters, Items, and Vehicles possess equipment."};
                }
                // And only Items may be equipped.
                if(!registry.any_of<Item>(ent)) {
                    return {false, "Invalid destination. Only Items may be equipped."};
                }
                // TODO: Check if d.x is a valid equipment slot for d.data.
            } else if(d.locationType == LocationType::Area) {
                // Only Characters, Items, and Vehicles may inhabit Areas.
                if(!registry.any_of<Character,Item,Vehicle,Structure>(ent)) {
                    return {false, "Invalid destination. Only Characters, Items, and Vehicles may inhabit Areas."};
                }
                // And d.data must have an Area component.
                if(auto area = registry.try_get<Area>(d.data)) {
                    // And d.x must be a valid room id in area.data...
                    if(!area->data.contains(d.x)) {
                        return {false, "Invalid destination. Area does not contain specified room."};
                    }
                } else {
                    return {false, "Invalid destination. Only Areas may be inhabited as Areas."};
                }
            } else if(d.locationType == LocationType::Expanse) {
                // Pretty much same conditions as Area but using Expanse component..
                if(!registry.any_of<Character,Item,Vehicle,Structure>(ent)) {
                    return {false, "Invalid destination. Only Characters, Items, and Vehicles may inhabit Expanse."};
                }
                if(auto exp = registry.try_get<Expanse>(d.data)) {
                    // d.x, y, and z must exist within the Expanse bounds of minX/maxX/etc.
                    if(d.x < exp->minX || d.x > exp->maxX || d.y < exp->minY || d.y > exp->maxY || d.z < exp->minZ || d.z > exp->maxZ) {
                        return {false, "Invalid destination. Coordinates are outside of Expanse bounds."};
                    }
                } else {
                    return {false, "Invalid destination. Only Expanses may be inhabited as Expanses."};
                }
            } else if(d.locationType == LocationType::Map) {
                // Pretty much same conditions as Area but using Map component..
                if(!registry.any_of<Character,Item,Vehicle,Structure>(ent)) {
                    return {false, "Invalid destination. Only Characters, Items, and Vehicles may inhabit Maps."};
                }
                if(auto map = registry.try_get<Map>(d.data)) {
                    GridPoint gp(d.x, d.y, d.z);
                    // gp must be contained within map->data.
                    if(!map->poi.contains(gp)) {
                        return {false, "Invalid destination. Map does not contain specified GridPoint."};
                    }
                } else {
                    return {false, "Invalid destination. Only Maps may be inhabited as Maps."};
                }
            } else if(d.locationType == LocationType::Space) {
                // Pretty much same conditions as Area but using Space component..
                if(!registry.any_of<Character,Item,Vehicle,CelestialBody>(ent)) {
                    return {false, "Invalid destination. Only Characters, Items, Vehicles, and CelestialBodies may inhabit Space."};
                }
                if(auto space = registry.try_get<Space>(d.data)) {
                    SectorPoint sp(d.x, d.y, d.z);
                    // d.x, y, and z must exist within the Space bounds of minX/maxX/etc.
                    if(sp.x < space->minX || sp.x > space->maxX || sp.y < space->minY || sp.y > space->maxY || sp.z < space->minZ || sp.z > space->maxZ) {
                        return {false, "Invalid destination. Coordinates are outside of Space bounds."};
                    }
                } else {
                    return {false, "Invalid destination. Only Space may be inhabited as Space."};
                }
            } else {
                return {false, "Invalid destination. Unknown enum value, contact staff."};
            }

            if(loc) {
                switch(loc->locationType) {
                    case LocationType::Inventory:
                        if(d.locationType == LocationType::Inventory && loc->data == d.data) {
                            return {false, "Already there."};
                        }
                        break;
                    case LocationType::Equipment:
                        if(d.locationType == LocationType::Equipment && loc->data == d.data && loc->x == d.x) {
                            return {false, "Already there."};
                        }
                        break;
                    case LocationType::Expanse:
                    case LocationType::Map:
                    case LocationType::Space:
                        if(d.locationType == loc->locationType
                           && loc->data == d.data
                           && loc->x == d.x && loc->y == d.y && loc->z == d.z) {
                            return {false, "Already there."};
                        }
                        break;
                    case LocationType::Area:
                        if(d.locationType == loc->locationType
                           && loc->data == d.data
                           && loc->x == d.x) {
                            return {false, "Already there."};
                        }
                        break;
                }

                auto parent = d.data;
                while(registry.valid(parent)) {
                    if(parent == ent) {
                        return {false, "That would cause a recursive relationship... very bad."};
                    }
                    auto l = registry.try_get<Location>(parent);
                    if(!l) {
                        break;
                    }
                    parent = l->data;
                }
            }
        }

        if(loc) {
            // We must check each kind of location type to see if we can leave it.
            if(loc->locationType == LocationType::Inventory) {
                auto [canRemove, reas3] = canRemoveFromInventory(loc->data, ent, params);
                if(!canRemove) {
                    return {false, reas3};
                }
            } else if(loc->locationType == LocationType::Equipment) {
                auto [canRemove, reas3] = canUnequip(loc->data, ent, params);
                if(!canRemove) {
                    return {false, reas3};
                }
            } else if(loc->locationType == LocationType::Area) {
                auto &area = registry.get<Area>(loc->data);
                auto &room = area.data[loc->x];
                auto [canLeave, reas3] = canLeaveRoom(ent, room, params);
                if(!canLeave) {
                    return {false, reas3};
                }
            } else if(loc->locationType == LocationType::Expanse) {
                auto &area = registry.get<Expanse>(loc->data);
                GridPoint gp(loc->x, loc->y, loc->z);
                auto found = area.poi.find(gp);
                if(found != area.poi.end()) {
                    auto [canLeave, reas3] = canLeavePOI(ent, found->second, params);
                    if(!canLeave) {
                        return {false, reas3};
                    }
                }
            } else if(loc->locationType == LocationType::Map) {
                auto &map = registry.get<Map>(loc->data);
                GridPoint gp(loc->x, loc->y, loc->z);
                auto &room = map.poi[gp];
                auto [canLeave, reas3] = canLeaveRoom(ent, room, params);
                if(!canLeave) {
                    return {false, reas3};
                }
            } else if(loc->locationType == LocationType::Space) {

            }
        }

        if(params.dest) {
            auto d = params.dest.value();
            if(d.locationType == LocationType::Inventory) {
                auto [canAdd, reas2] = canAddToInventory(d.data, ent, params);
                if(!canAdd) {
                    return {false, reas2};
                }
            } else if(d.locationType == LocationType::Equipment) {
                auto [canAdd, reas2] = canEquip(d.data, ent, params);
                if(!canAdd) {
                    return {false, reas2};
                }
            } else if(d.locationType == LocationType::Area) {
                auto &area = registry.get<Area>(d.data);
                auto &room = area.data[d.x];
                auto [canEnter, reas2] = canEnterRoom(ent, room, params);
                if(!canEnter) {
                    return {false, reas2};
                }
            } else if(d.locationType == LocationType::Expanse) {
                auto &area = registry.get<Expanse>(d.data);
                GridPoint gp(d.x, d.y, d.z);
                auto found = area.poi.find(gp);
                if(found != area.poi.end()) {
                    auto [canEnter, reas2] = canEnterPOI(ent, found->second, params);
                    if(!canEnter) {
                        return {false, reas2};
                    }
                }
            } else if(d.locationType == LocationType::Map) {
                auto &map = registry.get<Map>(d.data);
                GridPoint gp(d.x, d.y, d.z);
                auto &room = map.poi[gp];
                auto [canEnter, reas2] = canEnterRoom(ent, room, params);
                if(!canEnter) {
                    return {false, reas2};
                }
            } else if(d.locationType == LocationType::Space) {

            }
        }

        // if we've reached this point, the operation WILL proceed. Nothing can stop us. All validations have passed,
        // and all hooks have been called. We can now do the actual work.

        if(loc) {
            if(loc->locationType == LocationType::Inventory) {
                removeFromInventory(loc->data, ent, params);
            } else if(loc->locationType == LocationType::Equipment) {
                unequip(loc->data, ent, params);
            } else if(loc->locationType == LocationType::Area) {
                auto &area = registry.get<Area>(loc->data);
                auto &room = area.data[loc->x];
                leaveRoom(ent, room, params);
            } else if(loc->locationType == LocationType::Expanse) {
                auto &area = registry.get<Expanse>(loc->data);
                GridPoint gp(loc->x, loc->y, loc->z);
                auto found = area.poi.find(gp);
                if(found != area.poi.end()) {
                    leavePOI(ent, found->second, params);
                }
            } else if(loc->locationType == LocationType::Map) {
                auto &map = registry.get<Map>(loc->data);
                GridPoint gp(loc->x, loc->y, loc->z);
                auto &room = map.poi[gp];
                leaveRoom(ent, room, params);
            } else if(loc->locationType == LocationType::Space) {

            }
        }

        if(params.dest) {
            auto d = params.dest.value();
            if(d.locationType == LocationType::Inventory) {
                addToInventory(d.data, ent, params);
            } else if(d.locationType == LocationType::Equipment) {
                equip(d.data, ent, params);
            } else if(d.locationType == LocationType::Area) {
                auto &area = registry.get<Area>(d.data);
                auto &room = area.data[d.x];
                enterRoom(ent, room, params);
            } else if(d.locationType == LocationType::Expanse) {
                auto &area = registry.get<Expanse>(d.data);
                GridPoint gp(d.x, d.y, d.z);
                auto found = area.poi.find(gp);
                if(found != area.poi.end()) {
                    enterPOI(ent, found->second, params);
                }
            } else if(d.locationType == LocationType::Map) {
                auto &map = registry.get<Map>(d.data);
                GridPoint gp(d.x, d.y, d.z);
                auto &room = map.poi[gp];
                enterRoom(ent, room, params);
            } else if(d.locationType == LocationType::Space) {

            }
        }

        // Hooray, we made it!
        setDirty(ent);
        return {true, std::nullopt};

    }

    entt::entity getLocation(entt::entity ent) {
        if(auto par = registry.try_get<Location>(ent)) {
            return par->data;
        }
        return entt::null;
    }

    void addToContents(entt::entity ent, entt::entity child) {
        if(registry.valid(ent)) {
            auto &children = registry.get_or_emplace<Contents>(ent);
            children.data.push_back(child);
        }
    }

    void removeFromContents(entt::entity ent, entt::entity child) {
        if(registry.valid(ent)) {
            auto &children = registry.get_or_emplace<Contents>(ent);
            children.data.erase(std::remove(children.data.begin(), children.data.end(), child), children.data.end());
        }
    }

    std::vector<entt::entity> getContents(entt::entity ent) {
        if(registry.valid(ent)) {
            if(auto children = registry.try_get<Contents>(ent)) {
                return children->data;
            }
        }
        return {};
    }

    void atContentDeleted(entt::entity ent, entt::entity target) {
        removeFromContents(ent, target);
    }

    void atLocationDeleted(entt::entity ent, entt::entity target) {

    }

    void atDeleteObject(entt::entity ent) {
        if(auto par = registry.try_get<Parent>(ent)) {
            atChildDeleted(par->data, ent);
        }
        if(auto par = registry.try_get<Children>(ent)) {
            for(auto child : par->data) {
                atParentDeleted(child, ent);
            }
        }
        if(auto par = registry.try_get<Owner>(ent)) {
            atAssetDeleted(par->data, ent);
        }
        if(auto par = registry.try_get<Assets>(ent)) {
            for(auto child : par->data) {
                atOwnerDeleted(child, ent);
            }
        }
        if(auto par = registry.try_get<Location>(ent)) {
            atContentDeleted(par->data, ent);
        }
        if(auto par = registry.try_get<Contents>(ent)) {
            for(auto child : par->data) {
                atLocationDeleted(child, ent);
            }
        }
    }

    std::set<std::string> getSearchWords(entt::entity ent, entt::entity looker) {
        std::set<std::string> words;
        if(auto key = registry.try_get<Keywords>(ent) ;key) {
            boost::split(words, key->data, boost::algorithm::is_space());
        } else {
            boost::split(words, stripAnsi(getDisplayName(ent, looker)), boost::algorithm::is_space());
        }
        return words;
    }

    bool checkSearch(entt::entity ent, std::string_view term, entt::entity looker) {
        auto w = getSearchWords(ent, looker);
        std::vector<std::string> words;
        // Sort w into words by string length, smallest first and longest last.
        std::transform(w.begin(), w.end(), std::back_inserter(words), [](const std::string& s) { return s; });
        std::sort(words.begin(), words.end(), [](const std::string& a, const std::string& b) { return a.size() < b.size(); });

        for(auto& word : words) {
            if(boost::istarts_with(word, term)) return true;
        }
        return false;
    }

    bool canDetect(entt::entity ent, entt::entity target, uint64_t modes) {
        return true;
    }

    // It's up to each game to determine their own scheme for how equipment works.
    // But this API being present in Core helps a lot of other things work nicely.

    std::vector<entt::entity> getInventory(entt::entity ent) {
        auto inv = registry.try_get<Inventory>(ent);
        if(inv) {
            return inv->data;
        }
        // If there's no inventory, we can just return an empty vector.
        return {};
    }

    // It's up to an individual game to determine what counts as equipment, how equipment slots work, etc,
    // but this function should still return a vector of equipment entities.
    std::vector<entt::entity> getEquipment(entt::entity ent) {
        auto eq = registry.try_get<Equipment>(ent);
        std::vector<entt::entity> out;
        if(eq) {
            // copy eq->data values to out and return it.
            for(auto &[slot, e] : eq->data) out.push_back(e);
        }
        return out;
    }

    std::optional<std::vector<double>> parseCoordinates(const std::string& str) {
        std::vector<double> result;
        std::istringstream ss(str);
        std::string token;

        while (std::getline(ss, token, ',')) {
            try {
                double d = std::stod(token);
                result.push_back(d);
            } catch (const std::exception& e) {
                // Failed to parse token as double
                return std::nullopt;
            }
        }

        return result;
    }

    std::optional<Location> validDestination(entt::entity ent, const std::string& str) {
        if(!registry.valid(ent)) return std::nullopt;

        std::vector<double> coordinates;
        if(str.empty()) {
            coordinates = {0.0, 0.0, 0.0};
        } else {
            auto parsed = parseCoordinates(str);
            if(!parsed) return std::nullopt;
            coordinates = *parsed;
        }
        // if coordinates has less than 3 elements, fill remaining with 0.0.
        coordinates.resize(3, 0.0);
        Location dest;
        dest.data = ent;

        if(auto area = registry.try_get<Area>(ent)) {
            // check if coordinates[0] is a RoomId in area->data
            // but first we must turn it into a RoomId from a double.
            RoomId id = coordinates[0];
            if(area->data.contains(id)) {
                dest.x = id;
                dest.locationType == LocationType::Area;
                return dest;
            } else {
                return std::nullopt;
            }
        }
        else if(auto expanse = registry.try_get<Expanse>(ent)) {
            GridLength x = coordinates[0];
            GridLength y = coordinates[1];
            GridLength z = coordinates[2];
            // check if x y z is within the expanse minX/Y/Z and maxX/Y/Z
            if(x >= expanse->minX && x <= expanse->maxX
                && y >= expanse->minY && y <= expanse->maxY
                && z >= expanse->minZ && z <= expanse->maxZ) {
                dest.x = x;
                dest.y = y;
                dest.z = z;
                dest.locationType = LocationType::Expanse;
                return dest;
            } else {
                return std::nullopt;
            }

        }
        else if(auto map = registry.try_get<Map>(ent)) {
            GridLength x = coordinates[0];
            GridLength y = coordinates[1];
            GridLength z = coordinates[2];
            // check if x y z is within the map minX/Y/Z and maxX/Y/Z
            if(x >= map->minX && x <= map->maxX
                && y >= map->minY && y <= map->maxY
                && z >= map->minZ && z <= map->maxZ) {
                dest.x = x;
                dest.y = y;
                dest.z = z;
                dest.locationType = LocationType::Map;
                return dest;
            } else {
                return std::nullopt;
            }
        }
        else if(auto space = registry.try_get<Space>(ent)) {
            SectorLength x = coordinates[0];
            SectorLength y = coordinates[1];
            SectorLength z = coordinates[2];
            // check if x y z is within the space minX/Y/Z and maxX/Y/Z
            if(x >= space->minX && x <= space->maxX
                && y >= space->minY && y <= space->maxY
                && z >= space->minZ && z <= space->maxZ) {
                dest.x = x;
                dest.y = y;
                dest.z = z;
                dest.locationType = LocationType::Space;
                return dest;
            } else {
                return std::nullopt;
            }
        }
        return std::nullopt;

    }

    std::vector<entt::entity> getRoomContents(entt::entity ent) {
        auto rcon = registry.try_get<RoomContents>(ent);
        if(!rcon) return {};
        return rcon->data;
    }

    void sendText(entt::entity ent, const std::string& txt) {
        auto sess = registry.try_get<SessionHolder>(ent);
        if(!sess) return;
        sess->data->sendText(txt);
    }

    void sendLine(entt::entity ent, const std::string& txt) {
        auto sess = registry.try_get<SessionHolder>(ent);
        if(!sess) return;
        sess->data->sendLine(txt);
    }

    void handleBadMatch(entt::entity ent, const std::string& txt, std::unordered_map<std::string, std::string>& matches) {
        sendLine(ent, "Sorry, that's not a command.");
    }

    bool executeCommand(entt::entity ent, const std::string& txt) {
        auto match_map = parseCommand(txt);
        if(match_map.empty()) {
            //handleBadMatch(txt, match_map);
            return false;
        }
        auto ckey = match_map["cmd"];
        auto args = match_map["args"];

        // first we'll check if there any script command handlers...
        if(triggerCommand(ent, ckey, args)) {
            return true;
        }

        auto commands = getSortedCommands(ent);
        for(auto &[key, cmd] : commands) {
            if(!cmd->isAvailable(ent))
                continue;
            if(boost::iequals(ckey, key)) {
                auto [can, err] = cmd->canExecute(ent, match_map);
                if(!can) {
                    sendLine(ent, fmt::format("Sorry, you can't do that: {}", err.value()));
                    return false;
                }
                cmd->execute(ent, match_map);
                return true;
            }
        }
        handleBadMatch(ent, txt, match_map);
        return false;
    }

    std::vector<entt::entity> getAncestors(entt::entity ent) {
        std::vector<entt::entity> chain, out;
        // We're going to walk up the tree, starting with ent, and add each to chain.
        // At the end, reverse insert chain into out and return out.

        auto parent = ent;
        // Then, while ent has a parent, add it to chain and set ent to its parent.
        while(registry.valid(parent)) {
            chain.push_back(parent);
            if(auto pcon = registry.try_get<Parent>(parent); pcon)
                parent = pcon->data;
            else
                break;
        }
        // Now, reverse insert chain into out.
        std::reverse_copy(chain.begin(), chain.end(), std::back_inserter(out));
        return out;
    }

    static std::map<int64_t, std::string> zenniNames = {
            {0,     "an imaginary zenni"},
            {1,     "a single zenni"},
            {10,    "a tiny pile of zenni"},
            {20,    "a handful of zenni"},
            {75,    "a little pile of zenni"},
            {150,   "a small pile of zenni"},
            {250,   "a pile of zenni"},
            {500,   "a big pile of zenni"},
            {1000,  "a large heap of zenni"},
            {5000,  "a huge mound of zenni"},
            {10000, "an enormous mound of zenni"},
            {15000, "a small mountain of zenni"},
            {20000, "a mountain of zenni"},
            {25000, "a huge mountain of zenni"},
            {50000, "an enormous mountain of zenni"}
    };

    entt::entity createMoneyPile(int64_t amount) {
        auto ent = createObject();
        auto &p = registry.get_or_emplace<MoneyPile>(ent);
        p.data = amount;

        registry.emplace<Item>(ent);
        // We want to set the name based on the amount of zenni, referencing zenniNames.
        // We must iterate through it in reverse and pick the first where amount >= .first
        if(amount == 1) {
            setName(ent, "a single zenni");
            setRoomDescription(ent, "One miserable zenni is lying here.");
            setLookDescription(ent, "One lonely coin with a Z on it.");
        } else {
            setLookDescription(ent, "A stack of gold coins with a Z logo.");
            auto it = std::find_if(zenniNames.rbegin(), zenniNames.rend(), [amount](auto& p) {
                return amount >= p.first;
            });
            if(it != zenniNames.rend()) {
                setName(ent, it->second);
            } else {
                setName(ent, "a pile of zenni");
            }
            setRoomDescription(ent, fmt::format("{} is lying here.", grammar::capitalize(getName(ent))));
        }


        auto &key = registry.get_or_emplace<Keywords>(ent);
        key.data = intern(std::string("zenni money coin coins zenny"));
        auto &mat = registry.get_or_emplace<Material>(ent);
        mat.data = mat::CURRENCY;

        auto &ex = registry.get_or_emplace<ExtraDescriptions>(ent);
        auto &e = ex.data.emplace_back();
        e.first = intern(std::string("zenni money coin coins zenny"));
        std::string exDesc = "There is a LOT of zenni.";
        if(amount < 10) exDesc = fmt::format("There is {} zenni.", amount);
        else if(amount < 100) exDesc = fmt::format("There is about {} zenni.", 10 * (amount / 10));
        else if(amount < 1000) exDesc = fmt::format("It looks to be about {} zenni.", 100 * (amount / 100));
        else if(amount < 100000) exDesc = fmt::format("You guess there is, maybe, {} zenni.", 1000 * ((amount / 1000) + randomNumber<int64_t>(0, (amount / 1000))));
        e.second = intern(exDesc);

        return ent;
    }

    void deleteObject(entt::entity ent) {
        atDeleteObject(ent);
        registry.destroy(ent);
    }

    void adminAlert(const std::string& txt, int64_t superVisorLevel) {
        auto formatted = fmt::format("@g[ADMIN: {}@n@g]@n\n", txt);
        for(auto &[id, conn] : connections) {
            if(conn->getAdminLevel() >= superVisorLevel) {
                conn->sendText(formatted);
            }
        }
    }

}