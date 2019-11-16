/*
 * All (scheduling, tasks, etc.)  methods
 */

#include "darkBlinker.h"

// Superclass
#include <src/smartBlinker/smartBlinker.h>


#include "../../moment/moment.h"
#include "../../powerMgr/powerMgr.h"

// embeddedDutyCycle
#include <OS/taskScheduler.h>

// msp430Drivers
#include <assert/myAssert.h>




void
DarkBlinker::init() {
    // Nothing to init
}


void DarkBlinker::scheduleInitialTask() {
    scheduleCheckDarkTask();
}


void DarkBlinker::scheduleCheckDarkTask() {
    TaskScheduler::scheduleTask(
                checkDarkTask,
                Moment::betweenDarkBlinkerDarkChecks);
}

void DarkBlinker::scheduleBlinkTask() {
    TaskScheduler::scheduleTask(
                DarkBlinker::blinkTask,
                Moment::betweenBlinks);
}






void DarkBlinker::checkDarkTask() {
    if ( not SmartBlinker::transitionToKeepAlive()) {
        if (SmartBlinker::isNightDark()) {
                   onDarkDetected();
        }
        else {
            // still light, schedule self again short time later
            scheduleCheckDarkTask();
        }

        // Use LED *after* we used it for light sensing
        // SmartBlinker::blinkLiveness();
    }
    myAssert(SmartBlinker::isSomeTaskScheduled());
}


void DarkBlinker::blinkTask() {
    if (   (not PowerMgr::isPowerForBlinking())
            or SmartBlinker::isDayLight()
       ) {
        /*
         * Don't blink, and wait for dark again.
         *
         * It was dark, and if still dark, unlikely that power will recover.
         */
        scheduleCheckDarkTask();
    }
    else {
        // Still dark, and plenty of power
        SmartBlinker::blinkDecorativeDim();
        scheduleBlinkTask();
    }
}





/*
 * When dark, don't immediately blink, but schedule.
 */
void DarkBlinker::onDarkDetected() {
    // FUTURE scheduleInitialBlinkTask()  that filters out very brief dark detect
    scheduleBlinkTask();
}

/*
 * On leaving KeepAlive state.
 * Resume detecting dark.
 */
void DarkBlinker::onPowerGoodAtDay() {
    scheduleCheckDarkTask();
}

void DarkBlinker::onPowerGoodAtNight() {
    scheduleCheckDarkTask();
}

