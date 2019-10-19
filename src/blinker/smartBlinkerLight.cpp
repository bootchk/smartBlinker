
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


/*
 * Pair of similar functions
 */
bool SmartBlinker::checkIsSunrise() {
    bool result;

    bool isLight =  LightSensor::isLight();

    // Feed event to low-pass filter.
    bool isConfirmed = ConfirmedSunEvent::doesThisEventConfirm( isLight );

    // If not low-pass filtered
    if (isConfirmed) {
        // Is sane with respect to model of sunrise
        result = Day::doesSunEventFit(SunEventKind::Sunrise);
        /*
         * When result false, Seems like daylight, but too early.  Keep checking for daylight.
         * We should eventually find seeming daylight closer to modeled sunrise.
         */
    }
    else {
        // Not confirmed, wait for confirmation (two signals in a row.)
        result = false;
    }

    return result;
}


bool SmartBlinker::checkIsSunset() {
    bool result;

    bool isDark =  LightSensor::isDark();
    bool isConfirmed = ConfirmedSunEvent::doesThisEventConfirm( isDark );

    if (isConfirmed) {
            // Is sane with respect to model of sunset
            result = Day::doesSunEventFit(SunEventKind::Sunset);
    }
    else {
            result = false;
    }

    return result;
}


void SmartBlinker::feedDaylightEventToFilter() {
    ConfirmedSunEvent::feedDaylightEvent();
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





