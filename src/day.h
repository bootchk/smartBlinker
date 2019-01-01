#pragma once



#include <time/timeTypes.h>   // EpochTime



/*
 * Knows certain times of day transitions i.e. sunset, sunrise
 *
 * Valid sequence is init(); setSunriseTime(); isSunriseTimeValid=>true; timeTwoHoursBeforeSunriseTime()
 *
 * Illegal to call timeTwoHoursBeforeSunriseTime() when not isSunriseTimeValid()
 */

class Day {

public:
    static void init();

    static void setSunriseTime();
    static bool isSunriseTimeValid();

    /*
     * Time before sunrise by given seconds
     */
    static EpochTime timeBeforeNextSunriseBySeconds(Duration);
};
