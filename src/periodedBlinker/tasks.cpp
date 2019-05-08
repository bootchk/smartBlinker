
#include "periodedBlinker.h"
#include "../smartBlinker.h"

#include "../blinkPeriod.h"
#include "../powerMgr.h"



// msp430Drivers
#include <assert/myAssert.h>
#include <softFault/softFault.h>






void PeriodedBlinker::checkBlinkPeriodOverAndScheduleNextTask() {

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
    myAssert(SmartBlinker::isSomeTaskScheduled());
}


/*
 * Check power and terminate blink period prematurely.
 */
void PeriodedBlinker::checkBlinkingPowerExhaustedAndTerminateBlinkPeriod() {
    if (not PowerMgr::isPowerForBlinking()) {
        BlinkPeriod::terminatePrematurely();
        // assert BlinkPeriod::isOver()
    }
}


/*
 * Checking sun and blinking are mutually exclusive.
 */


/*
 * Tasks
 */

void PeriodedBlinker::checkSunriseTask() {
    if (PowerMgr::isNearBrownOut()) {
        SoftFault::info(4);
        SmartBlinker::scheduleKeepAliveTask();
    }
    else{
        if (SmartBlinker::isDay()) {
            onSunriseDetected();
        }
        else {
            // still dark, schedule self again short time later
            scheduleCheckSunriseTask();
        }

        // Use LED *after* we used it for light sensing
        SmartBlinker::blinkLiveness();
    }
    myAssert(SmartBlinker::isSomeTaskScheduled());
}


void PeriodedBlinker::checkSunsetTask() {
    if (PowerMgr::isNearBrownOut()) {
        SoftFault::info(4);
        SmartBlinker::scheduleKeepAliveTask();
    }
    else {
        if (SmartBlinker::isNight()) {
            onSunsetDetected();
        }
        else {
            // still day, schedule self again short time later
            scheduleCheckSunsetTask();
        }

        // Use LED *after* we used it for light sensing
        SmartBlinker::blinkLiveness();
    }
    myAssert(SmartBlinker::isSomeTaskScheduled());
}



void PeriodedBlinker::blinkTask() {

    SmartBlinker::blinkDecorative();

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





