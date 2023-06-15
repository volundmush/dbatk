#include "dbatk/room/room.h"
#include "dbatk/exit/exit.h"

namespace dbat {

    void Room::setRoomFlag(std::size_t flag, bool value) {
        // Error out if flag > NUM_ROOM_FLAGS
        if(flag > flags.size() - 1) return;
        if(value) {
            if(flags.test(flag)) return;
            flags.set(flag);
            rflags::roomFlags[flag]->onSet(this);
        } else {
            if(!flags.test(flag)) return;
            flags.reset(flag);
            rflags::roomFlags[flag]->onClear(this);
        }
    }

    bool Room::checkRoomFlag(std::size_t flag) const {
        // Error out if flag > NUM_ROOM_FLAGS
        if(flag > flags.size() - 1) return false;
        return flags.test(flag);
    }



    terrain::TerrainID Room::getTerrainID() const {
        return terrainId;
    }

    terrain::Terrain* Room::getTerrain() const {
        return terrain::terrainRegistry[terrainId].get();
    }


    nlohmann::json Room::serialize() {
        auto j = serializeBase();
        if(terrainId != terrain::INSIDE) j["terrainId"] = terrainId;
        for(auto i = 0; i < flags.size(); ++i) {
            if(flags.test(i)) j["flags"].push_back(i);
        }

        for(auto& e : exits) {
            j["exits"].push_back(std::make_pair(e.first, e.second.serialize()));
        }
        return j;
    }

    void Room::deserialize(const nlohmann::json &j) {
        deserializeBase(j);
        if(j.contains("terrainId")) terrainId = j["terrainId"];
        if(j.contains("flags"))
            for(auto i : j["flags"]) {
                setRoomFlag(i, true);
            }
        if(j.contains("exits"))
            for(auto& e : j["exits"]) {
                dir::DirNum d = e[0];
                exits.emplace(d, Exit(this, e[1]));
            }
    }

}