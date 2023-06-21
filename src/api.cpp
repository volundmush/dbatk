#include "dbatk/api.h"
#include "dbatk/components.h"
#include "dbatk/color.h"

namespace dbat {

    void setName(entt::entity ent, const std::string &txt) {
        setBaseText<Name>(ent, txt);
    }

    void setShortDescription(entt::entity ent, const std::string &txt) {
        setBaseText<ShortDescription>(ent, txt);
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

    std::string getShortDescription(entt::entity ent) {
        return std::string(getBaseText<ShortDescription>(ent));
    }

    std::string getRoomDescription(entt::entity ent) {
        return std::string(getBaseText<RoomDescription>(ent));
    }

    std::string getLookDescription(entt::entity ent) {
        return std::string(getBaseText<LookDescription>(ent));
    }

    std::string getDisplayName(entt::entity ent, entt::entity looker) {
        if(registry.any_of<Item, NPC>(ent)) {
            return getShortDescription(ent);
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

    std::string getRoomLine(entt::entity ent, entt::entity looker) {
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

    std::string renderAppearance(entt::entity ent, entt::entity looker) {
        return "";
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

    OpResult<> setLocation(entt::entity ent, entt::entity target) {
        if(registry.valid(target)) {
            // We must look up the parent chain to ensure no shenanigans.
            auto parent = target;
            while(registry.valid(parent)) {
                if(parent == ent) {
                    return {false, "That would cause a recursive relationship... very bad."};
                }
                parent = getLocation(parent);
            }
        }

        entt::entity oldLocation = getLocation(ent);
        if(registry.valid(oldLocation)) {
            removeFromContents(oldLocation, ent);
        }

        if(registry.valid(target)) {
            addToContents(target, ent);
            auto &par = registry.get_or_emplace<Location>(ent);
            par.data = target;
        } else {
            registry.remove<Location>(ent);
        }
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
        setLocation(ent, entt::null);
        registry.erase<GridLocation>(ent);
        registry.erase<RoomLocation>(ent);
        registry.erase<SectorLocation>(ent);
        // TODO: punt people in a deleted zone somewhere safe.
        // They SHOULD be evacuated first, by a higher-level
        // routine, but we need a failsafe for any stragglers.
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

}