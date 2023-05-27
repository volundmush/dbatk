#pragma once
#include "kaizermud/Aspects.h"

namespace kaizermud::dbat::aspect {

    class DBATAspect : public kaizermud::game::Aspect {
        using kaizermud::game::Aspect::Aspect;
    };

    class DBATAspectSlot : public kaizermud::game::AspectSlot {
    public:
        using kaizermud::game::AspectSlot::AspectSlot;
        virtual kaizermud::OpResult<> atPostLoad(entt::entity ent);
        [[nodiscard]] virtual std::optional<std::string> getDefault() const = 0;
    };

    // Character Aspect Slots.
    class RaceSlot : public DBATAspectSlot {
    public:
        using DBATAspectSlot::DBATAspectSlot;
        std::optional<std::string> getDefault() const override;
    };

    class SenseiSlot : public DBATAspectSlot {
    public:
        using DBATAspectSlot::DBATAspectSlot;
        std::optional<std::string> getDefault() const override;
    };

    class PositionSlot : public DBATAspectSlot {
    public:
        using DBATAspectSlot::DBATAspectSlot;
        std::optional<std::string> getDefault() const override;
    };

    class SexSlot : public DBATAspectSlot {
    public:
        using DBATAspectSlot::DBATAspectSlot;
        std::optional<std::string> getDefault() const override;
    };

    class SubSpeciesSlot : public DBATAspectSlot {
    public:
        using DBATAspectSlot::DBATAspectSlot;
    };

    // Character Aspects
    // Race
    class Race : public DBATAspect {
    public:
        using DBATAspect::DBATAspect;

    };

    class Human : public Race {
    public:
        using Race::Race;
    };

    class Saiyan : public Race {
    public:
        using Race::Race;
    };

    class Icer : public Race {
    public:
        using Race::Race;
    };

    class Konatsu : public Race {
    public:
        using Race::Race;
    };

    class Namekian : public Race {
    public:
        using Race::Race;
    };

    class Mutant : public Race {
    public:
        using Race::Race;
    };

    class Kanassan : public Race {
    public:
        using Race::Race;
    };

    class HalfSaiyan : public Race {
    public:
        using Race::Race;
    };

    class BioAndroid : public Race {
    public:
        using Race::Race;
    };

    class Android : public Race {
    public:
        using Race::Race;
    };

    class Demon : public Race {
    public:
        using Race::Race;
    };

    class Majin : public Race {
    public:
        using Race::Race;
    };

    class Kai : public Race {
    public:
        using Race::Race;
    };

    class Tuffle : public Race {
    public:
        using Race::Race;
    };

    class Hoshijin : public Race {
    public:
        using Race::Race;
    };

    class Animal : public Race {
    public:
        using Race::Race;
    };

    class Saiba : public Race {
    public:
        using Race::Race;
    };

    class Serpent : public Race {
    public:
        using Race::Race;
    };

    class Ogre : public Race {
    public:
        using Race::Race;
    };

    class Yardratian : public Race {
    public:
        using Race::Race;
    };

    class Arlian : public Race {
    public:
        using Race::Race;
    };

    class Dragon : public Race {
    public:
        using Race::Race;
    };

    class Mechanical : public Race {
    public:
        using Race::Race;
    };

    class Spirit : public Race {
    public:
        using Race::Race;
    };


    // Sensei Stuff
    class Sensei : public DBATAspect {
    public:
        using DBATAspect::DBATAspect;
    };

    class Roshi : public Sensei {
    public:
        using Sensei::Sensei;
    };

    class Piccolo : public Sensei {
    public:
        using Sensei::Sensei;
    };

    class Krane : public Sensei {
    public:
        using Sensei::Sensei;
    };

    class Nail : public Sensei {
    public:
        using Sensei::Sensei;
    };

    class Bardock : public Sensei {
    public:
        using Sensei::Sensei;
    };

    class Ginyu : public Sensei {
    public:
        using Sensei::Sensei;
    };

    class Frieza : public Sensei {
    public:
        using Sensei::Sensei;
    };

    class Tapion : public Sensei {
    public:
        using Sensei::Sensei;
    };

    class Sixteen : public Sensei {
    public:
        using Sensei::Sensei;
    };

    class Dabura : public Sensei {
    public:
        using Sensei::Sensei;
    };

    class Kibito : public Sensei {
    public:
        using Sensei::Sensei;
    };

    class Jinto : public Sensei {
    public:
        using Sensei::Sensei;
    };

    class Tsuna : public Sensei {
    public:
        using Sensei::Sensei;
    };

    class Kurzak : public Sensei {
    public:
        using Sensei::Sensei;
    };

    class Commoner : public Sensei {
    public:
        using Sensei::Sensei;
    };

    // Sex stuff

    class Sex : public DBATAspect {
    public:
        using DBATAspect::DBATAspect;
    };

    class Neuter : public Sex {
    public:
        using Sex::Sex;
    };

    class Male : public Sex {
    public:
        using Sex::Sex;
    };

    class Female : public Sex {
    public:
        using Sex::Sex;
    };

    // Positions
    class Position : public DBATAspect {
    public:
        using DBATAspect::DBATAspect;
    };

    class Dead : public Position {
    public:
        using Position::Position;
    };

    class MortallyWounded : public Position {
    public:
        using Position::Position;
    };

    class Stunned : public Position {
    public:
        using Position::Position;
    };

    class Sleeping : public Position {
    public:
        using Position::Position;
    };

    class Resting : public Position {
    public:
        using Position::Position;
    };

    class Sitting : public Position {
    public:
        using Position::Position;
    };

    class Fighting : public Position {
    public:
        using Position::Position;
    };

    class Standing : public Position {
    public:
        using Position::Position;
    };


};