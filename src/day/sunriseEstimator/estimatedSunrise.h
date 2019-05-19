#pragma once

// msp430Drivers
#include <alarmClock/time/timeTypes.h>



/*
 * Estimated statistically
 */
class EstimatedSunrise {
public:
    static void init();
    static void captureSunriseTime();
    static bool isSunriseTimeValid();

    /*
     * Duration til next estimated sunrise.
     * Zero if next sunrise is closer than lessDuration from now.
     */
    static Duration durationUntilNextSunriseLessSeconds(Duration lessDuration);
};
