

#include "ledBlinker.h"

// msp430Drivers
#include <LEDAndLightSensor/ledAndLightSensor.h>
#include <timer/timer.h>



/*
 * Formerly depended on config.h for ACCELERATED_TIME_PARAMETERS
 *
 * Now, the brightnesses are not configurable.
 * If you want different brightness, configure the caller, not the implementation.
 *
   #ifdef ACCELERATED_TIME_PARAMETERS
    // 2 millisecond is easily visible if staring from close distance
    LowPowerTimer::delayTwoMilliSeconds();
   #else
 */


/*
 * Implementation uses:
 * - combination LED and light sensor
 * - timer for blink duration
 */

void LEDBlinker::blinkBright()
{
    // Same LED as used to measure light, now used to generate light
    LEDAndLightSensor::toOnFromOff();


    // Budget calcs used 12 mS
    // Designed to be visible for hundreds of meters even if not staring
    LowPowerTimer::delayTenMilliSeconds();
    //LowPowerTimer::delayTwentyMilliSeconds();


    LEDAndLightSensor::toOffFromOn();
}



void LEDBlinker::blinkDim() {
    LEDAndLightSensor::toOnFromOff();
    /*
     * This is probably the absolute minimum.
     * Some say a 6uS blink is visible.
     */
    LowPowerTimer::delayTicksOf100uSec(1);
    LEDAndLightSensor::toOffFromOn();
}



/*
 * Can't use LowPowerTimer, its min is 100uSec
 */
void
LEDBlinker::blinkVisibleCloserRange() {
    LEDAndLightSensor::toOnFromOff();
    // @ 1Mhz MCU clock, ~ 10uSec
    __delay_cycles(10);
    LEDAndLightSensor::toOffFromOn();
}

void LEDBlinker::configureGPIO() {
    LEDAndLightSensor::toOffFromUnconfigured();
}
