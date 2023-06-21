#pragma once

#include "dbatk/base.h"


namespace dbat {

    ObjectId getObjectId(entt::entity ent);

    OpResult<> setParent(entt::entity ent, entt::entity target);
    entt::entity getParent(entt::entity ent);
    void addToChildren(entt::entity ent, entt::entity target);
    void removeFromChildren(entt::entity ent, entt::entity target);
    void atChildDeleted(entt::entity ent, entt::entity target);
    void atParentDeleted(entt::entity ent, entt::entity target);
    std::vector<entt::entity> getChildren(entt::entity ent);

    OpResult<> setOwner(entt::entity ent, entt::entity target);
    entt::entity getOwner(entt::entity ent);
    void addToAssets(entt::entity ent, entt::entity target);
    void removeFromAssets(entt::entity ent, entt::entity target);
    void atAssetDeleted(entt::entity ent, entt::entity target);
    void atOwnerDeleted(entt::entity ent, entt::entity target);
    std::vector<entt::entity> getAssets(entt::entity ent);

    OpResult<> setLocation(entt::entity ent, entt::entity target);
    entt::entity getLocation(entt::entity ent);
    void addToContents(entt::entity ent, entt::entity target);
    void removeFromContents(entt::entity ent, entt::entity target);
    void atContentDeleted(entt::entity ent, entt::entity target);
    void atLocationDeleted(entt::entity ent, entt::entity target);
    std::vector<entt::entity> getContents(entt::entity ent);

    template<typename T>
    void setBaseText(entt::entity ent, const std::string& txt) {
        auto &comp = registry.get_or_emplace<T>(ent);
        comp.setData(txt);
    }

    template<typename T>
    std::string_view getBaseText(entt::entity ent) {
        auto comp = registry.try_get<T>(ent);
        if(comp) return comp->data;
        return "";
    }

    std::string getName(entt::entity ent);
    void setName(entt::entity ent, const std::string& txt);

    std::string getShortDescription(entt::entity ent);
    void setShortDescription(entt::entity ent, const std::string& txt);

    std::string getRoomDescription(entt::entity ent);
    void setRoomDescription(entt::entity ent, const std::string& txt);

    std::string getLookDescription(entt::entity ent);
    void setLookDescription(entt::entity ent, const std::string& txt);

    std::string getDisplayName(entt::entity ent, entt::entity looker);
    std::string getRoomLine(entt::entity ent, entt::entity looker);
    std::string renderAppearance(entt::entity ent, entt::entity looker);

    void atDeleteObject(entt::entity ent);

}