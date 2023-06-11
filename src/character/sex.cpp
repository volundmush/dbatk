#include "dbatk/character/sex.h"

namespace dbat::sex {

    const std::vector<std::shared_ptr<Sex>> sexes = {
            std::make_shared<Neuter>(),
            std::make_shared<Male>(),
            std::make_shared<Female>()
    };

    Sex* findSex(std::size_t id, std::function<bool(std::shared_ptr<Sex>)> predicate) {
        // First, if the id is not within the range of races, return nullptr.
        if(id > sexes.size() - 1) {
            return nullptr;
        }
        auto &r = sexes[id];
        // check predicate...
        if(predicate(r)) {
            return r.get();
        }
        return nullptr;
    }

    Sex* findSex(const std::string &txt, std::function<bool(std::shared_ptr<Sex>)> predicate) {
        std::vector<std::shared_ptr<Sex>> filtered;
        std::copy_if(sexes.begin(), sexes.end(), std::back_inserter(filtered), predicate);
        auto res = partialMatch(txt, filtered.begin(), filtered.end(), false, [](auto r) { return r->getName(); });
        return res == filtered.end() ? nullptr : (*res).get();
    }


}