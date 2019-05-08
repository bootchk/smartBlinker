
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


/*
 * Nieve implementation, without confirmation.
 * Is subject to ephemeral weather events.
 */
#ifdef OLD



bool SmartBlinker::isDay() {
    return not isNight();
}



bool SmartBlinker::isNight() {
#ifdef OMIT_LIGHT_SENSOR
    return false;
#else
    return LightSensor::isDark();
#endif
}

#else


bool SmartBlinker::isDay() {
    // not dark => putative sunrise
    return ConfirmedSunEvent::doesThisEventConfirm(  not LightSensor::isDark() );
}



bool SmartBlinker::isNight() {
    // dark => putative sunset
    return ConfirmedSunEvent::doesThisEventConfirm( LightSensor::isDark() );
}

#endif

