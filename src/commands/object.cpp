#include "dbatk/commands.h"
#include "dbatk/api.h"
#include "dbatk/components.h"
#include "dbatk/aspects/direction.h"
#include "dbatk/operations/movement.h"
#include "dbatk/operations/information.h"
#include "dbatk/triggers.h"
#include "dbatk/message.h"
#include "dbatk/search.h"
#include "dbatk/color.h"
#include "fmt/format.h"

namespace dbat::cmd {
    // Object Commands.
    struct ObjCmd : Command {
        [[nodiscard]] bool isAvailable(entt::entity ent) override {return registry.any_of<Character,Item,Vehicle>(ent);};
    };

    /*
     * The look command is a staple of MUDs; it should show you where you are and what the room looks like.
     * It could also be used to look AT something, like a character or item in the room or in your inventory.
     */
    struct ObjLook : ObjCmd {
        [[nodiscard]] std::string getCmdName() override {return "look";};
        [[nodiscard]] std::set<std::string> getAliases() override {return {"l"};};
        void execute(entt::entity ent, std::unordered_map<std::string, std::string>& input) override;
    };

    /*
     * This help command will be able to get help on any command you have access to.
     */
    struct ObjHelp : ObjCmd {
        [[nodiscard]] std::string getCmdName() override {return "help";};
        [[nodiscard]] std::set<std::string> getAliases() override {return {"h"};};
        void execute(entt::entity ent, std::unordered_map<std::string, std::string>& input) override;
    };

    struct ObjCommands : ObjCmd {
        [[nodiscard]] std::string getCmdName() override {return "commands";};
        void execute(entt::entity ent, std::unordered_map<std::string, std::string>& input) override;
    };

    /*
     * This command will allow you to move from room to room.
     * This is an "object" command because many things should have this functionality
     * if an object is being possessed for normal control by a player or admin.
     * For example, controls might be passed to a spaceship or other vehicle.
     * Or, an admin might be possessing a statue (technically an item) and
     * wants to move to a different room...
     */
    struct ObjMove : ObjCmd {
        [[nodiscard]] std::string getCmdName() override { return "move"; };
        [[nodiscard]] std::set<std::string> getAliases() override { return {"mv", "go",
                                                                            "north", "n",
                                                                            "south", "s",
                                                                            "east", "e",
                                                                            "west", "w",
                                                                            "up", "u",
                                                                            "down", "d",
                                                                            "in", "inside",
                                                                            "out", "outside",
                                                                            "northwest", "nw",
                                                                            "northeast", "ne",
                                                                            "southwest", "sw",
                                                                            "southeast", "se"}; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
        OpResult<> canExecute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    /*
     * The QUIT command is used to exit play and return to the Account menu.
     */
    struct ObjQuit : ObjCmd {
        [[nodiscard]] std::string getCmdName() override {return "quit";};
        void execute(entt::entity ent, std::unordered_map<std::string, std::string>& input) override;
    };

    /*
     * The SAY command is used to speak to other players in the same room.
     * It's an Object Command for the same reason as "move".
     */
    struct ObjSay : ObjCmd {
        [[nodiscard]] std::string getCmdName() override { return "say"; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    /*
     * The POSE command is used to emote.
     * It's an Object Command for the same reason as "move".
     *
     * This would show to others, "<actor> <pose>"
     */
    struct ObjPose : ObjCmd {
        [[nodiscard]] std::string getCmdName() override { return "pose"; };
        [[nodiscard]] std::set<std::string> getAliases() override {return {";"};};
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    struct ObjEmote : ObjCmd {
        [[nodiscard]] std::string getCmdName() override { return "emote"; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    /*
     * The SEMIPOSE command is used to emote, but slightly different in that it lacks the space.
     * This is good for slightly differently formatted things, like adding an apostrophe's.
     * It's an Object Command for the same reason as "move".
     *
     * This would show to others, "<actor><pose>"
     */
    struct ObjSemipose : ObjCmd {
        [[nodiscard]] std::string getCmdName() override { return "semipose"; };
        [[nodiscard]] std::set<std::string> getAliases() override {return {":"};};
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    /*
     * The WHISPER command is used to speak to other players in the same room.
     */
    struct ObjWhisper : ObjCmd {
        [[nodiscard]] std::string getCmdName() override { return "whisper"; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    /*
     * The SHOUT command is used to speak to other players in the same area.
     */
    struct ObjShout : ObjCmd {
        [[nodiscard]] std::string getCmdName() override { return "shout"; };
        [[nodiscard]] std::set<std::string> getAliases() override {return {"yell"};};
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    /*
     * The "get" command is used to retrieve an object from the room or, sometimes, an object in the room.
     */
    struct ObjGet : ObjCmd {
        [[nodiscard]] std::string getCmdName() override { return "get"; };
        [[nodiscard]] std::set<std::string> getAliases() override { return {"take"}; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    /*
     * The "put" command is used to put an object into another object.
     */
    struct ObjPut : ObjCmd {
        [[nodiscard]] std::string getCmdName() override { return "put"; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    /*
     * The "give" command is used to give an object to another player.
     */
    struct ObjGive : ObjCmd {
        [[nodiscard]] std::string getCmdName() override { return "give"; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    /*
     * The "drop" command is used to drop an object from your inventory into the room.
     */
    struct ObjDrop : ObjCmd {
        [[nodiscard]] std::string getCmdName() override { return "drop"; };
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

    /*
     * The "inventory" command is used to list the objects in your inventory.
     */
    struct ObjInventory : ObjCmd {
        [[nodiscard]] std::string getCmdName() override { return "inventory"; };
        [[nodiscard]] std::set<std::string> getAliases() override {return {"inv", "i"};};
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };


    struct ObjEquip : ObjCmd {
        [[nodiscard]] std::string getCmdName() override { return "equip"; };
        [[nodiscard]] std::set<std::string> getAliases() override {return {"eq", "wear", "wield", "hold"};};
        void execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) override;
    };

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

    void ObjCommands::execute(entt::entity ent, std::unordered_map<std::string, std::string> &input) {
        auto args = input["args"];

        auto commands = getSortedCommands(ent);

        if(args.empty()) {
            // We will display all available commands the player can see/use.
            std::vector<Command*> cmds;
            for(auto &[key, cmd] : commands) {
                // If cmd is not in the vector, add it.
                if(!cmd->isAvailable(ent)) continue;
                if(std::find(cmds.begin(), cmds.end(), cmd) == cmds.end()) {
                    cmds.push_back(cmd);
                }
            }
            // Sort the vector by cmd->getCmdName()
            std::sort(cmds.begin(), cmds.end(), [](Command* a, Command* b) {
                return a->getCmdName() < b->getCmdName();
            });

            std::vector<std::string> lines;

            for(auto c : cmds) {
                auto brief = c->getBrief();
                auto aliases = c->getAliases();
                std::vector<std::string> sections;
                sections.emplace_back(c->getCmdName());
                if(!aliases.empty()) {
                    sections.emplace_back(fmt::format("({})", boost::join(aliases, ", ")));
                }
                if(!brief.empty()) {
                    sections.emplace_back(fmt::format("{}", brief));
                }
                lines.emplace_back(boost::join(sections, " - "));
            }
            sendLine(ent, boost::join(lines, "\n"));

        } else {
            for(auto &[key, cmd] : commands) {
                if(!cmd->isAvailable(ent))
                    continue;
                if(boost::iequals(args, key)) {
                    std::vector<std::string> lines;
                    lines.emplace_back("Help: " + cmd->getCmdName());
                    auto aliases = cmd->getAliases();
                    if(!aliases.empty()) {
                        lines.emplace_back(fmt::format("Aliases: {}", boost::join(aliases, ", ")));
                    }
                    lines.emplace_back("------------------");
                    auto help = cmd->getHelp();
                    if(help.empty()) {
                        lines.emplace_back("No help available.");
                    } else {
                        lines.emplace_back(help);
                    }
                    lines.emplace_back("------------------");
                    sendLine(ent, boost::join(lines, "\n"));
                    return;
                }
            }
            sendLine(ent, fmt::format("Command '{}' not found.", args));
        }
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
        for(auto &w : words) {
            boost::trim(w);
        }
        if(boost::iequals(words[0], "all")) {
            words[0] = "all.*";
        }

        if(words.size() == 1) {
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
            s.useAsterisk(true);

            if(boost::iequals(args, "all")) {
                args = "all.*";
            }

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
        commands.push_back(std::make_shared<ObjCommands>());
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