/*
 * Scheduling methods
 */
#include "smartBlinker.h"

#include "moment.h"

// embeddedDutyCycle
#include <OS/taskScheduler.h>


/*
 * !!!! Cannot use clock, which is not ready.
 */






/*
 * Blink tasks
 */

void SmartBlinker::scheduleBlinkTask() {
    TaskScheduler::scheduleTask(
            blinkTask,
            Moment::betweenBlinks);
}


void SmartBlinker::scheduleFirstBlinkTaskOfPeriod(MomentMethodPtr momentFirstBlink) {

    TaskScheduler::scheduleTask(
            blinkTask,
            momentFirstBlink);
}






/*
 * Check sun tasks
 */


void SmartBlinker::scheduleCheckSunriseTask()
{
    TaskScheduler::scheduleTask(
            checkSunriseTask,
            Moment::betweenSunChecks);
}

void SmartBlinker::scheduleCheckSunsetTask()
{
    TaskScheduler::scheduleTask(
            checkSunsetTask,
            Moment::betweenSunChecks);
}

void SmartBlinker::scheduleKeepAliveTask()
{
    TaskScheduler::scheduleTask(
            keepAliveTask,
            Moment::betweenKeepAlive);
}



bool SmartBlinker::isSomeTaskScheduled() { return TaskScheduler::isTaskScheduled(); }





#ifdef OLD
//Now we schedule by duration, not time.
EpochTime SmartBlinker::timeOfMorningBlinkPeriodStart() {
    return Day::timeBeforeNextSunriseBySeconds(Parameters::BetweenMorningBlinkStartAndSunrise);
}
#endif

#ifdef FUTURE
void SmartBlinker::scheduleLaggedCheckSunsetTask() {
    TaskScheduler::scheduleTask(1,
                                checkSunsetTask,
                                EpochClock::timeDurationFromNow(900));
}
#endif


#ifdef OLD
// A fixed duration (typically 30 minutes) after now, which is sunset
void SmartBlinker::scheduleFirstEveningBlinkTask() {

    TaskScheduler::scheduleTask(
            0,
            blinkTask,
            Parameters::BetweenSunsetAndBlinking);
}

// A fixed duration (typically 1 minute) after now, which is end of evening blinking
void SmartBlinker::scheduleFirstNightBlinkTask() {
    // 1 minute from now (from end of evening.)
    TaskScheduler::scheduleTask(
            0,
            blinkTask,
            Parameters::BetweenEveningAndNightBlinking);
}

// A variable duration, which depends on length of night as indicated by saved sunrise time
void SmartBlinker::scheduleFirstMorningBlinkTask() {
    TaskScheduler::scheduleTask(
            0,
            blinkTask,
            durationUntilMorningBlinkPeriodStart());
}
#endif
