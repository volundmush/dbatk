#include "dbatk/room/flags.h"

namespace dbat::rflags {

    static std::shared_ptr<RoomFlag> makeRoomFlag(RFlag flag, const std::string& name) {
        return std::make_shared<SimpleRoomFlag>(flag, name);
    }

    // So now that we have the enum RFlag and the class RoomFlag, we will create a vector
    // where the vector index matches the RFlag enum value for O(1) lookups.
    // For starters, every flag will just be a SimpleRoomFlag.

    const std::vector<std::shared_ptr<RoomFlag>> roomFlags = {
            makeRoomFlag(RFlag::DARK, "DARK"),
            makeRoomFlag(RFlag::DEATH, "DEATH"),
            makeRoomFlag(RFlag::NOMOB, "NOMOB"),
            makeRoomFlag(RFlag::INDOORS, "INDOORS"),
            makeRoomFlag(RFlag::PEACEFUL, "PEACEFUL"),
            makeRoomFlag(RFlag::SOUNDPROOF, "SOUNDPROOF"),
            makeRoomFlag(RFlag::NOTRACK, "NOTRACK"),
            makeRoomFlag(RFlag::NOINSTANT, "NOINSTANT"),
            makeRoomFlag(RFlag::TUNNEL, "TUNNEL"),
            makeRoomFlag(RFlag::PRIVATE, "PRIVATE"),
            makeRoomFlag(RFlag::GODROOM, "GODROOM"),
            makeRoomFlag(RFlag::HOUSE, "HOUSE"),
            makeRoomFlag(RFlag::HOUSE_CRASH, "HOUSE_CRASH"),
            makeRoomFlag(RFlag::ATRIUM, "ATRIUM"),
            makeRoomFlag(RFlag::OLC, "OLC"),
            makeRoomFlag(RFlag::BFS_MARK, "BFS_MARK"),
            makeRoomFlag(RFlag::VEHICLE, "VEHICLE"),
            makeRoomFlag(RFlag::UNDERGROUND, "UNDERGROUND"),
            makeRoomFlag(RFlag::CURRENT, "CURRENT"),
            makeRoomFlag(RFlag::TIMED_DT, "TIMED_DT"),
            makeRoomFlag(RFlag::EARTH, "EARTH"),
            makeRoomFlag(RFlag::VEGETA, "VEGETA"),
            makeRoomFlag(RFlag::FRIGID, "FRIGID"),
            makeRoomFlag(RFlag::KONACK, "KONACK"),
            makeRoomFlag(RFlag::NAMEK, "NAMEK"),
            makeRoomFlag(RFlag::NEO, "NEO"),
            makeRoomFlag(RFlag::AL, "AL"),
            makeRoomFlag(RFlag::SPACE, "SPACE"),
            makeRoomFlag(RFlag::HELL, "HELL"),
            makeRoomFlag(RFlag::REGEN, "REGEN"),
            makeRoomFlag(RFlag::RHELL, "RHELL"),
            makeRoomFlag(RFlag::GRAVITYX10, "GRAVITYX10"),
            makeRoomFlag(RFlag::AETHER, "AETHER"),
            makeRoomFlag(RFlag::HBTC, "HBTC"),
            makeRoomFlag(RFlag::PAST, "PAST"),
            makeRoomFlag(RFlag::CBANK, "CBANK"),
            makeRoomFlag(RFlag::SHIP, "SHIP"),
            makeRoomFlag(RFlag::YARDRAT, "YARDRAT"),
            makeRoomFlag(RFlag::KANASSA, "KANASSA"),
            makeRoomFlag(RFlag::ARLIA, "ARLIA"),
            makeRoomFlag(RFlag::AURA, "AURA"),
            makeRoomFlag(RFlag::EORBIT, "EORBIT"),
            makeRoomFlag(RFlag::FORBIT, "FORBIT"),
            makeRoomFlag(RFlag::KORBIT, "KORBIT"),
            makeRoomFlag(RFlag::NORBIT, "NORBIT"),
            makeRoomFlag(RFlag::VORBIT, "VORBIT"),
            makeRoomFlag(RFlag::AORBIT, "AORBIT"),
            makeRoomFlag(RFlag::YORBIT, "YORBIT"),
            makeRoomFlag(RFlag::KANORB, "KANORB"),
            makeRoomFlag(RFlag::ARLORB, "ARLORB"),
            makeRoomFlag(RFlag::NEBULA, "NEBULA"),
            makeRoomFlag(RFlag::ASTERO, "ASTERO"),
            makeRoomFlag(RFlag::WORMHO, "WORMHO"),
            makeRoomFlag(RFlag::STATION, "STATION"),
            makeRoomFlag(RFlag::STAR, "STAR"),
            makeRoomFlag(RFlag::CERRIA, "CERRIA"),
            makeRoomFlag(RFlag::CORBIT, "CORBIT"),
            makeRoomFlag(RFlag::BEDROOM, "BEDROOM"),
            makeRoomFlag(RFlag::WORKOUT, "WORKOUT"),
            makeRoomFlag(RFlag::GARDEN1, "GARDEN1"),
            makeRoomFlag(RFlag::GARDEN2, "GARDEN2"),
            makeRoomFlag(RFlag::FERTILE1, "FERTILE1"),
            makeRoomFlag(RFlag::FERTILE2, "FERTILE2"),
            makeRoomFlag(RFlag::FISHING, "FISHING"),
            makeRoomFlag(RFlag::FISHFRESH, "FISHFRESH"),
            makeRoomFlag(RFlag::CANREMODEL, "CANREMODEL"),
            makeRoomFlag(RFlag::CONTROLS, "CONTROLS"),
            makeRoomFlag(RFlag::WINDOW, "WINDOW"),
            makeRoomFlag(RFlag::HATCH, "HATCH"),
            makeRoomFlag(RFlag::DOCK, "DOCK"),
            makeRoomFlag(RFlag::LANDING, "LANDING"),
    };

}