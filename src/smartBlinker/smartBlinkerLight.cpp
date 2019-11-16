




// MSP430Drivers
#include <lightSensor/lightSensor.h>
#include <src/smartBlinker/smartBlinker.h>

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

bool SmartBlinker::isDayLight() {
    return not isNightDark();
}



bool SmartBlinker::isNightDark() {
#ifdef OMIT_LIGHT_SENSOR
    return false;
#else
    return LightSensor::isDark();
#endif
}





