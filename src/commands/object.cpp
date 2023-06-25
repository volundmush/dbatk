#include "dbatk/commands/object.h"
#include "core/api.h"
#include "dbatk/components.h"
#include "dbatk/aspects/direction.h"
#include "dbatk/operations/movement.h"

namespace dbat::cmd {
    void ObjLook::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto loc = getLocation(ent);
        if(!registry.valid(loc)) {
            sendText(ent, "You are nowhere.");
            return;
        }
        atLook(ent, loc);
    }

    void ObjHelp::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

    }



    OpResult<> ObjMove::canExecute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        // This command requires that ent be in a location.
        auto loc = getLocation(ent);
        if(!registry.valid(loc)) {
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
                sendText(ent, "Usage: move <direction>");
                return;
            }
        } else {
            direction = cmd;
        }

        auto [res, err] = op::travelInDirection(ent, direction);
        if(!res) {
            sendText(ent, err.value());
            return;
        }
    }


    void ObjQuit::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

    }

    void ObjSay::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

    }

    void ObjPose::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

    }

    void ObjSemipose::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

    }

    void ObjWhisper::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

    }

    void ObjShout::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {

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