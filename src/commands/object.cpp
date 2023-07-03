#include "dbatk/commands/object.h"
#include "dbatk/api.h"
#include "dbatk/components.h"
#include "dbatk/aspects/direction.h"
#include "dbatk/operations/movement.h"
#include "dbatk/operations/information.h"
#include "dbatk/triggers.h"
#include "dbatk/message.h"
#include "dbatk/search.h"
#include "dbatk/color.h"

namespace dbat::cmd {
    void ObjLook::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto loc = registry.try_get<Location>(ent);
        if(!loc) {
            sendLine(ent, "You are nowhere.");
            return;
        }

        auto args = input["args"];
        boost::trim(args);
        if(args.empty()) {
            sendLine(ent, op::renderLocation(*loc, ent));
            return;
        }
        if(boost::istarts_with(args, "at ")) {
            args = args.substr(3);
            boost::trim(args);
        }

        Search s(ent);
        s.in(*loc);
        s.inventory(ent);
        s.equipment(ent);

        auto res = s.find(args);
        if(res.empty()) {
            sendLine(ent, "You don't see that here.");
            return;
        }

        Location l;
        l.locationType = LocationType::Absolute;
        l.data = res[0];

        sendLine(ent, op::renderLocation(l, ent));
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
        if(args.empty()) {
            sendLine(ent, "Say what?");
            return;
        }

        MsgFormat m(ent, fmt::format("$You() @W$conj(say), '@n@C{}@W'", args));
        m.in(*loc);
        m.send();
        triggerSpeech(ent, stripAnsi(args));

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
        m.in(*loc);
        m.send();
    }

    void ObjEmote::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto loc = registry.try_get<Location>(ent);
        if(!loc) {
            sendLine(ent, "You are nowhere. There is nobody to notice you.");
            return;
        }

        auto args = input["args"];
        if(args.empty()) {
            sendLine(ent, "Emote what?");
            return;
        }
        // We need to perform some substitution on the string:
        // &1 becomes: '@C
        // &2 becomes: @N'

        boost::replace_all(args, "&1", "'@C");
        boost::replace_all(args, "&2", "@n'");

        // TODO: Handle the *target stuff for roleplay later.

        MsgFormat m(ent, fmt::format("$You() {}", args));
        m.in(*loc);

        m.send();
    }

    void ObjEcho::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto loc = registry.try_get<Location>(ent);
        if(!loc) {
            sendLine(ent, "You are nowhere. There is nobody to notice you.");
            return;
        }

        auto args = input["args"];
        if(args.empty()) {
            sendLine(ent, "Echo what?");
            return;
        }

        // TODO: Handle the *target stuff for roleplay later.

        MsgFormat m(ent, args);
        m.in(*loc);
        m.send();
    }


    void ObjSemipose::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto loc = registry.try_get<Location>(ent);
        if(!loc) {
            sendLine(ent, "You are nowhere. There is nobody to notice you.");
            return;
        }

        auto args = input["args"];
        if(args.empty()) {
            sendLine(ent, "Semipose what?");
            return;
        }

        MsgFormat m(ent, fmt::format("$You(){}", args));
        m.in(*loc);
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
        if(args.empty()) {
            sendLine(ent, "Shout what?");
            return;
        }

        MsgFormat m(ent, fmt::format("@y$You() $conj(shout), \"{}\"@n", args));
        m.region(loc->data);
        m.send();
    }

    void ObjGet::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto loc = registry.try_get<Location>(ent);
        if(!loc) {
            sendLine(ent, "You are nowhere. Beware the void and what dwells here.");
            return;
        }
        auto args = input["args"];
        if(args.empty()) {
            sendLine(ent, "Get what?");
            return;
        }

        // TODO: Arms check.

        std::vector<std::string> words;
        boost::split(words, args, boost::is_space());

        Location inv;
        inv.data = ent;
        inv.locationType = LocationType::Inventory;

        MoveParams params;
        params.dest = inv;
        params.moveType = MoveType::Get;
        params.traverseType = TraverseType::Physical;
        params.mover = ent;


        Search s(ent);
        s.useAll(true);
        s.setType(SearchType::Items);
        s.useAsterisk(true);
        if(words.empty()) {
            sendLine(ent, "Get what?");
            return;
        }
        else if(words.size() == 1) {
            // We are getting from the location...
            s.in(*loc);
            auto results = s.find(words[0]);
            if(results.empty()) {
                sendLine(ent, "You don't see that here.");
                return;
            }
            for(auto e : results) {
                auto p = params;
                auto [res, err] = moveTo(e, p);
                if(!res) {
                    sendLine(ent, fmt::format("Cannot get {}: {}", getDisplayName(e, ent), err.value()));
                }
            }
        }
        else if(words.size() >= 2) {
            // We are getting from containers.
            Search s2(ent);
            s2.useAll(true);
            s2.setType(SearchType::Items);
            s2.useAsterisk(true);
            s2.inventory(ent);
            s2.equipment(ent);
            s2.in(*loc);
            auto containers = s2.find(words[1]);
            if(containers.empty()) {
                sendLine(ent, "You don't see that here.");
                return;
            }
            for(auto c : containers) {
                if(auto con = registry.try_get<ContainerData>(c); con) {
                    if(con->containerFlags.test(con::CLOSED)) {
                        sendLine(ent, fmt::format("The {} is closed.", getDisplayName(c, ent)));
                        continue;
                    }
                    s.inventory(c);
                } else {
                    sendLine(ent, fmt::format("The {} is not a container.", getDisplayName(c, ent)));
                    continue;
                }
            }
            if(boost::iequals(words[0], "all")) {
                words[0] = "all.*";
            }
            auto results = s.find(words[0]);
            if(results.empty()) {
                sendLine(ent, "You don't see that here.");
                return;
            }
            for(auto e : results) {
                auto p = params;
                auto [res, err] = moveTo(e, p);
                if(!res) {
                    sendLine(ent, fmt::format("Cannot get {}: {}", getDisplayName(e, ent), err.value()));
                }
            }
        }
    }

    void ObjPut::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto loc = registry.try_get<Location>(ent);
        if(!loc) {
            sendLine(ent, "You are nowhere. Beware the void and what dwells here.");
            return;
        }
        auto args = input["args"];
        if(args.empty()) {
            sendLine(ent, "Put what where?");
            return;
        }

        // TODO: Arms check.

        std::vector<std::string> words;
        boost::split(words, args, boost::is_space());
        if(words.size() < 2) {
            sendLine(ent, "Put what where?");
            return;
        }

        MoveParams params;
        params.moveType = MoveType::Get;
        params.traverseType = TraverseType::Physical;
        params.mover = ent;


        Search s(ent);
        s.useAll(true);
        s.setType(SearchType::Items);
        s.useAsterisk(true);
        s.inventory(ent);

        // We are putting into a single container.
        Search s2(ent);
        s2.useAll(false);
        s2.setType(SearchType::Items);
        s2.useAsterisk(false);
        s2.inventory(ent);
        s2.equipment(ent);
        s2.in(*loc);

        auto containers = s2.find(words[1]);
        if(containers.empty()) {
            sendLine(ent, "You don't see that here.");
            return;
        }

        auto c = containers[0];
        if(auto con = registry.try_get<ContainerData>(c); con) {
            if(con->containerFlags.test(con::CLOSED)) {
                sendLine(ent, fmt::format("The {} is closed.", getDisplayName(c, ent)));
                return;
            }
        } else {
            sendLine(ent, fmt::format("The {} is not a container.", getDisplayName(c, ent)));
            return;
        }

        Location inv;
        inv.data = c;
        inv.locationType = LocationType::Inventory;
        params.dest = inv;

        if(boost::iequals(words[0], "all")) {
            words[0] = "all.*";
        }
        auto results = s.find(words[0]);
        if(results.empty()) {
            sendLine(ent, "You don't see that here.");
            return;
        }
        for(auto e : results) {
            auto p = params;
            auto [res, err] = moveTo(e, p);
            if(!res) {
                sendLine(ent, fmt::format("Cannot put {} into {}: {}", getDisplayName(e, ent), getDisplayName(c, ent), err.value()));
            }
        }
    }

    void ObjGive::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto loc = registry.try_get<Location>(ent);
        if(!loc) {
            sendLine(ent, "You are nowhere. Beware the void and what dwells here.");
            return;
        }
        auto args = input["args"];
        if(args.empty()) {
            sendLine(ent, "Give what to who?");
            return;
        }

        // TODO: Arms check.

        std::vector<std::string> words;
        boost::split(words, args, boost::is_space());
        if(words.size() < 2) {
            sendLine(ent, "Give what to who?");
            return;
        }

        Search st(ent);
        st.in(*loc);
        st.setType(SearchType::Characters);
        st.useAll(false);
        st.useAsterisk(false);

        std::string targetName = "";

        if(words.size() >= 3) {
            targetName = words[2];
        } else {
            targetName = words[1];
        }
        entt::entity target = entt::null;

        auto targets = st.find(targetName);
        if(targets.empty()) {
            sendLine(ent, "You don't see them here.");
            return;
        } else {
            target = targets[0];
        }

        MoveParams params;
        params.moveType = MoveType::Give;
        params.traverseType = TraverseType::Physical;
        params.mover = ent;

        Location inv;
        inv.data = target;
        inv.locationType = LocationType::Inventory;
        params.dest = inv;

        std::vector<entt::entity> items;
        bool isMoney = false;
        int64_t amount = 0;

        if(words.size() == 2) {
            // This is an item transfer, we are transferring search of words[0] to target...

            Search s(ent);
            s.setType(SearchType::Items);
            s.inventory(ent);
            s.useAll(true);
            s.useAsterisk(true);

            items = s.find(words[0]);
            if(items.empty()) {
                sendLine(ent, "You don't have that.");
                return;
            }

        } else if (words.size() >= 3) {
            // This is a money transfer, we are transferring words[0] words[1] to target...
            if(!(boost::iequals(words[1], "zenni") || boost::iequals(words[1], "coins")
                || boost::iequals(words[1], "money") || boost::iequals(words[1], "coin") || boost::iequals(words[1], "zenny"))) {
                sendLine(ent, "You can only give money. Try zenni, zenny, coin, coins, or money.");
                return;
            }
            // words[0] must be an integer.
            isMoney = true;

            try {
                amount = std::stoll(words[0]);
            } catch(std::exception &e) {
                sendLine(ent, "That isn't a number!");
                return;
            }
            if(amount <= 0) {
                sendLine(ent, "You... can't give negative amounts of money, you weirdo.");
                return;
            }
            auto money = registry.try_get<Money>(ent);
            if(!money) {
                sendLine(ent, "You don't have any money.");
                return;
            }
            if(money->data < amount) {
                sendLine(ent, "You don't have that much money.");
                return;
            }
            items.emplace_back(createMoneyPile(amount));
        }

        for(auto item : items) {
            auto [res, err] = moveTo(item, params);
            if(!res) {
                sendLine(ent, fmt::format("Cannot give {} to {}: {}", getDisplayName(item, ent), getDisplayName(target, ent), err.value()));
                if(isMoney) {
                    deleteObject(item);
                }
            } else {
                if(isMoney) {
                    auto &cur = registry.get_or_emplace<Money>(ent);
                    cur.data -= amount;
                }
            }
        }

    }

    void ObjDrop::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto loc = registry.try_get<Location>(ent);
        if(!loc) {
            sendLine(ent, "You are nowhere. Dropping stuff nowhere is bad for the universe's health.");
            return;
        }

        auto args = input["args"];

        std::vector<std::string> words;
        boost::split(words, args, boost::is_space());
        int64_t amount = 0;
        bool isMoney = false;
        std::vector<entt::entity> results;

        if(words.empty()) {
            sendLine(ent, "Drop what?");
            return;
        } else if(words.size() == 1) {
            Search s(ent);
            s.inventory(ent);
            s.useAll(true);
            s.setType(SearchType::Items);

            results = s.find(args);
            if(results.empty()) {
                sendLine(ent, "You don't have that.");
                return;
            }
        } else if(words.size() >= 2) {
            if(!(boost::iequals(words[1], "zenni") || boost::iequals(words[1], "coins")
                 || boost::iequals(words[1], "money") || boost::iequals(words[1], "coin") || boost::iequals(words[1], "zenny"))) {
                sendLine(ent, "Try zenni, zenny, coin, coins, or money.");
                return;
            }
            // words[0] must be an integer.
            isMoney = true;
            try {
                amount = std::stoll(words[0]);
            } catch(std::exception &e) {
                sendLine(ent, "That isn't a number!");
                return;
            }
            if(amount <= 0) {
                sendLine(ent, "You... can't drop negative amounts of money, you weirdo.");
                return;
            }
            auto money = registry.try_get<Money>(ent);
            if(!money) {
                sendLine(ent, "You don't have any money.");
                return;
            }
            if(money->data < amount) {
                sendLine(ent, "You don't have that much money.");
                return;
            }
            results.emplace_back(createMoneyPile(amount));
        }

        Location inv;
        inv.data = ent;
        inv.locationType = LocationType::Inventory;

        MoveParams params;
        params.moveType = MoveType::Drop;
        params.traverseType = TraverseType::Physical;
        params.previousLocation = inv;
        params.dest = *loc;
        params.mover = ent;

        for(auto e : results) {
            auto [res, err] = moveTo(e, params);
            if(!res) {
                sendLine(ent, fmt::format("Cannot drop {}: {}", getDisplayName(e, ent), err.value()));
                if(isMoney) {
                    deleteObject(e);
                }
            } else {
                if(isMoney) {
                    auto &cur = registry.get_or_emplace<Money>(ent);
                    cur.data -= amount;
                }
            }
        }
    }

    void ObjInventory::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        Location l;
        l.data = ent;
        l.locationType = LocationType::Inventory;
        std::vector<std::string> lines;
        lines.emplace_back("              @YInventory@n");
        lines.emplace_back("-------------------------------------");
        lines.emplace_back(op::renderLocation(l, ent));
        sendLine(ent, boost::join(lines, "\n"));
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