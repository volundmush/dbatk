#include "dbatk/dgscript.h"

namespace dbat {
    std::map<std::size_t, std::shared_ptr<DgScriptPrototype>> dgScripts{};

    std::set<std::shared_ptr<DgScript>> dgScriptsWaiting{};
}