#include "dbatk/DgScripts.h"

namespace dbat {
    std::map<kaizer::ObjectID, std::shared_ptr<DgScriptPrototype>> dgScripts{};

    std::set<std::shared_ptr<DgScript>> dgScriptsWaiting{};
}