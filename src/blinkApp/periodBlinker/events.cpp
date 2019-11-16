
/*
 * Event methods of PeriodedBlinker
 */
#include "periodBlinker.h"

// Superclass
#include "../../smartBlinker/smartBlinker.h"


#include "blinkPeriod/blinkPeriod.h"
#include "blinkPeriod/morningBlinkPeriod.h"


#include "../../day/day.h"
#include "../../powerMgr/powerMgr.h"
#include "../../moment.h"
#include "../../day/sunEvent/ConfirmedSunEvent.h"


// msp430Drivers
#include <softFault/softFault.h>



void
PeriodedBlinker::init() {
    // Show ignorance of day attributes (sunrise time).
    // Show ignorance of any sunrise/sunset events
    Day::init();
}



/*
 * On leaving KeepAlive state.
 * Resume detecting sun.
 */
void PeriodedBlinker::onPowerGoodAtDay() {
    scheduleCheckSunsetTask();
}

void PeriodedBlinker::onPowerGoodAtNight() {
    scheduleCheckSunriseTask();
}







void PeriodedBlinker::onSunriseDetected() {
    /*
     * Tell Day.
     * (It might flag that sunrise was detected, but not capture time until later.)
     */
    Day::onSunriseDetected();

    /*
     * Reset low pass filter for new signal: sunset instead of sunrise
     */
    ConfirmedSunEvent::reset();

    SmartBlinker::indicateSunEvent();

    scheduleCheckSunsetTask();
    /// Optional optimization of power:
    /// check sunset much later today
    /// scheduleLaggedCheckSunsetTask();
}



void PeriodedBlinker::onSunsetDetected() {

    Day::onSunsetDetected();
    ConfirmedSunEvent::reset();
    SmartBlinker::indicateSunEvent();

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

void PeriodedBlinker::onPowerForEveningBlinking() {
    BlinkPeriod::initForEveningBlinking();
    scheduleFirstBlinkTaskOfPeriod(Moment::betweenSunsetAndBlinking);
}

void PeriodedBlinker::onPowerForNightBlinking() {
    BlinkPeriod::initForNightBlinking();
    scheduleFirstBlinkTaskOfPeriod(Moment::betweenEveningAndNightBlinking);
}

// Enough power and sunrise valid
void PeriodedBlinker::onPowerForMorningBlinking() {
    // init subperiod
    BlinkPeriod::initForMorningBlinking();
    // init superperiod
    MorningSuperBlinkPeriod::init();
    scheduleFirstMorningBlinkTask();
}



/*
 * Schedule evening blinking from a detected sunset.
 * If power.
 */
void PeriodedBlinker::onEveningBlinkPeriodOver() {
    if (PowerMgr::isPowerForBlinking()) {
            onPowerForNightBlinking();
    }
    else {
        /*
         * No more power tonight.  Skip night and morning blinking.
         * But schedule same tasks as if, i.e. checkSunrise
         */
        scheduleCheckSunriseTask();
    }
}


void PeriodedBlinker::onNightBlinkPeriodOver() {
    /*
     * Schedule morning blinking if we know sunrise.
     *
     * If we cold started recently, we don't know valid sunrise yet
     */
    if (Day::isSunriseTimeValid()) {
        if (PowerMgr::isPowerForBlinking()) {
            onPowerForMorningBlinking();
        }
        else {
            scheduleCheckSunriseTask();
        }
    }
    else {
        /*
         * We don't know when to start, so omit morning blinking.
         * But schedule same tasks as if,  i.e. checkSunrise.
         */
        scheduleCheckSunriseTask();
    }
    // assert some task is scheduled
}


/*
 * OLD In this design, we don't check for sunrise until morning blink is over.
 * That should be soon enough for an advancing sunrise due to seasons,
 * That is also safer: no possiblity of falsely detecting sunrise, then sunset while it is really night.
 *
 * But we check sunrise immediately.
 * If morning blink period overruns, it pushes sunrise out, falsely.
 * Morning blink would continue to overrun, worsening in a feedback loop.
 * Morning blink should check sunrise no less frequently than the usual checkSunrise period.
 */
void PeriodedBlinker::onMorningBlinkPeriodOver() {

    if (SmartBlinker::isDayLight()) {
        /*
         * Morning blinking has overrun sunrise.
         * Begin filtered sunrise detect.
         * Not schedule another morning blink, schedule check sunrise again.
         */
        SmartBlinker::feedDaylightEventToFilter();
        scheduleCheckSunriseTask();
    }
    else {
        // If last morning blink sub period or it was terminated prematurely
        if (MorningSuperBlinkPeriod::isDone())
        {
            scheduleCheckSunriseTask();
        }
        else
        {
            // Schedule another morning blink subperiod
            BlinkPeriod::initForMorningBlinking();
            scheduleSubsequentMorningBlinkTask();
        }
    }
    // assert some task is scheduled
}

