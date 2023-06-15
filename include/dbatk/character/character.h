#pragma once
#include "dbatk/object/object.h"
#include "dbatk/character/race.h"
#include "dbatk/character/sensei.h"
#include "dbatk/character/position.h"
#include "dbatk/character/sex.h"
#include "dbatk/character/stat.h"

namespace dbat {
    class Character : public Object {
    public:
        using Object::Object;
        ~Character() override = default;
        [[nodiscard]] race::RaceID getRaceID() const;
        [[nodiscard]] race::Race* getRace() const;
        [[nodiscard]] virtual OpResult<> setRace(race::RaceID id) = 0;
        [[nodiscard]] virtual OpResult<> setRace(const std::string& txt) = 0;

        size::SizeID getSizeID() const override;

        [[nodiscard]] position::PositionID getPositionID() const;
        [[nodiscard]] position::Position* getPosition() const;
        [[nodiscard]] sensei::SenseiID getSenseiID() const;
        [[nodiscard]] sensei::Sensei* getSensei() const;
        [[nodiscard]] virtual bool isNPC() const = 0;
        [[nodiscard]] std::array<double, cstat::NUM_CHAR_STATS>& getCstats();
    protected:
        race::RaceID raceId{};
        sensei::SenseiID senseiId{};
        position::PositionID positionId{};
        std::array<double, cstat::NUM_CHAR_STATS> cstats;

    };
}