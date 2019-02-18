

#include "ledBlinker.h"

// msp430Drivers
#include <LEDAndLightSensor/ledAndLightSensor.h>
#include <timer/timer.h>




/*
 * Implementation uses:
 * - combination LED and light sensor
 * - timer for blink duration
 */

void LEDBlinker::blink()
{
    // Same LED as used to measure light, now used to generate light
    LEDAndLightSensor::toOnFromOff();

    // Budget calcs used 12 mS
    LowPowerTimer::delayTenMilliSeconds();
    //LowPowerTimer::delayTwentyMilliSeconds();

    LEDAndLightSensor::toOffFromOn();
}


void LEDBlinker::configureGPIO() {
    LEDAndLightSensor::toOffFromUnconfigured();
}
