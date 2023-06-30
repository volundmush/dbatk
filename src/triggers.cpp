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
}