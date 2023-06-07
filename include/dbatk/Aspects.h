#pragma once
#include "kaizermud/Aspects.h"

namespace dbat::aspect {

    struct DBATAspect : public kaizer::Aspect {

    };

    // Character Aspects

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