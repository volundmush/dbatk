#pragma once
#include "dbatk/base.h"
#include "dbatk/exit/flags.h"

namespace dbat {


    struct Exit {
        Exit(Room* room, RoomID destination, std::optional<ObjectID> objectID = std::nullopt) :
                room(room), destination(destination), objectID(objectID) {};
        Exit(Room* room, const nlohmann::json& j);
        std::string_view keyword, description;
        // The original code has an int16_t for exit flags, so we'll use a bitset.
        std::bitset<eflags::NUM_EXIT_FLAGS> flags;
        // Exits which have an objectID are pointed at a room inside a different object.
        // If this holds no value, then the destination is 'local' to this object.
        std::optional<ObjectID> objectID;
        RoomID destination;
        // I'm not actually sure what these do yet, just copying them from the original.
        int dclock{0};
        int dchide{0};
        Room* getDestination();
        Room* room;
        void setExitFlag(std::size_t flag, bool value);
        [[nodiscard]] bool checkExitFlag(std::size_t flag) const;
        void deserialize(const nlohmann::json& j);
        nlohmann::json serialize();
    };
}