#include "dbatk/aspects/pref.h"

namespace dbat::pref {

    class SimplePreferenceFlag : public PreferenceFlag {
    public:
        SimplePreferenceFlag(std::size_t id, std::string name) : id(id), name(std::move(name)) {}
        [[nodiscard]] std::size_t getId() const override { return id; }
        [[nodiscard]] std::string getName() const override { return name; }
    protected:
        std::size_t id;
        std::string name;
    };

    const std::vector<std::shared_ptr<PreferenceFlag>> preferenceFlags = {
            std::make_shared<SimplePreferenceFlag>(BRIEF, "BRIEF"),
            std::make_shared<SimplePreferenceFlag>(COMPACT, "COMPACT"),
            std::make_shared<SimplePreferenceFlag>(DEAF, "DEAF"),
            std::make_shared<SimplePreferenceFlag>(NOTELL, "NOTELL"),
            std::make_shared<SimplePreferenceFlag>(DISPHP, "DISPHP"),
            std::make_shared<SimplePreferenceFlag>(DISPMANA, "DISPMANA"),
            std::make_shared<SimplePreferenceFlag>(DISPMOVE, "DISPMOVE"),
            std::make_shared<SimplePreferenceFlag>(AUTOEXIT, "AUTOEXIT"),
            std::make_shared<SimplePreferenceFlag>(NOHASSLE, "NOHASSLE"),
            std::make_shared<SimplePreferenceFlag>(QUEST, "QUEST"),
            std::make_shared<SimplePreferenceFlag>(SUMMONABLE, "SUMMONABLE"),
            std::make_shared<SimplePreferenceFlag>(NOREPEAT, "NOREPEAT"),
            std::make_shared<SimplePreferenceFlag>(HOLYLIGHT, "HOLYLIGHT"),
            std::make_shared<SimplePreferenceFlag>(COLOR, "COLOR"),
            std::make_shared<SimplePreferenceFlag>(SPARE, "SPARE"),
            std::make_shared<SimplePreferenceFlag>(NOWIZ, "NOWIZ"),
            std::make_shared<SimplePreferenceFlag>(LOG1, "LOG1"),
            std::make_shared<SimplePreferenceFlag>(LOG2, "LOG2"),
            std::make_shared<SimplePreferenceFlag>(NOAUCT, "NOAUCT"),
            std::make_shared<SimplePreferenceFlag>(NOGOSS, "NOGOSS"),
            std::make_shared<SimplePreferenceFlag>(NOGRATZ, "NOGRATZ"),
            std::make_shared<SimplePreferenceFlag>(ROOMFLAGS, "ROOMFLAGS"),
            std::make_shared<SimplePreferenceFlag>(DISPAUTO, "DISPAUTO"),
            std::make_shared<SimplePreferenceFlag>(CLS, "CLS"),
            std::make_shared<SimplePreferenceFlag>(BUILDWALK, "BUILDWALK"),
            std::make_shared<SimplePreferenceFlag>(AFK, "AFK"),
            std::make_shared<SimplePreferenceFlag>(AUTOLOOT, "AUTOLOOT"),
            std::make_shared<SimplePreferenceFlag>(AUTOGOLD, "AUTOGOLD"),
            std::make_shared<SimplePreferenceFlag>(AUTOSPLIT, "AUTOSPLIT"),
            std::make_shared<SimplePreferenceFlag>(FULL_EXIT, "FULL_EXIT"),
            std::make_shared<SimplePreferenceFlag>(AUTOSAC, "AUTOSAC"),
            std::make_shared<SimplePreferenceFlag>(AUTOMEM, "AUTOMEM"),
            std::make_shared<SimplePreferenceFlag>(VIEWORDER, "VIEWORDER"),
            std::make_shared<SimplePreferenceFlag>(NOCOMPRESS, "NOCOMPRESS"),
            std::make_shared<SimplePreferenceFlag>(AUTOASSIST, "AUTOASSIST"),
            std::make_shared<SimplePreferenceFlag>(DISPKI, "DISPKI"),
            std::make_shared<SimplePreferenceFlag>(DISPEXP, "DISPEXP"),
            std::make_shared<SimplePreferenceFlag>(DISPTNL, "DISPTNL"),
            std::make_shared<SimplePreferenceFlag>(TEST, "TEST"),
            std::make_shared<SimplePreferenceFlag>(HIDE, "HIDE"),
            std::make_shared<SimplePreferenceFlag>(NMWARN, "NMWARN"),
            std::make_shared<SimplePreferenceFlag>(HINTS, "HINTS"),
            std::make_shared<SimplePreferenceFlag>(FURY, "FURY"),
            std::make_shared<SimplePreferenceFlag>(NODEC, "NODEC"),
            std::make_shared<SimplePreferenceFlag>(NOEQSEE, "NOEQSEE"),
            std::make_shared<SimplePreferenceFlag>(NOMUSIC, "NOMUSIC"),
            std::make_shared<SimplePreferenceFlag>(LKEEP, "LKEEP"),
            std::make_shared<SimplePreferenceFlag>(DISTIME, "DISTIME"),
            std::make_shared<SimplePreferenceFlag>(DISGOLD, "DISGOLD"),
            std::make_shared<SimplePreferenceFlag>(DISPRAC, "DISPRAC"),
            std::make_shared<SimplePreferenceFlag>(NOPARRY, "NOPARRY"),
            std::make_shared<SimplePreferenceFlag>(DISHUTH, "DISHUTH"),
            std::make_shared<SimplePreferenceFlag>(DISPERC, "DISPERC"),
            std::make_shared<SimplePreferenceFlag>(CARVE, "CARVE"),
            std::make_shared<SimplePreferenceFlag>(ARENAWATCH, "ARENAWATCH"),
            std::make_shared<SimplePreferenceFlag>(NOGIVE, "NOGIVE"),
            std::make_shared<SimplePreferenceFlag>(INSTRUCT, "INSTRUCT"),
            std::make_shared<SimplePreferenceFlag>(GHEALTH, "GHEALTH"),
            std::make_shared<SimplePreferenceFlag>(IHEALTH, "IHEALTH"),
            std::make_shared<SimplePreferenceFlag>(ENERGIZE, "ENERGIZE")
    };

    std::optional<PrefId> getPreferenceFlagId(const std::string& name, bool exact) {
        auto found = partialMatch(name, preferenceFlags.begin(), preferenceFlags.end(), exact, [](const auto& flag) { return flag->getName(); });
        if(found != preferenceFlags.end()) {
            return static_cast<PrefId>((*found)->getId());
        }
        return std::nullopt;
    }


}