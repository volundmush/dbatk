#include "dbatk/commands/object.h"
#include "dbatk/api.h"
#include "dbatk/components.h"
#include "dbatk/aspects/direction.h"
#include "dbatk/operations/movement.h"
#include "dbatk/operations/information.h"
#include "dbatk/message.h"

namespace dbat::cmd {
    void ObjLook::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto loc = registry.try_get<Location>(ent);
        if(!loc) {
            sendLine(ent, "You are nowhere.");
            return;
        }

        if(loc->locationType == LocationType::Area) {
            auto &area = registry.get<Area>(loc->data);
            auto &room = area.data[loc->x];
            sendLine(ent, op::renderRoomAppearance(room, ent));
            return;
        }
    }

    void ObjHelp::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

    }



    OpResult<> ObjMove::canExecute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        // This command requires that ent be in a location.
        auto loc = registry.try_get<Location>(ent);
        if(!loc) {
            return {false, "You are somewhere beyond space and time... ordinary movement will not avail you."};
        }
        return {true, std::nullopt};
    }

    void ObjMove::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        // Next we must determine the direction that the player wants to move.
        // If this command was called as move, go, or mv, we need to retrieve args
        // from input. Otherwise, input["cmd"] itself is the direction.

        std::string direction;
        auto cmd = input["cmd"];
        if (boost::iequals(cmd, "move") || boost::iequals(cmd, "go") || boost::iequals(cmd, "mv")) {
            direction = input["args"];
            if(direction.empty()) {
                sendLine(ent, "Usage: move <direction>");
                return;
            }
        } else {
            direction = cmd;
        }

        auto [res, err] = op::travelInDirection(ent, direction);
        if(!res) {
            sendLine(ent, err.value());
            return;
        }
    }


    void ObjQuit::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

    }

    void ObjSay::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto loc = registry.try_get<Location>(ent);
        if(!loc) {
            sendLine(ent, "You are nowhere. There is nobody to hear you.");
            return;
        }

        auto args = input["args"];
        if(args.empty()) sendLine(ent, "Say what?");

        MsgFormat m(ent, fmt::format("$You() @W$conj(say), '@n@C{}@W'", args));
        if(loc->locationType == LocationType::Area) {
            auto &area = registry.get<Area>(loc->data);
            auto &room = area.data[loc->x];
            m.room(room);
        } else {
            // todo: handle these cases...
        }
        m.send();

    }

    void ObjPose::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto loc = registry.try_get<Location>(ent);
        if(!loc) {
            sendLine(ent, "You are nowhere. There is nobody to notice you.");
            return;
        }

        auto args = input["args"];
        if(args.empty()) sendLine(ent, "Pose what?");

        MsgFormat m(ent, fmt::format("$You() {}", args));
        if(loc->locationType == LocationType::Area) {
            auto &area = registry.get<Area>(loc->data);
            auto &room = area.data[loc->x];
            m.room(room);
        } else {
            // todo: handle these cases...
        }
        m.send();
    }

    void ObjEmote::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto loc = registry.try_get<Location>(ent);
        if(!loc) {
            sendLine(ent, "You are nowhere. There is nobody to notice you.");
            return;
        }

        auto args = input["args"];
        if(args.empty()) sendLine(ent, "Emote what?");
        // We need to perform some substitution on the string:
        // &1 becomes: '@C
        // &2 becomes: @N'

        boost::replace_all(args, "&1", "'@C");
        boost::replace_all(args, "&2", "@n'");

        // TODO: Handle the *target stuff for roleplay later.

        MsgFormat m(ent, fmt::format("$You() {}", args));
        if(loc->locationType == LocationType::Area) {
            auto &area = registry.get<Area>(loc->data);
            auto &room = area.data[loc->x];
            m.room(room);
        } else {
            // todo: handle these cases...
        }
        m.send();
    }

    void ObjSemipose::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto loc = registry.try_get<Location>(ent);
        if(!loc) {
            sendLine(ent, "You are nowhere. There is nobody to notice you.");
            return;
        }

        auto args = input["args"];
        if(args.empty()) sendLine(ent, "Semipose what?");

        MsgFormat m(ent, fmt::format("$You(){}", args));
        if(loc->locationType == LocationType::Area) {
            auto &area = registry.get<Area>(loc->data);
            auto &room = area.data[loc->x];
            m.room(room);
        } else {
            // todo: handle these cases...
        }
        m.send();
    }

    void ObjWhisper::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

    }

    void ObjShout::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto loc = registry.try_get<Location>(ent);
        if(!loc) {
            sendLine(ent, "You are nowhere. There is nobody to hear you.");
            return;
        }

        auto args = input["args"];
        if(args.empty()) sendLine(ent, "Say what?");

        MsgFormat m(ent, fmt::format("@y$You() $conj(shout), \"{}\"@n", args));
        m.region(loc->data);
        m.send();
    }

    void ObjGet::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

    }

    void ObjTake::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

    }

    void ObjPut::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

    }

    void ObjGive::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

    }

    void ObjDrop::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

    }

    void ObjInventory::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

    }

    void ObjEquip::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

    }

    void registerObjectCommands() {
        std::vector<std::shared_ptr<Command>> commands;
        commands.push_back(std::make_shared<ObjLook>());
        commands.push_back(std::make_shared<ObjHelp>());
        commands.push_back(std::make_shared<ObjMove>());
        commands.push_back(std::make_shared<ObjQuit>());
        commands.push_back(std::make_shared<ObjSay>());
        commands.push_back(std::make_shared<ObjPose>());
        commands.push_back(std::make_shared<ObjEmote>());
        commands.push_back(std::make_shared<ObjSemipose>());
        commands.push_back(std::make_shared<ObjWhisper>());
        commands.push_back(std::make_shared<ObjShout>());
        commands.push_back(std::make_shared<ObjGet>());
        commands.push_back(std::make_shared<ObjTake>());
        commands.push_back(std::make_shared<ObjPut>());
        commands.push_back(std::make_shared<ObjGive>());
        commands.push_back(std::make_shared<ObjDrop>());
        commands.push_back(std::make_shared<ObjInventory>());
        commands.push_back(std::make_shared<ObjEquip>());

        for(auto &command : commands) {
            registerCommand(command);
        }
    }
}