

#include "smartBlinker.h"

#include "blinkPeriod.h"
#include "ledBlinker.h"
#include "powerMgr.h"



// msp430Drivers
#include <assert/myAssert.h>
#include <logger/logger.h>






void SmartBlinker::checkBlinkPeriodOverAndScheduleNextTask() {

    if (BlinkPeriod::isOver()) {
        if (BlinkPeriod::isEvening()) {
            onEveningBlinkPeriodOver();
            // first blink task of next period (if any) is scheduled
        }
        else if (BlinkPeriod::isNight()) {
            onNightBlinkPeriodOver();
        }
        else {
            // else morning blink subperiod over,  blink period over, no blinkTask scheduled
            onMorningBlinkPeriodOver();
        }
        // assert another blinkPeriod is scheduled OR checkSunrise task is scheduled
    }
    else {
        scheduleBlinkTask();
        // next blink task of period is scheduled
    }
    /*
     * assert is scheduled:
     * - another blinkPeriod
     * - OR next blink task in this period
     * - OR checkSunrise task
     */
}


/*
 * Check power and terminate blink period prematurely.
 */
void SmartBlinker::checkBlinkingPowerExhaustedAndTerminateBlinkPeriod() {
    if (not PowerMgr::isPowerForBlinking()) {
        BlinkPeriod::terminatePrematurely();
        // assert BlinkPeriod::isOver()
    }
}


/*
 * Checking sun and blinking are mutually exclusive.
 *
 */


/*
 * Tasks
 */

void SmartBlinker::checkSunriseTask() {
    if (PowerMgr::isNearBrownOut()) {
        Logger::log(4);
        scheduleKeepAliveTask();
    }
    else{

#ifdef SUN_CHECK_BLINK_LIVENESS
    LEDBlinker::blink();
#endif

        if (not isNight()) {
            onSunriseDetected();
        }
        else {
            // still dark, schedule self again short time later
            scheduleCheckSunriseTask();
        }
    }
    // assert some task scheduled
}


void SmartBlinker::checkSunsetTask() {
    if (PowerMgr::isNearBrownOut()) {
        Logger::log(4);
        scheduleKeepAliveTask();
    }
    else {

        if (isNight()) {
            onSunsetDetected();
        }
        else {
            // still dark, schedule self again short time later
            scheduleCheckSunsetTask();
        }
    }
}



void SmartBlinker::blinkTask() {

    // main task: blink
    LEDBlinker::blink();

    ///TestMain::blinkForcedGreenLED(5);

    myAssert(BlinkPeriod::isActive());

    BlinkPeriod::advance();

    /*
     * Try hard not to exhaust power, cold start is perilous.
     */
    checkBlinkingPowerExhaustedAndTerminateBlinkPeriod();
    // blinkPeriod might be over

    checkBlinkPeriodOverAndScheduleNextTask();

    // assert (blinkTask scheduled and not blinkperiod::isOver())
    // or (blinkPeriod::isOver and no blinking related task is scheduled but a checkSunrise task is scheduled)
}




void SmartBlinker::indicateEvent() {
    LEDBlinker::blink();
}

