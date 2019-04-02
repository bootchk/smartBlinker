
#include "smartBlinker.h"

#include "powerMgr.h"






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
         * There is enough power to check sun.
         */
        if (isNight()) {
             scheduleCheckSunriseTask();
        }
        else {
            scheduleCheckSunsetTask();
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

