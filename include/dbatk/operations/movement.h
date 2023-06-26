#pragma once
#include "dbatk/base.h"
#include "dbatk/components.h"
#include "dbatk/aspects/direction.h"

namespace dbat::op {
    OpResult<> travelToDestination(entt::entity ent, const Destination& dest);
    OpResult<> travelInDirection(entt::entity ent, dir::DirectionId dir);
    OpResult<> travelInDirectionArea(entt::entity ent, dir::DirectionId dir);
    OpResult<> travelInDirectionExpanse(entt::entity ent, dir::DirectionId dir);
    OpResult<> travelInDirectionMap(entt::entity ent, dir::DirectionId dir);
    OpResult<> travelInDirectionSpace(entt::entity ent, dir::DirectionId dir);
    OpResult<> travelInDirection(entt::entity ent, const std::string& dir);
}