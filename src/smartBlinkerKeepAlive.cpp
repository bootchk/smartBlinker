
#include "smartBlinker.h"

#include "periodedBlinker/periodedBlinker.h"

#include "powerMgr.h"

#include "moment.h"

// embeddedDutyCycle
#include <OS/taskScheduler.h>






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
            PeriodedBlinker::onPowerGoodAtNight();
        }
        else {
            PeriodedBlinker::onPowerGoodAtDay();
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
