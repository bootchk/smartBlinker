
#include "smartBlinker.h"

#include "ledBlinker.h"
#include "powerMgr.h"

#include <logger/logger.h>




/*
 * Stuff we do every time we wake to check the sun.
 */
void SmartBlinker::everySunCheck() {

    /*
     * Show a human observer that we are still alive.
     */
    LEDBlinker::blink();

    /*
     * Check for a brownout condition.
     */
    if (PowerMgr::isNearBrownOut()) {
        Logger::log(4);
    }
}
