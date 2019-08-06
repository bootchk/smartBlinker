
#include "smartBlinker.h"

#include "../day/sunEvent/ConfirmedSunEvent.h"



// MSP430Drivers
#include <lightSensor/lightSensor.h>

/*
 * The day/night methods.
 */

void SmartBlinker::calibrateLightSensor() {
    LightSensor::calibrateInLightOrReset();
}


bool SmartBlinker::checkIsDaylight() {
    /*
     * If daylight, feed event to filter.
     */
    bool isLight =  LightSensor::isLight();
    return ConfirmedSunEvent::doesThisEventConfirm( isLight );
    // TODO Here should do a sanity check
}


void SmartBlinker::feedDaylightEvent() {
    ConfirmedSunEvent::feedDaylightEvent();
}


bool SmartBlinker::checkIsNight() {
    bool isDark =  LightSensor::isDark();
    return ConfirmedSunEvent::doesThisEventConfirm( isDark );
}




/*
 * Nieve implementation, without confirmation.
 * Is subject to ephemeral weather events.
 */

bool SmartBlinker::isDaylight() {
    return not isNight();
}



bool SmartBlinker::isNight() {
#ifdef OMIT_LIGHT_SENSOR
    return false;
#else
    return LightSensor::isDark();
#endif
}





