#include "kaizermud/Api.h"

namespace dbat {

    std::string roomRenderAppearance(entt::entity ent, entt::entity looker);
    std::string roomGetDisplayName(entt::entity ent, entt::entity looker);

    void registerDBAPI();
}