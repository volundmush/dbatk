#pragma once
#include "dbatk/base.h"

namespace dbat {

    enum class SearchType {
        Anything = 0, // Characters, Items, and Vehicles... anything, really.
        Characters = 1, // will also catch players and NPCs!
        Players = 2,
        NPCs = 3,
        Vehicles = 4,
        Items = 5
    };

    struct Location;

    class Search {
    public:
        explicit Search(entt::entity ent);
        Search& in(const Location& loc);
        Search& inventory(entt::entity ent);
        Search& equipment(entt::entity ent);
        Search& modes(uint64_t m);
        Search& useId(bool useId);
        Search& useSelf(bool useSelf);
        Search& useAll(bool useAll);
        Search& useAsterisk(bool useAsterisk);
        Search& setType(SearchType t);

        std::vector<entt::entity> find(std::string_view name);

    protected:
        OpResult<entt::entity> _simplecheck(std::string_view name);
        bool detect(entt::entity target);
        entt::entity ent;
        std::string txt;
        std::vector<Location> searchLocations;
        uint64_t useModes{0};
        SearchType type{SearchType::Anything};
        bool allowId{false};
        bool allowSelf{true};
        bool allowAll{false};
        bool allowAsterisk{false};
    };
}