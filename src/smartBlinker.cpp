
#include "smartBlinker.h"

#include "day.h"
#include "parameters.h"
#include "powerMgr.h"



// embeddedDutyCycle
#include <OS/taskScheduler.h>

// MSP430Drivers
#include <LED/led.h>
#include <lightSensor/lightSensor.h>
#include <assert/myAssert.h>






/*
 * Config for integration testing.
 *
 * BLINK_ONLY:  exercise task scheduling but only one task, the blinkTask.
 *
 * NIGHT_ONLY: blink tasks, and detect sun tasks, but always dark.
 * Starts with a forced sunrise and sunset.
 *
 * NORMAL_PERIODS: full test as designed, blink and sun detect tasks, aligned with day
 */
//#define BLINK_ONLY
//#define NIGHT_ONLY
#define NORMAL_PERIODS


void SmartBlinker::scheduleInitialTask() {

#ifdef BLINK_ONLY
    // Start a blink period without waiting for sunset
    BlinkPeriod::initForEveningBlinking();

    /*
     * test blink task by calling it to schedule self
     * Alternatively, could: scheduleBlinkTask();
     */
    blinkTask();
#endif
#if defined(NIGHT_ONLY)
    // Force sunrise.
    // !!! Not onSunriseDetected(), it will schedule sunset detect for 15 minutes later
    // but onSunsetDetected would try schedule task of same kind, and fail
    Day::setSunriseTime();

    // Sunset detect will artificially detect sunset and schedule nightime blinking
    onSunsetDetected();
    myAssert(TaskScheduler::isTaskScheduled());

#endif
#if defined(NORMAL_PERIODS)
    /*
     * Schedule sun detection
     */
    /*
     * Assume it is daylight.
     * Start detecting sunset.
     *
     * Case 1: is already night, the first check sunset will immediately start checking for sunrise.
     * and possibly start blinking.
     *
     * Case 2: is daylight.  Check and continue checking for sunset.
     */
    scheduleCheckSunsetTask();
#endif

    // Some task is scheduled
    myAssert(TaskScheduler::isTaskScheduled());
}

/*
 * Power on reset.
 */
void SmartBlinker::init() {

    // Show we booted.
    // This power spike obscures EnergyTrace low power, so remove it.
    // indicateEvent();

    // Show ignorance of actual day.
    Day::init();

    TaskScheduler::init();
    // Assert no tasks schedule, ready to schedule

    scheduleInitialTask();
}


void SmartBlinker::onAlarm() {
    // Execute the ready task
    TaskScheduler::onAlarm();
}

#ifdef OLD
Old: scheduler knows time instead of duration
EpochTime SmartBlinker::timeToWake() {
    return TaskScheduler::timeOfNextTask();
}
#endif


Duration SmartBlinker::durationUntilWake() { return TaskScheduler::durationUntilNextTask(); }



void SmartBlinker::testTasks() {
    checkSunriseTask();
    checkSunsetTask();
    blinkTask();
}





bool SmartBlinker::isNight() {
    return LightSensor::isDark();
}
