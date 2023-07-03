#pragma once

#include "dbatk/base.h"
#include "dbatk/components.h"


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

    enum class MoveType {
        // Used for moving between rooms or coordinates via normal travel.
        Traverse = 0,
        // Retrieving something from a room or container.
        Get = 1,
        // Giving an item to another character.
        Give = 2,
        // Dropping an item into a room.
        Drop = 3,
        // Putting an item into a container.
        Put = 4,
        // Equipping and removing items.
        Equip = 5,
        Remove = 6

    };

    enum class TraverseType {
        Physical = 0,
        Teleport = 1,
        System = 2
    };

    struct MoveParams {
        bool force{false};
        bool quiet{false};
        std::optional<Location> previousLocation;
        MoveType moveType{MoveType::Traverse};
        TraverseType traverseType{TraverseType::Physical};
        std::optional<Location> dest;
        // When exits or directions are being used, direction tells which way the character is going.
        // If they used a north exit, then dir is North.
        std::optional<dir::DirectionId> dir;
        entt::entity mover{entt::null};
    };


    OpResult<> moveTo(entt::entity ent, MoveParams& params);
    OpResult<> canUnequip(entt::entity ent, entt::entity item, MoveParams& params);
    OpResult<> canRemoveFromInventory(entt::entity ent, entt::entity item, MoveParams& params);
    OpResult<> canLeaveRoom(entt::entity ent, entt::entity room, MoveParams& params);
    OpResult<> canLeavePOI(entt::entity ent, entt::entity poi, MoveParams& params);

    OpResult<> canEquip(entt::entity ent, entt::entity item, MoveParams& params);
    OpResult<> canAddToInventory(entt::entity ent, entt::entity item, MoveParams& params);
    OpResult<> canEnterRoom(entt::entity ent, entt::entity room, MoveParams& params);
    OpResult<> canEnterPOI(entt::entity ent, entt::entity poi, MoveParams& params);

    void unequip(entt::entity ent, entt::entity item, MoveParams& params);
    void removeFromInventory(entt::entity ent, entt::entity item, MoveParams& params);
    void leaveRoom(entt::entity ent, entt::entity room, MoveParams& params);
    void leavePOI(entt::entity ent, entt::entity poi, MoveParams& params);

    void equip(entt::entity ent, entt::entity item, MoveParams& params);
    void addToInventory(entt::entity ent, entt::entity item, MoveParams& params);
    void enterRoom(entt::entity ent, entt::entity room, MoveParams& params);
    void enterPOI(entt::entity ent, entt::entity poi, MoveParams& params);

    OpResult<> atPreObjectReceive(entt::entity ent, entt::entity target, MoveParams& params);
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

    std::string getDisplayName(entt::entity ent, entt::entity looker, bool matrix=false);
    std::string getRoomLine(entt::entity ent, entt::entity looker, bool matrix=false);
    std::string renderAppearance(entt::entity ent, entt::entity looker);

    void deleteObject(entt::entity ent);
    void atDeleteObject(entt::entity ent);

    std::set<std::string> getSearchWords(entt::entity ent, entt::entity looker);

    bool checkSearch(entt::entity ent, std::string_view term, entt::entity looker);

    bool canDetect(entt::entity ent, entt::entity target, uint64_t modes);

    std::vector<entt::entity> getInventory(entt::entity ent);

    std::vector<entt::entity> getEquipment(entt::entity ent);

    std::optional<std::vector<double>> parseCoordinates(const std::string& str);

    std::optional<Location> validDestination(entt::entity ent, const std::string& str);

    std::vector<entt::entity> getRoomContents(entt::entity ent);

    void sendText(entt::entity ent, const std::string& txt);
    void sendLine(entt::entity ent, const std::string& txt);

    void handleBadMatch(entt::entity ent, const std::string& txt, std::unordered_map<std::string, std::string>& matches);
    bool executeCommand(entt::entity ent, const std::string& cmd);

    std::vector<entt::entity> getAncestors(entt::entity ent);

    entt::entity createMoneyPile(int64_t amount);

    void adminAlert(const std::string& txt, int64_t superVisorLevel = 1);

}