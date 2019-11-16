#include "moment.h"

#include "../parameters.h"



/*
 * These return constants.
 *
 * Note C++ invokes default constructor to Duration
 */


// Common to all strategies
Duration Moment::betweenBlinks(void) { return Parameters::BetweenBlinks; }

// Strategy PeriodedBlinker
Duration Moment::betweenSunChecks(void) { return Parameters::BetweenSunChecks; }
Duration Moment::betweenKeepAlive(void) { return Parameters::BetweenKeepAlive; }
Duration Moment::betweenSunsetAndBlinking(void) { return Parameters::BetweenSunsetAndBlinking; }
Duration Moment::betweenEveningAndNightBlinking(void) { return Parameters::BetweenEveningAndNightBlinking; }





// Strategy DarkBlinker
Duration Moment::betweenDarkBlinkerDarkChecks(void) { return Parameters::BetweenDarkBlinkerDarkChecks; }

#include "../day/day.h"


/*
 * Return varying value.
 *
 * !!! If use clock, clock must be ready.
 * Which is why schedule stores MomentMethods, that are called later when clock is ready.
 */
Duration Moment::untilMorningBlinkPeriodStart() {
    // TESTING, certain accelerated: return Parameters::BetweenEveningAndNightBlinking;

    return Day::durationUntilNextSunriseLessSeconds(Parameters::BetweenMorningBlinkStartAndSunrise);


    /*
     * Duration can be zero, meaning sunrise is so imminent that we can't schedule a moment before it less seconds.
     * Caller must handle that.
     * Not easy to undo our decision to schedule it, effectively task is scheduled imminently.
     * And the task itself must decide it is scheduled inappropriately.
     */
}


Duration Moment::untilSubsequentMorningBlinkPeriodStart(void) {
    // Keep blinking at same period as prior morning blink period.
    return Parameters::BetweenBlinks;
}
