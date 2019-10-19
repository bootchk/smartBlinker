#pragma once


#include "sunEventEstimate/sunEventEstimate.h"

// mwp430Drivers
#include <alarmClock/time/timeTypes.h>   // EpochTime



/*
 * Model of day i.e. sideral time, from sun event samples.
 *
 * Knows day events i.e. sunset, sunrise.
 *
 * Also knows whether sunrise was detected in this waking period.
 *
 * !!! Does NOT delineate 24-hour periods.
 * Sunrise time may be the sunrise many days ago.
 *
 * Valid sequence is init(); setSunriseTime(); isSunriseTimeValid=>true; timeOfNextSunriseAfterTime()
 *
 * Illegal to call timeOfNextSunriseAfterTime() when not isSunriseTimeValid()
 *
 * Abstracts two implementations.
 */


enum class SunEventKind {
    Sunrise,
    Sunset
};

enum class SunEventFit {
    Early,
    DoesFit,
    Late,
    ModelInvalid
};



class Day {
private:
    static SunEventEstimate sunriseEstimate;
    static SunEventEstimate sunsetEstimate;


public:
    /*
     * Called at coldstart, not at beginning of each day.
     */
    static void init();

    /*
     * App has detected sun event.
     *
     * A flag that indicates sunrise was detected in this waking period.
     * !!! Does not mean that sunrise time was captured in this waking period, or ever.
     *
     * Necessary because at the time sunrise is detected, clock is not available.
     */
    static void onSunriseDetected();
    static bool wasSunriseDetected();

    static void onSunsetDetected();


    /*
     * Add event to model.
     *
     * Called after sun event detected (possibly earlier in waking period.)
     *
     * One imp:: Read timeNow and store it as sunrise time.
     */
    static void captureSunriseTime();

    /*
     * Do we know sunrise yet?
     *
     * Has setSunriseTime() been called at least once in the past.
     * Does not guarantee that sunrise time is within 24 hours of now, i.e. not far in the past.
     */
    static bool isSunriseTimeValid();

    /*
     * Guarantees that returned time is past the given time
     * (in the future if given time is now) and within the next 24 hours.
     */
    static EpochTime timeOfNextSunriseAfterTime(EpochTime& time);


    // Convenience (no need to pass kind.)
    static Duration durationUntilNextSunriseLessSeconds(Duration lessDuration);

    /*
     * For sun event detected now, is it sane with respect to any known model of seasonal day length?
     *
     * A crude model does not account for seasonal changes.
     * A more elaborate model does.
     *
     *
     */
    static bool doesSunEventFit(SunEventKind);

private:
    static SunEventFit doesSunEventFitModel(SunEventEstimate&);
};
