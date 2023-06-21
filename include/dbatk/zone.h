#pragma once
#include "dbatk/base.h"
#include <map>
#include <bitset>

namespace dbat {

    struct reset_com {
        reset_com() = default;
        explicit reset_com(const nlohmann::json& j);
        char command;   /* current command                      */

        bool if_flag;    /* if TRUE: exe only if preceding exe'd */
        int arg1;        /*                                      */
        int arg2;        /* Arguments to the command             */
        int arg3;        /*                                      */
        int arg4;        /* room_max  default 0			*/
        int arg5;           /* percentages variable                 */
        std::string sarg1;        /* string argument                      */
        std::string sarg2;        /* string argument                      */
        nlohmann::json serialize();

        /*
         *  Commands:              *
         *  'M': Read a mobile     *
         *  'O': Read an object    *
         *  'G': Give obj to mob   *
         *  'P': Put obj in obj    *
         *  'G': Obj to char       *
         *  'E': Obj to char equip *
         *  'D': Set state of door *
         *  'T': Trigger command   *
             *  'V': Assign a variable *
        */
    };

    class Zone {
    public:
        Zone() = default;
        explicit Zone(const nlohmann::json& j);
        std::string name, builders;
        int lifespan, reset_mode, min_level, max_level;
        double age;
        std::size_t id, bot, top;
        std::bitset<4> flags;
        std::vector<reset_com> cmds;
        nlohmann::json serialize();

    };

    extern std::unordered_map<std::size_t, Zone> zones;

}