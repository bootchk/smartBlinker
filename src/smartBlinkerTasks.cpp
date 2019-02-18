

#include "smartBlinker.h"

#include "blinkPeriod.h"
#include "ledBlinker.h"
#include "powerMgr.h"


// msp430Drivers
#include <assert/myAssert.h>






void SmartBlinker::checkBlinkPeriodOver() {

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
void SmartBlinker::checkBlinkingPowerExhausted() {
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
    everySunCheck();

    if ( not isNight() ) {
        onSunriseDetected();
    }
    else {
        // still dark, schedule self again short time later
        scheduleCheckSunriseTask();
    }
}


void SmartBlinker::checkSunsetTask() {
    everySunCheck();

    if ( isNight() ) {
        onSunsetDetected();
    }
    else {
        // still dark, schedule self again short time later
        scheduleCheckSunsetTask();
    }
}



void SmartBlinker::blinkTask() {
    LEDBlinker::blink();

    ///TestMain::blinkForcedGreenLED(5);

    myAssert(BlinkPeriod::isActive());

    BlinkPeriod::advance();

#ifdef ACCELERATED_TIME_PARAMETERS
    checkBlinkingPowerExhausted();
    // blinkPeriod might be over
#endif

    checkBlinkPeriodOver();

    // assert (blinkTask scheduled and not blinkperiod::isOver())
    // or (blinkPeriod::isOver and no blinking related task is scheduled but a checkSunrise task is scheduled)
}




void SmartBlinker::indicateEvent() {
    LEDBlinker::blink();
}

