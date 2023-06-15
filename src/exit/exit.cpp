#include "dbatk/exit/exit.h"
#include "dbatk/room/room.h"

namespace dbat {

    Room* Exit::getDestination() {
        if(objectID) {
            auto obj = objectID.value().getObject();
            if(obj) {
                return obj->getRoom(destination);
            }
        } else {
            return room->obj->getRoom(destination);
        }
        return nullptr;
    }

    nlohmann::json Exit::serialize() {
        nlohmann::json j;
        j["destination"] = destination;
        for(auto i = 0; i < flags.size(); ++i) {
            if(flags.test(i)) j["flags"].push_back(i);
        }
        if(objectID) j["objectID"] = objectID.value();
        if(dclock) j["dclock"] = dclock;
        if(dchide) j["dchide"] = dchide;
        return j;
    }

    void Exit::deserialize(const nlohmann::json &j) {
        if(j.contains("destination")) destination = j["destination"];
        if(j.contains("flags"))
            for(const auto& i : j["flags"]) {
                setExitFlag(i, true);
            }
        if(j.contains("objectID")) objectID.emplace(j["objectID"]);
        if(j.contains("dclock")) dclock = j["dclock"];
        if(j.contains("dchide")) dchide = j["dchide"];
    }

    Exit::Exit(Room* room, const nlohmann::json& j) {
        this->room = room;
        deserialize(j);
    }

    void Exit::setExitFlag(std::size_t flag, bool value) {
        // Error out if flag > NUM_EXIT_FLAGS
        if(flag > flags.size() - 1) return;
        if(value) {
            if(flags.test(flag)) return;
            flags.set(flag);
            eflags::exitFlags[flag]->onSet(this);
        } else {
            if(!flags.test(flag)) return;
            flags.reset(flag);
            eflags::exitFlags[flag]->onClear(this);
        }
    }

    bool Exit::checkExitFlag(std::size_t flag) const {
        // Error out if flag > NUM_EXIT_FLAGS
        if(flag > flags.size() - 1) return false;
        return flags.test(flag);
    }

}