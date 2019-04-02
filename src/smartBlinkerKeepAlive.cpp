
#include "smartBlinker.h"

#include "ledBlinker.h"
#include "powerMgr.h"
#include "../config.h"

#include <logger/logger.h>



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





#ifdef OLD
/*
 * Stuff we do every time we wake to check the sun.
 */
void SmartBlinker::everySunCheck() {

#ifdef SUN_CHECK_BLINK_LIVENESS    /*
     * Show a human observer that we are still alive.
     */
    LEDBlinker::blink();
#endif

    /*
     * Check for a brownout condition.
     */
    if (PowerMgr::isNearBrownOut()) {
        Logger::log(4);
    }
}
#endif
