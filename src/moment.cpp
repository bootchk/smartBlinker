#include "moment.h"

#include "parameters.h"

/*
 * These return constants.
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
}



