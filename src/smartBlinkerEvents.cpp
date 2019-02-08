
#include <src/blinkPeriod.h>
#include <src/day.h>
#include <src/powerMgr.h>
#include <src/smartBlinker.h>




/*
 * Events
 */




void SmartBlinker::onSunriseDetected() {
    Day::setSunriseTime();

    indicateEvent();

    scheduleCheckSunsetTask();
    /// Optional optimization of power:
    /// check sunset much later today
    /// scheduleLaggedCheckSunsetTask();
}



void SmartBlinker::onSunsetDetected() {
    // We don't record sunset

    indicateEvent();

    if (PowerMgr::isPowerForBlinking()) {
        onPowerForEveningBlinking();
        // check sunrise much later tonight
        // scheduleLaggedCheckSunriseTask();
        // OR after morning blink period
    }
    else {
        // Not enough power to blink, check sunrise all night long
        scheduleCheckSunriseTask();
        // No blinking task
    }
    // assert blinking task or checkSunriseTask is scheduled
}





/*
 * We determined there is enough power for a blink period.
 */

void SmartBlinker::onPowerForEveningBlinking() {
    BlinkPeriod::initForEveningBlinking();
    scheduleFirstEveningBlinkTask();
}

void SmartBlinker::onPowerForNightBlinking() {
    BlinkPeriod::initForNightBlinking();
    scheduleFirstNightBlinkTask();
}

void SmartBlinker::onPowerForMorningBlinking() {
    BlinkPeriod::initForMorningBlinking();
    scheduleFirstMorningBlinkTask();
}



/*
 * We schedule evening blinking from a detected sunset.
 * We can only schedule morning blinking if there exists a prior detected sunrise.
 * On startup, might not exist.
 */

void SmartBlinker::onEveningBlinkPeriodOver() {
    /*
     * Schedule night blinking if power.
     */

    if (PowerMgr::isPowerForBlinking()) {
            onPowerForNightBlinking();
    }
    else {
        /*
         * No more power tonight.
         * But we must schedule same tasks as if we had done morning blinking,
         * i.e. checkSunrise
         */
        scheduleCheckSunriseTask();
    }
}


void SmartBlinker::onNightBlinkPeriodOver() {
    /*
     * Schedule morning blinking if possible.
     *
     * If we cold started recently, we have not detected a sunrise yet
     */
    if (Day::isSunriseTimeValid()) {
        if (PowerMgr::isPowerForBlinking()) {
            onPowerForMorningBlinking();
        }
    }
    else {
        /*
         * We don't know when to start, so omit morning blinking.
         * But we must schedule same tasks as if we had done morning blinking,
         * i.e. checkSunrise
         */
        scheduleCheckSunriseTask();
    }
}


void SmartBlinker::onMorningBlinkPeriodOver() {
    /*
     * In this design, we don't check for sunrise until morning blink is over.
     * That should be soon enough for an advancing sunrise due to seasons,
     * That is also safer: no possiblity of falsely detecting sunrise, then sunset while it is really night.
     */
    scheduleCheckSunriseTask();
}

