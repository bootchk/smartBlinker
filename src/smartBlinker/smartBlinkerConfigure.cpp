
#include "smartBlinker.h"

#include "../ledBlinker/ledBlinker.h"



/*
 * SmartBlinker
 * - used LED during wake
 * - retains LED state during sleep (usually off)
 *
 * By design, LED is off during sleep, but it need not be.
 * You could have a signal on a GPIO that is on during sleep,
 * e.g. to hold some other chip in a certain state.
 */
void
SmartBlinker::configureGPIO() {
    LEDBlinker::configureGPIO();
}
