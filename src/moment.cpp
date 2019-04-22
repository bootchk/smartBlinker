#include "moment.h"

#include "parameters.h"



/*
 * These return constants.
 *
 * Note C++ invokes default constructor to Duration
 */
Duration Moment::betweenBlinks(void) { return Parameters::BetweenBlinks; }
Duration Moment::betweenSunChecks(void) { return Parameters::BetweenSunChecks; }
Duration Moment::betweenKeepAlive(void) { return Parameters::BetweenKeepAlive; }
Duration Moment::betweenSunsetAndBlinking(void) { return Parameters::BetweenSunsetAndBlinking; }
Duration Moment::betweenEveningAndNightBlinking(void) { return Parameters::BetweenEveningAndNightBlinking; }



#include "day.h"


/*
 * Return varying value.
 *
 * !!! If use clock, clock must be ready.
 * Which is why schedule stores MomentMethods, that are called later when clock is ready.
 */
Duration Moment::untilMorningBlinkPeriodStart() {

#ifdef ACCELERATED_TIME_PARAMETERS
    return Parameters::BetweenEveningAndNightBlinking;
#else
    return Day::durationUntilNextSunriseLessSeconds(Parameters::BetweenMorningBlinkStartAndSunrise);
#endif

    /*
     * Duration can be zero, meaning sunrise is so imminent that we can't schedule a moment before it less seconds.
     * Caller must handle that.
     * Not easy to undo our decision to schedule it, effectively task is scheduled imminently.
     * And the task itself must decide it is scheduled inappropriately.
     */
}



