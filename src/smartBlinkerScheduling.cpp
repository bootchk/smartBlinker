/*
 * Scheduling methods
 */
#include "smartBlinker.h"
#include "parameters.h"
#include "day.h"


// embeddedDutyCycle
#include <OS/taskScheduler.h>

// msp430Drivers
#include <realTimeClock/epochClock/epochClock.h>





#ifdef OLD
//Now we schedule by duration, not time.
EpochTime SmartBlinker::timeOfMorningBlinkPeriodStart() {
    return Day::timeBeforeNextSunriseBySeconds(Parameters::BetweenMorningBlinkStartAndSunrise);
}
#endif



Duration SmartBlinker::durationUntilMorningBlinkPeriodStart() {
#ifdef ACCELERATED_TIME_PARAMETERS
    return Parameters::BetweenEveningAndNightBlinking;
#else
    return Day::durationUntilNextSunriseLessSeconds(Parameters::BetweenMorningBlinkStartAndSunrise);
#endif
}

/*
 * Task of kind 0 i.e. blink tasks
 */

void SmartBlinker::scheduleBlinkTask() {
    TaskScheduler::scheduleTask(
            0,
            blinkTask,
            Parameters::BetweenBlinks);
}



// FUTURE combine these into one parameterized method scheduleFirstBlinkTask

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




/*
 * Tasks of kind 1 i.e. check sun tasks
 */


void SmartBlinker::scheduleCheckSunriseTask()
{
    TaskScheduler::scheduleTask(
            1,
            checkSunriseTask,
            Parameters::BetweenSunChecks);
}

void SmartBlinker::scheduleCheckSunsetTask()
{
    TaskScheduler::scheduleTask(
            1,
            checkSunsetTask,
            Parameters::BetweenSunChecks);
}

// FUTURE Scheduled with different frequency as sun checks
void SmartBlinker::scheduleKeepAliveTask()
{
    TaskScheduler::scheduleTask(
            1,
            keepAliveTask,
            Parameters::BetweenSunChecks);
}



bool SmartBlinker::isSomeTaskScheduled() { return TaskScheduler::isTaskScheduled(); }




#ifdef FUTURE
void SmartBlinker::scheduleLaggedCheckSunsetTask() {
    TaskScheduler::scheduleTask(1,
                                checkSunsetTask,
                                EpochClock::timeDurationFromNow(900));
}
#endif
