#include "dbatk/api.h"
#include "dbatk/components.h"
#include "dbatk/color.h"
#include "dbatk/session.h"
#include "dbatk/operations/movement.h"
#include "dbatk/operations/information.h"
#include "dbatk/commands.h"
#include "dbatk/triggers.h"

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
        registry.erase<Location>(item);
    }

    void leaveRoom(entt::entity ent, entt::entity room, MoveParams& params) {
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

    }

    void enterRoom(entt::entity ent, entt::entity room, MoveParams& params) {
        auto &loc = registry.get_or_emplace<Location>(ent);
        auto &d = params.dest.value();
        loc.data = d.data;
        loc.x = d.x;
        loc.locationType = d.locationType;
        if(!params.quiet) {
            // TODO: announce movement here...
        }
        auto &rc = registry.get_or_emplace<RoomContents>(room);
        rc.data.push_back(ent);

        // greet triggers.
        if(params.traverseType != TraverseType::System && !mobTriggerGreet(ent, params.dir)) {
            // If it returned false, we must kick the character back to previous location if possible.
            if(params.previousLocation) {
                MoveParams mp = params;
                mp.previousLocation = loc;
                mp.dest = params.previousLocation;
                mp.dir.reset(); // maybe need to reverse directions here...
                // should probably set some kind of safeguard on MoveParams to prevent infinite ping-ponging.
                moveTo(ent, mp);
            }
        } else {
            // Rendering a room appearance is very expensive, so we'll only do it if we know there'll be someone listening.
            if(registry.any_of<SessionHolder>(ent)) {
                sendLine(ent, op::renderLocation(loc, ent));
            }
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

    void executeCommand(entt::entity ent, const std::string& txt) {
        auto match_map = parseCommand(txt);
        if(match_map.empty()) {
            //handleBadMatch(txt, match_map);
            return;
        }
        auto ckey = match_map["cmd"];
        auto commands = getSortedCommands(ent);
        for(auto &[key, cmd] : commands) {
            if(!cmd->isAvailable(ent))
                continue;
            if(boost::iequals(ckey, key)) {
                auto [can, err] = cmd->canExecute(ent, match_map);
                if(!can) {
                    sendLine(ent, fmt::format("Sorry, you can't do that: {}", err.value()));
                    return;
                }
                cmd->execute(ent, match_map);
                return;
            }
        }
        handleBadMatch(ent, txt, match_map);
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

}