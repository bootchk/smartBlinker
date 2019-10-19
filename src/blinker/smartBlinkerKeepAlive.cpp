
#include "smartBlinker.h"

#include "config.h"

#ifdef USE_PERIODED_BLINKER
#include "../periodedBlinker/periodedBlinker.h"
#else

#include "../darkBlinker/darkBlinker.h"
#endif

#include "../powerMgr.h"
#include "../moment.h"

// embeddedDutyCycle
#include <OS/taskScheduler.h>

// msp430Drivers
#include <assert/myAssert.h>
#include <softFault/softFault.h>






/*
 * Power was near brownout.
 * We are only waking to check power level.
 * If power is restored, schedule sun check.
 * Typically we exhaust power during the night.
 * It is possible that we missed sunrise and that it is already day again.
 */
void SmartBlinker::keepAliveTask() {
    if (not PowerMgr::isNearBrownOut()) {
        /*
         * There is enough power to check sunlight.
         */
        if (isNightDark()) {
            BlinkStrategy::onPowerGoodAtNight();
        }
        else {
            BlinkStrategy::onPowerGoodAtDay();
        }
    }
    else {
        /*
         * Still near brownout: schedule self task again.
         */
        scheduleKeepAliveTask();
    }
    // assert some task scheduled
}


void SmartBlinker::scheduleKeepAliveTask()
{
    TaskScheduler::scheduleTask(
            keepAliveTask,
            Moment::betweenKeepAlive);
}


bool SmartBlinker::transitionToKeepAlive() {
    bool result = false;

    if (PowerMgr::isNearBrownOut()) {
            SoftFault::info(4);
            SmartBlinker::scheduleKeepAliveTask();
            result = true;
    }
    return result;
}
