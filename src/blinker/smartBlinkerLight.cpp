
#include "smartBlinker.h"

#include "../day/sunEvent/ConfirmedSunEvent.h"
#include "../day/day.h"



// MSP430Drivers
#include <lightSensor/lightSensor.h>

/*
 * The day/night methods.
 */

void SmartBlinker::calibrateLightSensor() {
    LightSensor::calibrateInLightOrReset();
}


bool SmartBlinker::checkIsDaylight() {
    bool result;


    bool isLight =  LightSensor::isLight();

    // Feed event to low-pass filter.
    bool isConfirmed = ConfirmedSunEvent::doesThisEventConfirm( isLight );

    // If not low-pass filtered
    if (isConfirmed) {
        // Is sane with respect to model of sunrise
        result = Day::isSunEventSane();
    }
    else {
        // Seems like daylight, but too early.  Keep checking for daylight
        // We should eventually find seeming daylight closer to modeled sunrise.
        result = false;
    }

    return result;
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





