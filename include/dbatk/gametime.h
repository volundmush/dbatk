#pragma once

#include "dbatk/base.h"

namespace dbat {
    /* This structure is purely intended to be an easy way to transfer */
    /* and return information about time (real or mudwise).            */
    // Note that it's tracked in 24-hour format. so hours might be 20, 21, 22, 23, 0, 1, 2, 3, 4, 5, 6, 7....
    struct time_info_data {
        double seconds;
        int minutes, hours, day, month;
        int16_t year;
    };

    struct weather_data {
        int pressure;    /* How is the pressure ( Mb ) */
        int change;    /* How fast and what way does it change. */
        int sky;    /* How is the sky. */
        int sunlight;    /* And how much sun. */
    };

    // One second in reality is one minute in game time.
    constexpr double timeAccelFactor = 60.0;
    constexpr int secsPerMinute = 60;
    constexpr int minutesPerHour = 60;
    constexpr int hoursPerDay = 24;
    constexpr int daysPerWeek = 7;
    constexpr int daysPerMonth = 30;
    constexpr int monthsPerYear = 12;


    extern weather_data weather_info;
    extern time_info_data time_info;
}