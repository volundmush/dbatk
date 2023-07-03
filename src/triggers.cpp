#include "dbatk/triggers.h"
#include "dbatk/api.h"

namespace dbat {
    int64_t mobTriggerGreet(entt::entity actor, std::optional<dir::DirectionId> d) {
        bool final = true;
        bool intermediate;
        // This will iterate through every character in the same room...

        auto loc = registry.try_get<Location>(actor);
        if(!loc) {
            return final;
        }
        if(loc->locationType != LocationType::Area) {
            return final;
        }
        auto &ar = registry.get<Area>(loc->data);
        auto &r = ar.data[loc->x];

        auto dirname = d.has_value() ? dir::directions[d.value()].getName() : "none";

        auto &con = registry.get<RoomContents>(r);
        for(auto e : con.data) {
            // we only want characters who aren't actor.
            // They also must have a DgScripts component.
            if(!registry.all_of<Character, DgScripts>(e)) continue;
            if(e == actor) continue;

            auto &dg = registry.get<DgScripts>(e);

            for(auto &sc : dg.scripts) {
                if(!(sc->prototype->triggerType.test(mtrig::GREET_ALL) || (sc->prototype->triggerType.test(mtrig::GREET) && canDetect(e, actor, 0)))) continue;
                if(sc->state != DgState::DORMANT) continue;
                auto rand = randomNumber(1,100);
                if(rand >= sc->prototype->narg) continue;
                sc->setVar("actor", actor);
                sc->setVar("direction", dirname);
                intermediate = sc->execute();
                if(!intermediate) final = false;
           }
        }
        return final;
    }

    int64_t mobTriggerEntry(entt::entity actor) {
        auto dg = registry.try_get<DgScripts>(actor);
        if(!dg) return true;

        for(auto &sc : dg->scripts) {
            if(!(sc->prototype->triggerType.test(mtrig::ENTRY))) continue;
            if(sc->state != DgState::DORMANT) continue;
            auto rand = randomNumber(1,100);
            if(rand >= sc->prototype->narg) continue;
            return sc->execute();
        }
        return true;
    }

    int64_t roomTriggerEntry(entt::entity actor, entt::entity room, std::optional<dir::DirectionId> d) {
        auto dg = registry.try_get<DgScripts>(room);
        if(!dg) return true;
        auto dirname = d.has_value() ? dir::directions[d.value()].getName() : "none";

        for(auto &sc : dg->scripts) {
            if(!(sc->prototype->triggerType.test(rtrig::ENTER))) continue;
            if(sc->state != DgState::DORMANT) continue;
            auto rand = randomNumber(1,100);
            if(rand >= sc->prototype->narg) continue;
            sc->setVar("actor", actor);
            sc->setVar("direction", dirname);
            return sc->execute();
        }

        return 1;
    }

    static bool wordIn(const std::string& word, const std::string& phrase) {
        // we need to determine, case-insensitively, if word is found in phrase.

        // first, we need to split phrase into words.
        std::vector<std::string> words;
        boost::split(words, phrase, boost::is_any_of(" "));
        for(auto &w : words) {
            boost::trim(w);
            if(boost::iequals(w, word)) return true;
        }
        return false;
    }

    void triggerSpeech(entt::entity actor, const std::string& speech) {
        auto words = boost::trim_copy(speech);
        if(words.empty()) return;

        // This trigger will check for scripts on the location (room) and NPCs in the room.
        auto loc = registry.try_get<Location>(actor);
        if(!loc) {
            return;
        }

        auto execute = [&](DgScripts* dg, std::size_t bit) {
            for(auto &sc : dg->scripts) {
                if(!sc->prototype->triggerType.test(bit)) continue;
                if(sc->state != DgState::DORMANT) continue;
                if(!((sc->prototype->narg && wordIn(sc->prototype->arglist, words)) || (!sc->prototype->narg &&
                    boost::icontains(words, sc->prototype->arglist)))) continue;
                sc->setVar("actor", actor);
                sc->setVar("speech", words);
                sc->execute();
            }
        };

        std::vector<entt::entity> contents;
        if(loc->locationType == LocationType::Area) {
            auto &ar = registry.get<Area>(loc->data);
            auto &r = ar.data[loc->x];
            auto &con = registry.get<RoomContents>(r);
            contents = con.data;
            if(auto dg = registry.try_get<DgScripts>(r); dg) {
                execute(dg, rtrig::SPEECH);
            }

        } else if(loc->locationType == LocationType::Expanse) {
            auto &ex = registry.get<GridContents>(loc->data);
            GridPoint gp(loc->x, loc->y, loc->z);
            auto found = ex.data.find(gp);
            if(found == ex.data.end()) {
                return;
            }
            contents = found->second;
        } else {
            return;
        }

        for(auto e : contents) {
            if(!registry.all_of<NPC, DgScripts>(e)) continue;
            if(e == actor) continue;
            auto &dg = registry.get<DgScripts>(e);
            execute(&dg, mtrig::SPEECH);
        }
    }

    void triggerRandom(entt::entity actor) {
        auto dg = registry.try_get<DgScripts>(actor);
        if(!dg) return;
        std::size_t bitcheck = 0;
        if(registry.any_of<Room>(actor)) {
            bitcheck = rtrig::RANDOM;
        }
        else if(registry.any_of<NPC>(actor)) {
            bitcheck = mtrig::RANDOM;
        }
        else if(registry.any_of<Item>(actor)) {
            bitcheck = otrig::RANDOM;
        }
        else {
            return;
        }
        for(auto &sc : dg->scripts) {
            if(!sc->prototype->triggerType.test(bitcheck)) continue;
            if(sc->state != DgState::DORMANT) continue;
            auto rand = randomNumber(1,100);
            if(rand >= sc->prototype->narg) continue;
            sc->execute();
        }
    }

    int64_t triggerCommand(entt::entity actor, const std::string& cmd, const std::string& arg) {
        // First we check the room the actor's in for scripts.
        // Then we check NPCs in the same location as the actor.
        // Lastly, we will check the actor's worn equipment, then their inventory,
        // then Items in the same location. The moment any one script fires, we return.
        // Return 1 if the command was handled, 0 if not.


        auto loc = registry.try_get<Location>(actor);
        if(!loc) return 0;

        std::function<bool(DgScripts*, std::size_t)> check = [&](DgScripts* dg, std::size_t bit) {
            for(auto &sc : dg->scripts) {
                if(!sc->prototype->triggerType.test(bit)) continue;
                if(sc->state != DgState::DORMANT) continue;
                if(boost::iequals(sc->prototype->arglist, "*") || boost::iequals(sc->prototype->arglist, cmd)) {
                    sc->setVar("actor", actor);
                    sc->setVar("cmd", cmd);
                    sc->setVar("arg", arg);
                    if(sc->execute()) return true;
                }
            }
            return false;
        };

        std::vector<entt::entity> contents;
        if(loc->locationType == LocationType::Area) {
            auto &ar = registry.get<Area>(loc->data);
            auto &r = ar.data[loc->x];
            auto &con = registry.get<RoomContents>(r);
            contents = con.data;
            if(auto dg = registry.try_get<DgScripts>(r); dg) {
                if(check(dg, rtrig::COMMAND)) return 1;
            }

        } else if(loc->locationType == LocationType::Expanse) {
            auto &ex = registry.get<GridContents>(loc->data);
            GridPoint gp(loc->x, loc->y, loc->z);
            auto found = ex.data.find(gp);
            if(found != ex.data.end()) {
                contents = found->second;
            }
        }

        for(auto e : contents) {
            if(!registry.all_of<NPC, DgScripts>(e)) continue;
            auto &dg = registry.get<DgScripts>(e);
            if(check(&dg, mtrig::COMMAND)) return 1;
        }

        // Oh well, so far, nothing. Next we will check the actor's equipment.
        if(auto eq = registry.try_get<Equipment>(actor); eq) {
            for(auto &[slot, e] : eq->data) {
                if(!registry.all_of<Item, DgScripts>(e)) continue;
                auto &dg = registry.get<DgScripts>(e);
                if(check(&dg, otrig::COMMAND)) return 1;
            }
        }

        // Still nothing? next we'll check the actor's inventory...
        if(auto inv = registry.try_get<Inventory>(actor); inv) {
            for(auto &e : inv->data) {
                if(!registry.all_of<Item, DgScripts>(e)) continue;
                auto &dg = registry.get<DgScripts>(e);
                if(check(&dg, otrig::COMMAND)) return 1;
            }
        }

        // Lastly, we check items in the room...
        for(auto e : contents) {
            if(!registry.all_of<Item, DgScripts>(e)) continue;
            auto &dg = registry.get<DgScripts>(e);
            if(check(&dg, otrig::COMMAND)) return 1;
        }

        return 0;
    }

    bool mobBribeTrigger(entt::entity actor, entt::entity target, int64_t amount, bool checkOnly) {
        // This trigger will check for scripts on the location (room) and NPCs in the room.

        auto dg = registry.try_get<DgScripts>(target);
        if(!dg) return false;
        bool hadTrigger = false;
        for(auto &sc : dg->scripts) {
            if(!sc->prototype->triggerType.test(mtrig::BRIBE)) continue;
            if(sc->state != DgState::DORMANT) continue;
            hadTrigger = true;
            if(checkOnly) return true;
            sc->setVar("actor", actor);
            sc->setVar("amount", std::to_string(amount));
            sc->execute();
        }

        return hadTrigger;

    }

    bool mobReceiveTrigger(entt::entity actor, entt::entity target, entt::entity item, bool checkOnly) {
        auto dg = registry.try_get<DgScripts>(target);
        if(!dg) return false;

        for(auto &sc : dg->scripts) {
            if(!sc->prototype->triggerType.test(mtrig::RECEIVE)) continue;
            if(sc->state != DgState::DORMANT) continue;
            if(checkOnly) return true;
            sc->setVar("actor", actor);
            sc->setVar("object", item);
            auto outval = sc->execute();
            // TODO: Check if dead and return false if so?
            return outval;
        }

        return true;
    }

}