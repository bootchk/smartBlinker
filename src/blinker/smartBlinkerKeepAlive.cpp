
#include "smartBlinker.h"

//#include "../periodedBlinker/periodedBlinker.h"
#include "../darkBlinker/darkBlinker.h"

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
         * There is enough power to check sun.
         */
        if (isNight()) {
            //PeriodedBlinker::onPowerGoodAtNight();
            DarkBlinker::onPowerGoodAtNight();
        }
        else {
            //PeriodedBlinker::onPowerGoodAtDay();
            DarkBlinker::onPowerGoodAtDay();
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
