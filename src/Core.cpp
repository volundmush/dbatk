#include "dbatk/Core.h"
#include "kaizermud/Stats.h"
#include "kaizermud/Aspects.h"
#include "dbatk/Database.h"
#include "kaizermud/Database.h"


namespace kaizermud::dbat {



    void registerResources() {

        db::schema.insert(db::schema.end(), extendSchema.begin(), extendSchema.end());

        db::preLoadFuncs.push_back(&loadFlags);
        db::preSaveFuncs.push_back(&saveFlags);

    }
}