#pragma once
#include "dbatk/coordinates.h"

namespace dbat {
    struct Location;

    namespace op {
        struct RenderInfo {
            bool matrix;
            entt::entity looker;
            Location loc;
            std::vector<std::string>& output;
        };

        bool renderAbsoluteLocation(RenderInfo &info);
        bool renderInventoryLocation(RenderInfo &info);
        bool renderEquipmentLocation(RenderInfo &info);
        bool renderAreaLocation(RenderInfo &info);
        bool renderExpanseLocation(RenderInfo &info);
        bool renderMapLocation(RenderInfo &info);
        bool renderSpaceLocation(RenderInfo &info);

        std::string listZones();

        std::string renderLocation(const Location& loc, entt::entity looker);

        std::string renderPrefixData(entt::entity ent);

        std::string renderRoomLine(entt::entity ent, entt::entity looker, bool matrix);
        std::string renderDisplayName(entt::entity ent, entt::entity looker, bool matrix);
        std::string renderItemListing(entt::entity ent, entt::entity looker, bool matrix);

        std::string renderExamine(entt::entity ent, entt::entity looker);
        std::string renderPrefix(entt::entity ent);

        std::string renderWhere(const std::string &name, entt::entity looker);

    }
}