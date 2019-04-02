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
EpochTime SmartBlinker::timeOfMorningBlinkPeriodStart() {
    return Day::timeBeforeNextSunriseBySeconds(Parameters::BetweenMorningBlinkStartAndSunrise);
}
#endif

Duration SmartBlinker::durationUntilMorningBlinkPeriodStart() {
    return Day::durationUntilNextSunriseLessSeconds(Parameters::BetweenMorningBlinkStartAndSunrise);
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






void SmartBlinker::scheduleFirstEveningBlinkTask() {
    // 30 minutes after sunset
    TaskScheduler::scheduleTask(
            0,
            blinkTask,Parameters::BetweenSunsetAndBlinking);
}
void SmartBlinker::scheduleFirstNightBlinkTask() {
    // 1 minute from now (from end of evening.)
    TaskScheduler::scheduleTask(
            0,
            blinkTask,
            Parameters::BetweenEveningAndNightBlinking);
}

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

// TODO Scheduled with different frequency as sun checks
void SmartBlinker::scheduleKeepAliveTask()
{
    TaskScheduler::scheduleTask(
            1,
            keepAliveTask,
            Parameters::BetweenSunChecks);
}



#ifdef FUTURE
void SmartBlinker::scheduleLaggedCheckSunsetTask() {
    TaskScheduler::scheduleTask(1,
                                checkSunsetTask,
                                EpochClock::timeDurationFromNow(900));
}
#endif
