
#include "smartBlinker.h"

#include "../ConfirmedSunEvent.h"



// MSP430Drivers
//#include <LED/led.h>
#include <lightSensor/lightSensor.h>

/*
 * The day/night methods.
 */

void SmartBlinker::calibrateLightSensor() {
    LightSensor::calibrateInLightOrReset();
}


bool SmartBlinker::checkIsDaylight() {
    // not dark => putative sunrise
    return ConfirmedSunEvent::doesThisEventConfirm(  not LightSensor::isDark() );
}


void SmartBlinker::feedDaylightEvent() {
    ConfirmedSunEvent::feedDaylightEvent();
}


bool SmartBlinker::checkIsNight() {
    // dark => putative sunset
    return ConfirmedSunEvent::doesThisEventConfirm( LightSensor::isDark() );
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





