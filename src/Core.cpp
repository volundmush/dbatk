#include "dbatk/Core.h"
#include "kaizermud/Object.h"
#include "kaizermud/Stats.h"
#include "kaizermud/Aspects.h"
#include "dbatk/Room.h"
#include "dbatk/Character.h"
#include "dbatk/Item.h"
#include "dbatk/Grid.h"
#include "dbatk/Sector.h"
#include "dbatk/Structure.h"
#include "dbatk/Exit.h"
#include "dbatk/Database.h"
#include "kaizermud/Database.h"


namespace kaizermud::dbat {

    template<typename T>
    game::ObjectEntry makeObjectEntry(std::string type, std::string subtype) {
        return {type, subtype, [](ObjectID id) { return std::make_shared<T>(id); }};
    }

    void registerResources() {

        db::schema.insert(db::schema.end(), extendSchema.begin(), extendSchema.end());

        db::preLoadFuncs.push_back(&loadFlags);
        db::preSaveFuncs.push_back(&saveFlags);

        std::vector<game::ObjectEntry> objectEntries = {
                makeObjectEntry<Room>("room", "basic"),
                makeObjectEntry<PlayerCharacter>("character", "player"),
                makeObjectEntry<NonPlayerCharacter>("character", "npc"),
                makeObjectEntry<Item>("item", "basic"),
                makeObjectEntry<Grid>("grid", "basic"),
                makeObjectEntry<Sector>("sector", "basic"),
                makeObjectEntry<Structure>("structure", "basic"),
                makeObjectEntry<Exit>("exit", "basic"),

        };

        for (auto &entry: objectEntries) {
            const auto &[result, err] = game::registerObject(std::move(entry));
            if (!result)
                throw std::runtime_error(std::string(err.value()));
        }

    }
}