
#include "smartBlinker.h"

#include "ledBlinker.h"
#include "powerMgr.h"
#include "../config.h"

#include <logger/logger.h>





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
