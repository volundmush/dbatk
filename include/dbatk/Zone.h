#pragma once
#include "kaizermud/base.h"
#include <map>
#include <bitset>

namespace kaizermud::dbat {

    struct reset_com {
        char command;   /* current command                      */

        bool if_flag;    /* if TRUE: exe only if preceding exe'd */
        int arg1;        /*                                      */
        int arg2;        /* Arguments to the command             */
        int arg3;        /*                                      */
        int arg4;        /* room_max  default 0			*/
        int arg5;           /* percentages variable                 */
        std::string sarg1{""};        /* string argument                      */
        std::string sarg2{""};        /* string argument                      */

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
        std::string_view name, builders;
        int lifespan, age, reset_mode, min_level, max_level;
        ObjectID id, bot, top;
        std::bitset<36> flags;
        std::vector<reset_com> cmds;

    };

    std::map<ObjectID, Zone> zones;
    std::map<int, std::string_view> zoneFlags;

}