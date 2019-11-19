#include "smartBlinker.h"
#include "../ledBlinker/ledBlinker.h"

#include "../../config.h"



/*
 * Methods that use LED
 */


void SmartBlinker::blinkDecorativeBright() {
    LEDBlinker::blinkBright();
}

void SmartBlinker::blinkDecorativeDim() {
    LEDBlinker::blinkDim();
}

void
SmartBlinker::blinkDecorativeDimmest() {\
    LEDBlinker::blinkVisibleCloserRange();
}

void SmartBlinker::indicateSunCheck() {
#ifdef BLINK_ON_SUN_CHECK
    LEDBlinker::blinkDim();
#endif
}


void SmartBlinker::indicateSunEvent() {
#ifdef BLINK_ON_SUN_EVENT
    LEDBlinker::blinkDim();
#endif
}
