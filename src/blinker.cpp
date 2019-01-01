
#include <LEDAndLightSensor/ledAndLightSensor.h>
#include <src/blinker.h>
// Use low power timer
#include <timer/timer.h>


void Blinker::blink()
{
    // Same LED as used to measure light, now used to generate light
    LEDAndLightSensor::toOnFromOff();

    LowPowerTimer::delayTwentyMilliSeconds();

    LEDAndLightSensor::toOffFromOn();
}
