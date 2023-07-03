#include "dbatk/search.h"
#include "dbatk/api.h"


namespace dbat {
    Search::Search(entt::entity ent) : ent(ent) {

    }

    Search& Search::in(const Location& loc) {
        searchLocations.emplace_back(loc);
        return *this;
    }

    Search& Search::inventory(entt::entity ent) {
        if(ent != entt::null) {
            Location loc;
            loc.data = ent;
            loc.locationType = LocationType::Inventory;
            searchLocations.emplace_back(loc);
        }
        return *this;
    }

    Search& Search::equipment(entt::entity ent) {
        if(ent != entt::null) {
            Location loc;
            loc.data = ent;
            loc.locationType = LocationType::Equipment;
            searchLocations.emplace_back(loc);
        }
        return *this;
    }

    Search& Search::modes(uint64_t m) {
        useModes = m;
        return *this;
    }

    Search& Search::useId(bool useId) {
        this->allowId = useId;
        return *this;
    }

    Search& Search::useSelf(bool useSelf) {
        this->allowSelf = useSelf;
        return *this;
    }

    Search& Search::useAll(bool useAll) {
        this->allowAll = useAll;
        return *this;
    }

    Search& Search::useAsterisk(bool useAsterisk) {
        this->allowAsterisk = useAsterisk;
        return *this;
    }

    Search& Search::setType(SearchType t) {
        this->type = t;
        return *this;
    }

    OpResult<entt::entity> Search::_simplecheck(std::string_view name) {
        if(allowSelf) {
            if(boost::iequals(name, "self") || boost::iequals(name, "me"))
                return {ent, "self check"};
        }
        std::string n(name);
        // if allowID, check for #ID pattern like #5 or #10 and search entities for it...
        if(allowId && isObjId(n) || isObjRef(n)) {
            auto parsed = parseObjectId(n);
            return {parsed, "id search"};
        }

        return {entt::null, std::nullopt};
    }

    bool Search::detect(entt::entity target) {
        return canDetect(ent, target, useModes);
    }

    std::vector<entt::entity> Search::find(std::string_view name) {
        auto [res, handled] = _simplecheck(name);
        if(handled) {
            if(registry.valid(res)) {
                return {res};
            } else {
                return {};
            }
        }

        // So we need to check if the name is prefixed with <something>.<name>.
        // That might be 5.meat or all.meat for instance. if there's no prefix, we
        // assume it's equal to 1.meat, so to speak.

        std::string_view prefix;
        int64_t num = 1;
        bool allMode = false;

        auto dot = name.find('.');
        if(dot != std::string_view::npos) {
            prefix = name.substr(0, dot);
            name = name.substr(dot+1);
        } else {
            prefix = "1";
        }

        // Now check to make sure that the prefix is either a number or the string "all".
        if(!boost::iequals(prefix, "all")) {
            try {
                num = std::stoll(std::string(prefix));
                if(num < 1) return {};
            } catch (std::invalid_argument &e) {
                return {};
            }
        } else {
            // switch to AllMode...
            allMode = true && allowAll;
        }

        std::size_t count = 0;
        bool aster = false;
        if(name == "*") {
            if(!allowAsterisk) return {};
            aster = true;
        }
        std::vector<entt::entity> results;

        for(auto& l : searchLocations) {
            std::vector<entt::entity> ents;
            if(l.locationType == LocationType::Area) {
                ents = getRoomContents(l.getRoom());
            } else if(l.locationType == LocationType::Inventory) {
                ents = getInventory(l.data);
            } else if(l.locationType == LocationType::Equipment) {
                ents = getEquipment(l.data);
            } else if(l.locationType == LocationType::Expanse) {

            } else if(l.locationType == LocationType::Map) {

            } else if(l.locationType == LocationType::Space) {

            }

            if(ents.empty()) continue;
            for(auto e : ents) {
                if(e == ent) continue;
                if(!registry.valid(e)) continue;
                if(type != SearchType::Anything) {
                    switch(type) {
                        case SearchType::Characters:
                            if(!registry.any_of<Character>(e)) continue;
                            break;
                        case SearchType::Players:
                            if(!registry.any_of<Player>(e)) continue;
                            break;
                        case SearchType::NPCs:
                            if(!registry.any_of<NPC>(e)) continue;
                            break;
                        case SearchType::Vehicles:
                            if(!registry.any_of<Vehicle>(e)) continue;
                            break;
                        case SearchType::Items:
                            if(!registry.any_of<Item>(e)) continue;
                            break;
                    }
                }
                if(useModes && !detect(e)) continue;
                if(aster) {
                    results.push_back(e);
                } else {
                    // In allMode, we want ALL things which match name.
                    // but otherwise, we're looking for the nth instance of name.
                    if(checkSearch(e, name, ent)) {
                        if(allMode) {
                            results.push_back(e);
                        } else {
                            count++;
                            if(count == num) {
                                results.push_back(e);
                                return results;
                            }
                        }
                    }
                }
            }
        }

        return results;
    }


}