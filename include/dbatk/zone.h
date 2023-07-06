#pragma once
#include <map>
#include <bitset>
#include "dbatk/base.h"


namespace dbat {

    struct reset_com {
        reset_com() = default;
        explicit reset_com(const nlohmann::json& j);
        char command{};   /* current command                      */

        bool if_flag{};    /* if TRUE: exe only if preceding exe'd */
        int arg1{};        /*                                      */
        int arg2{};        /* Arguments to the command             */
        int arg3{};        /*                                      */
        int arg4{};        /* room_max  default 0			*/
        int arg5{};           /* percentages variable                 */
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
        int lifespan{10}, reset_mode{2}, min_level{0}, max_level{200};
        double age{0.0};
        std::size_t id, bot, top;
        std::bitset<4> flags;
        std::vector<reset_com> cmds;
        nlohmann::json serialize();
        void reset();
        void onHeartbeat(double deltaTime);

        /*
     * Reset mode:
     *   0: Don't reset, and don't update age.
     *   1: Reset if no PC's are located in zone.
     *   2: Just reset.
     */
    };

    extern std::map<std::size_t, Zone> zones;

}