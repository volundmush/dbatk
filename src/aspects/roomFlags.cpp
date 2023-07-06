#include "dbatk/aspects/roomFlags.h"

namespace dbat::rflags {

    class SimpleRoomFlag : public RoomFlag {
    public:
        SimpleRoomFlag(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    static std::shared_ptr<RoomFlag> makeRoomFlag(RFlagId flag, const std::string& name) {
        return std::make_shared<SimpleRoomFlag>(flag, name);
    }

    // So now that we have the enum RFlag and the class RoomFlag, we will create a vector
    // where the vector index matches the RFlag enum value for O(1) lookups.
    // For starters, every flag will just be a SimpleRoomFlag.

    const std::vector<std::shared_ptr<RoomFlag>> roomFlags = {
            makeRoomFlag(RFlagId::DARK, "DARK"),
            makeRoomFlag(RFlagId::DEATH, "DEATH"),
            makeRoomFlag(RFlagId::NOMOB, "NOMOB"),
            makeRoomFlag(RFlagId::INDOORS, "INDOORS"),
            makeRoomFlag(RFlagId::PEACEFUL, "PEACEFUL"),
            makeRoomFlag(RFlagId::SOUNDPROOF, "SOUNDPROOF"),
            makeRoomFlag(RFlagId::NOTRACK, "NOTRACK"),
            makeRoomFlag(RFlagId::NOINSTANT, "NOINSTANT"),
            makeRoomFlag(RFlagId::TUNNEL, "TUNNEL"),
            makeRoomFlag(RFlagId::PRIVATE, "PRIVATE"),
            makeRoomFlag(RFlagId::GODROOM, "GODROOM"),
            makeRoomFlag(RFlagId::HOUSE, "HOUSE"),
            makeRoomFlag(RFlagId::HOUSE_CRASH, "HOUSE_CRASH"),
            makeRoomFlag(RFlagId::ATRIUM, "ATRIUM"),
            makeRoomFlag(RFlagId::OLC, "OLC"),
            makeRoomFlag(RFlagId::BFS_MARK, "BFS_MARK"),
            makeRoomFlag(RFlagId::VEHICLE, "VEHICLE"),
            makeRoomFlag(RFlagId::UNDERGROUND, "UNDERGROUND"),
            makeRoomFlag(RFlagId::CURRENT, "CURRENT"),
            makeRoomFlag(RFlagId::TIMED_DT, "TIMED_DT"),
            makeRoomFlag(RFlagId::EARTH, "EARTH"),
            makeRoomFlag(RFlagId::VEGETA, "VEGETA"),
            makeRoomFlag(RFlagId::FRIGID, "FRIGID"),
            makeRoomFlag(RFlagId::KONACK, "KONACK"),
            makeRoomFlag(RFlagId::NAMEK, "NAMEK"),
            makeRoomFlag(RFlagId::NEO, "NEO"),
            makeRoomFlag(RFlagId::AL, "AL"),
            makeRoomFlag(RFlagId::SPACE, "SPACE"),
            makeRoomFlag(RFlagId::HELL, "HELL"),
            makeRoomFlag(RFlagId::REGEN, "REGEN"),
            makeRoomFlag(RFlagId::RHELL, "RHELL"),
            makeRoomFlag(RFlagId::GRAVITYX10, "GRAVITYX10"),
            makeRoomFlag(RFlagId::AETHER, "AETHER"),
            makeRoomFlag(RFlagId::HBTC, "HBTC"),
            makeRoomFlag(RFlagId::PAST, "PAST"),
            makeRoomFlag(RFlagId::CBANK, "CBANK"),
            makeRoomFlag(RFlagId::SHIP, "SHIP"),
            makeRoomFlag(RFlagId::YARDRAT, "YARDRAT"),
            makeRoomFlag(RFlagId::KANASSA, "KANASSA"),
            makeRoomFlag(RFlagId::ARLIA, "ARLIA"),
            makeRoomFlag(RFlagId::AURA, "AURA"),
            makeRoomFlag(RFlagId::EORBIT, "EORBIT"),
            makeRoomFlag(RFlagId::FORBIT, "FORBIT"),
            makeRoomFlag(RFlagId::KORBIT, "KORBIT"),
            makeRoomFlag(RFlagId::NORBIT, "NORBIT"),
            makeRoomFlag(RFlagId::VORBIT, "VORBIT"),
            makeRoomFlag(RFlagId::AORBIT, "AORBIT"),
            makeRoomFlag(RFlagId::YORBIT, "YORBIT"),
            makeRoomFlag(RFlagId::KANORB, "KANORB"),
            makeRoomFlag(RFlagId::ARLORB, "ARLORB"),
            makeRoomFlag(RFlagId::NEBULA, "NEBULA"),
            makeRoomFlag(RFlagId::ASTERO, "ASTERO"),
            makeRoomFlag(RFlagId::WORMHO, "WORMHO"),
            makeRoomFlag(RFlagId::STATION, "STATION"),
            makeRoomFlag(RFlagId::STAR, "STAR"),
            makeRoomFlag(RFlagId::CERRIA, "CERRIA"),
            makeRoomFlag(RFlagId::CORBIT, "CORBIT"),
            makeRoomFlag(RFlagId::BEDROOM, "BEDROOM"),
            makeRoomFlag(RFlagId::WORKOUT, "WORKOUT"),
            makeRoomFlag(RFlagId::GARDEN1, "GARDEN1"),
            makeRoomFlag(RFlagId::GARDEN2, "GARDEN2"),
            makeRoomFlag(RFlagId::FERTILE1, "FERTILE1"),
            makeRoomFlag(RFlagId::FERTILE2, "FERTILE2"),
            makeRoomFlag(RFlagId::FISHING, "FISHING"),
            makeRoomFlag(RFlagId::FISHFRESH, "FISHFRESH"),
            makeRoomFlag(RFlagId::CANREMODEL, "CANREMODEL"),
            makeRoomFlag(RFlagId::CONTROLS, "CONTROLS"),
            makeRoomFlag(RFlagId::WINDOW, "WINDOW"),
            makeRoomFlag(RFlagId::HATCH, "HATCH"),
            makeRoomFlag(RFlagId::DOCK, "DOCK"),
            makeRoomFlag(RFlagId::LANDING, "LANDING"),
    };

}