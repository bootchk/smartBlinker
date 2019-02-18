/*
 * Scheduling methods
 */
#include "smartBlinker.h"
#include "parameters.h"


// embeddedDutyCycle
#include <OS/taskScheduler.h>

// msp430Drivers
#include <realTimeClock/epochClock/epochClock.h>





/*
 * Task of kind 0 i.e. blink tasks
 */

void SmartBlinker::scheduleBlinkTask() {
    TaskScheduler::scheduleTask(
            0,
            blinkTask,
            EpochClock::timeDurationFromNow(Parameters::BetweenBlinks));
}






void SmartBlinker::scheduleFirstEveningBlinkTask() {
    // 30 minutes after sunset
    TaskScheduler::scheduleTask(
            0,
            blinkTask,
            EpochClock::timeDurationFromNow(
                    Parameters::BetweenSunsetAndBlinking));
}
void SmartBlinker::scheduleFirstNightBlinkTask() {
    // 1 minute from now (from end of evening.)
    TaskScheduler::scheduleTask(
            0,
            blinkTask,
            EpochClock::timeDurationFromNow(
                    Parameters::BetweenEveningAndNightBlinking));
}

void SmartBlinker::scheduleFirstMorningBlinkTask() {
    TaskScheduler::scheduleTask(
            0,
            blinkTask,
            timeOfMorningBlinkPeriodStart());
}




/*
 * Tasks of kind 1 i.e. check sun tasks
 */


void SmartBlinker::scheduleCheckSunriseTask()
{
    TaskScheduler::scheduleTask(
            1,
            checkSunriseTask,
            EpochClock::timeDurationFromNow(Parameters::BetweenSunChecks));
}

void SmartBlinker::scheduleCheckSunsetTask()
{
    TaskScheduler::scheduleTask(
            1,
            checkSunsetTask,
            EpochClock::timeDurationFromNow(Parameters::BetweenSunChecks));
}



#ifdef FUTURE
void SmartBlinker::scheduleLaggedCheckSunsetTask() {
    TaskScheduler::scheduleTask(1,
                                checkSunsetTask,
                                EpochClock::timeDurationFromNow(900));
}
#endif
