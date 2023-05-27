#include "dbatk/DgScripts.h"

namespace kaizermud::dbat {
    std::map<ObjectID, std::shared_ptr<DgScriptPrototype>> dgScripts{};

    std::set<std::shared_ptr<DgScript>> dgScriptsWaiting{};
}