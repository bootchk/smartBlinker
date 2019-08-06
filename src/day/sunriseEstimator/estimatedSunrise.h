#pragma once

// msp430Drivers
#include <alarmClock/time/timeTypes.h>



/*
 * Estimated statistically from a time series of samples.
 * I.E. a model of the sinusoidal sunlight signal over many days.
 */

class EstimatedSunrise {
public:
    // Called once at coldstart
    static void init();

    /*
     * With a confirmed sun event (sunrise), convert to a time.
     */
    static EpochTime getSunriseTimeSampleForConfirmedSunEvent();

    /*
     * With a confirmed sun event (sunrise), convert to time and feed to model.
     */
    static void captureSunriseTime();

    /*
     * Is the model of sunrise valid (enough samples, consistent)
     */
    static bool isSunriseTimeValid();

    /*
     * Duration til next estimated sunrise.
     * Zero if next sunrise is closer than lessDuration from now.
     */
    static Duration durationUntilNextSunriseLessSeconds(Duration lessDuration);
};
