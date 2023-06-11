#pragma once
#include "dbatk/object/object.h"
#include "dbatk/room/flags.h"
#include "dbatk/room/terrain.h"
#include "dbatk/exit/direction.h"
#include "dbatk/exit/exit.h"

namespace dbat {

    class Room : public Unit {
        friend class Exit;
    public:
        Room(RoomID id, Object* obj, const nlohmann::json& j) : id(id), obj(obj) {deserialize(j);}
        Room(RoomID id, Object* obj) : id(id), obj(obj) {};
        Room(RoomID id, const std::shared_ptr<Object>& obj) : Room(id, obj.get()) {};
        Room(RoomID id, const std::shared_ptr<Object>& obj, const nlohmann::json& j) : Room(id, obj.get(), j) {};
        void setRoomFlag(std::size_t flag, bool value);
        [[nodiscard]] bool checkRoomFlag(std::size_t flag) const;
        terrain::Terrain* getTerrain() const;
        terrain::TerrainID getTerrainID() const;
        void deserialize(const nlohmann::json& j);
        nlohmann::json serialize();
    protected:
        RoomID id{};
        // There's no reason for a Room to hang on to a shared pointer to its object
        // because Rooms are always deleted when their holding Object is.
        Object* obj{};
        std::bitset<rflags::NUM_ROOM_FLAGS> flags{};
        std::unordered_map<dir::DirNum, Exit> exits{};
        terrain::TerrainID terrainId{terrain::INSIDE};
    };

}