

#include "ledBlinker.h"

// msp430Drivers
#include <LEDAndLightSensor/ledAndLightSensor.h>
#include <timer/timer.h>

#include "parameters.h"




/*
 * Implementation uses:
 * - combination LED and light sensor
 * - timer for blink duration
 */

void LEDBlinker::blink()
{
    // Same LED as used to measure light, now used to generate light
    LEDAndLightSensor::toOnFromOff();



#ifdef ACCELERATED_TIME_PARAMETERS
    // 2 millisecond is easily visible if staring from close distance
    LowPowerTimer::delayTwoMilliSeconds();
#else
    // Budget calcs used 12 mS
    // Designed to be visible for hundreds of meters even if not staring
    LowPowerTimer::delayTenMilliSeconds();
    //LowPowerTimer::delayTwentyMilliSeconds();
#endif

    LEDAndLightSensor::toOffFromOn();
}


void LEDBlinker::configureGPIO() {
    LEDAndLightSensor::toOffFromUnconfigured();
}
