/*
 * Scheduling methods
 */
#include "darkBlinker.h"
#include "../blinker/smartBlinker.h"

#include "../moment.h"
#include "../powerMgr.h"

// embeddedDutyCycle
#include <OS/taskScheduler.h>

// msp430Drivers
#include <assert/myAssert.h>





void DarkBlinker::scheduleInitialTask() {
    scheduleCheckDarkTask();
}


void DarkBlinker::scheduleCheckDarkTask() {
    TaskScheduler::scheduleTask(
                checkDarkTask,
                Moment::betweenDarkChecks);
}

void DarkBlinker::scheduleBlinkTask() {
    TaskScheduler::scheduleTask(
                DarkBlinker::blinkTask,
                Moment::betweenBlinks);
}






void DarkBlinker::checkDarkTask() {
    if ( not SmartBlinker::transitionToKeepAlive()) {
        if (SmartBlinker::checkIsNight()) {
                   onDarkDetected();
        }
        else {
            // still day, schedule self again short time later
            scheduleCheckDarkTask();
        }

        // Use LED *after* we used it for light sensing
        // SmartBlinker::blinkLiveness();
    }
    myAssert(SmartBlinker::isSomeTaskScheduled());
}


void DarkBlinker::blinkTask() {
    if (   (not PowerMgr::isPowerForBlinking())
            or SmartBlinker::checkIsDaylight()
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
        SmartBlinker::blinkDecorative();
        scheduleBlinkTask();
    }

}





/*
 * When dark, don't immediately blink, but schedule.
 */
void DarkBlinker::onDarkDetected() {
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

