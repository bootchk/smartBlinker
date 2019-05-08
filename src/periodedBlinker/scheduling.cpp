/*
 * Scheduling methods
 */
#include "periodedBlinker.h"

#include "../moment.h"

// embeddedDutyCycle
#include <OS/taskScheduler.h>


/*
 * !!!! Cannot use clock, which is not ready.
 */


/*
 * Schedule sun detection
 *
 * Assume it is daylight.
 * Start detecting sunset.
 *
 * Case 1: is already night, the first check sunset will immediately start checking for sunrise.
 * and possibly start blinking.
 *
 * Case 2: is daylight.  Check and continue checking for sunset.
 */
void PeriodedBlinker::scheduleInitialTask() {
    scheduleCheckSunsetTask();
}



/*
 * Blink tasks
 */

void PeriodedBlinker::scheduleBlinkTask() {
    TaskScheduler::scheduleTask(
            blinkTask,
            Moment::betweenBlinks);
}


void PeriodedBlinker::scheduleFirstBlinkTaskOfPeriod(MomentMethodPtr momentFirstBlink) {

    TaskScheduler::scheduleTask(
            blinkTask,
            momentFirstBlink);
}






/*
 * Check sun tasks
 */


void PeriodedBlinker::scheduleCheckSunriseTask()
{
    TaskScheduler::scheduleTask(
            checkSunriseTask,
            Moment::betweenSunChecks);
}

void PeriodedBlinker::scheduleCheckSunsetTask()
{
    TaskScheduler::scheduleTask(
            checkSunsetTask,
            Moment::betweenSunChecks);
}








#ifdef OLD
//Now we schedule by duration, not time.
EpochTime PeriodedBlinker::timeOfMorningBlinkPeriodStart() {
    return Day::timeBeforeNextSunriseBySeconds(Parameters::BetweenMorningBlinkStartAndSunrise);
}
#endif

#ifdef FUTURE
void PeriodedBlinker::scheduleLaggedCheckSunsetTask() {
    TaskScheduler::scheduleTask(1,
                                checkSunsetTask,
                                EpochClock::timeDurationFromNow(900));
}
#endif


#ifdef OLD
// A fixed duration (typically 30 minutes) after now, which is sunset
void PeriodedBlinker::scheduleFirstEveningBlinkTask() {

    TaskScheduler::scheduleTask(
            0,
            blinkTask,
            Parameters::BetweenSunsetAndBlinking);
}

// A fixed duration (typically 1 minute) after now, which is end of evening blinking
void PeriodedBlinker::scheduleFirstNightBlinkTask() {
    // 1 minute from now (from end of evening.)
    TaskScheduler::scheduleTask(
            0,
            blinkTask,
            Parameters::BetweenEveningAndNightBlinking);
}

// A variable duration, which depends on length of night as indicated by saved sunrise time
void PeriodedBlinker::scheduleFirstMorningBlinkTask() {
    TaskScheduler::scheduleTask(
            0,
            blinkTask,
            durationUntilMorningBlinkPeriodStart());
}
#endif
