#include "dbatk/operations/movement.h"
#include "dbatk/api.h"
#include "dbatk/components.h"

namespace dbat::op {
    OpResult<> travelToDestination(entt::entity ent, const Location& dest) {
        MoveParams params;
        params.dest = dest;
        params.moveType = MoveType::Traverse;
        params.traverseType = TraverseType::Physical;
        params.mover = ent;
        return moveTo(ent, params);
    }

    OpResult<> travelInDirection(entt::entity ent, const std::string& dir) {
        auto dirOpt = dir::parseDirection(dir);
        if(!dirOpt) {
            return {false, "That is not a valid direction."};
        }
        return travelInDirection(ent, dirOpt.value());
    }

    OpResult<> travelInDirectionArea(entt::entity ent, dir::DirectionId dir) {
        auto loc = registry.try_get<Location>(ent);
        auto id = loc->x;
        auto &area = registry.get<Area>(loc->data);
        auto room = area.data.find(id);
        if(room == area.data.end()) {
            return {false, "You are somewhere beyond space and time... ordinary movement will not avail you."};
        }
        auto exits = registry.try_get<Exits>(room->second);
        if(!exits) {
            return {false, "You can't go that way."};
        }
        auto exit = exits->data.find(dir);
        if(exit == exits->data.end()) {
            return {false, "You can't go that way."};
        }
        auto doors = registry.try_get<Doors>(room->second);
        if(doors) {
            // We'll run some logic for this if doors exist...
            auto door = doors->data.find(dir);
            if(door != doors->data.end()) {
                // TODO: Door data exists and we should check its state...
            }
        }
        Location dest = exit->second;
        if(dest.data == entt::null) {
            dest.data = loc->data;
        }
        return travelToDestination(ent, dest);
    }

    OpResult<> travelInDirectionExpanse(entt::entity ent, dir::DirectionId dir) {
        auto loc = registry.try_get<Location>(ent);
        auto &exp = registry.get<Expanse>(loc->data);
        GridPoint gp(loc->x, loc->y, loc->z);

        // If there is a POI present, check its exits before we try normal movement...
        auto poi = exp.poi.find(gp);
        if(poi != exp.poi.end()) {
            auto exits = registry.try_get<Exits>(poi->second);
            if(exits) {
                auto exit = exits->data.find(dir);
                if(exit != exits->data.end()) {
                    Location dest = exit->second;
                    if(dest.data == entt::null) {
                        dest.data = loc->data;
                    }
                    return travelToDestination(ent, dest);
                }
            }
        }
        // Either there was no POI or no matching exit... use normal directional movement.


        // Update gp based on dir chosen... like north is +1 to x, south is -1 to x, etc.
        // We also have to account for nw, sw, se, and ne... and up and down.
        switch(dir) {
            case dir::North:
                gp.y++;
                break;
            case dir::South:
                gp.y--;
                break;
            case dir::East:
                gp.x++;
                break;
            case dir::West:
                gp.x--;
                break;
            case dir::Northwest:
                gp.x--;
                gp.y++;
                break;
            case dir::Southwest:
                gp.x--;
                gp.y--;
                break;
            case dir::Southeast:
                gp.x++;
                gp.y--;
                break;
            case dir::Northeast:
                gp.x++;
                gp.y++;
                break;
            case dir::Up:
                gp.z++;
                break;
            case dir::Down:
                gp.z--;
                break;
            default:
                return {false, "You can't go that way."};
        }
        // We must check if GP is within the min/max X/Y/Z fields on exp...
        if(gp.x < exp.minX || gp.x > exp.maxX || gp.y < exp.minY || gp.y > exp.maxY || gp.z < exp.minZ || gp.z > exp.maxZ) {
            return {false, "You can't go that way."};
        }
        // Good to go, let's move!
        Location dest;
        dest.data = loc->data;
        dest.x = gp.x;
        dest.y = gp.y;
        dest.z = gp.z;
        dest.locationType = LocationType::Expanse;
        return travelToDestination(ent, dest);
    }

    OpResult<> travelInDirectionMap(entt::entity ent, dir::DirectionId dir) {
        return {false, "Not implemented yet."};
    }

    OpResult<> travelInDirectionSpace(entt::entity ent, dir::DirectionId dir) {
        return {false, "Not implemented yet."};
    }

    OpResult<> travelInDirection(entt::entity ent, dir::DirectionId dir) {
        auto loc = registry.try_get<Location>(ent);
        if(!loc) {
            return {false, "You are somewhere beyond space and time... ordinary movement will not avail you."};
        }

        // We need to figure out if we're in an Area, Expanse, Map, or Space...
        if(loc->locationType == LocationType::Area) {
            return travelInDirectionArea(ent, dir);
        } else if(loc->locationType == LocationType::Expanse) {
            return travelInDirectionExpanse(ent, dir);
        } else if(loc->locationType == LocationType::Map) {
            return travelInDirectionMap(ent, dir);
        } else if(loc->locationType == LocationType::Space) {
            return travelInDirectionSpace(ent, dir);
        } else {
            return {false, "You are somewhere beyond space and time... ordinary movement will not avail you."};
        }
    }
}