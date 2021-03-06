
#include "periodBlinker.h"

#include "../../moment/moment.h"

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


/*
 * Schedule for a long while from now, wee hours of morning.
 *
 * If moment is zero, blinking periods are longer than night duration?
 * Could be true at high latitudes.
 */
void PeriodedBlinker::scheduleFirstMorningBlinkTask() {
    scheduleFirstBlinkTaskOfPeriod(Moment::untilMorningBlinkPeriodStart);
}

/*
 * Schedule immediately following current morning blink period.
 */
void PeriodedBlinker::scheduleSubsequentMorningBlinkTask() {
    scheduleFirstBlinkTaskOfPeriod(Moment::untilSubsequentMorningBlinkPeriodStart);
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

// A variable duration, which depends on length of night via saved sunrise time
void PeriodedBlinker::scheduleFirstMorningBlinkTask() {
    TaskScheduler::scheduleTask(
            0,
            blinkTask,
            durationUntilMorningBlinkPeriodStart());
}
#endif
