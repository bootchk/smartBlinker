#include "moment.h"

#include "../../config.h"



#ifdef USE_DARK_BLINKER
#include "../blinkApp/darkBlinker/parameters.h"
#elif defined(USE_PERIOD_BLINKER)
#include "../blinkApp/periodBlinker/parameters.h"
#else
#errror "Kind of blinker app not defined"
#endif

/*
 * These return constants.
 *
 * Note C++ invokes default constructor to Duration
 */





// Common to all strategies
Duration Moment::betweenBlinks(void) { return Parameters::BetweenBlinks; }
Duration Moment::betweenKeepAlive(void) { return Parameters::BetweenKeepAlive; }


#ifdef USE_DARK_BLINKER
// Strategy DarkBlinker
Duration Moment::betweenDarkBlinkerDarkChecks(void) { return Parameters::BetweenDarkBlinkerDarkChecks; }
#endif



#ifdef USE_PERIOD_BLINKER

// Strategy PeriodedBlinker
Duration Moment::betweenSunChecks(void) { return Parameters::BetweenSunChecks; }
Duration Moment::betweenSunsetAndBlinking(void) { return Parameters::BetweenSunsetAndBlinking; }
Duration Moment::betweenEveningAndNightBlinking(void) { return Parameters::BetweenEveningAndNightBlinking; }





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

#endif
