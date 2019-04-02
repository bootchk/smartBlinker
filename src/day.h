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

    /*
     * Has setSunriseTime() been called at least once in the past.
     * Does not guarantee that sunrise time is within 24 hours of now, i.e. not far in the past.
     */
    static bool isSunriseTimeValid();

    /*
     * Guarantees that returned time is past the given time
     * (in the future if given time is now) and within the next 24 hours.
     */
    static EpochTime timeOfNextSunriseAfterTime(EpochTime& time);



    /*
     * Duration from now til next sunrise less given seconds.
     *
     * Must not be called if now is less than duration from next sunrise.
     */
    static Duration durationUntilNextSunriseLessSeconds(Duration);

};
