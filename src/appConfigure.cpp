/*
 * Configuration aspect of App
 */

// API defined by framework
#include <app/app.h>

#include <gpio.h>   // depends on msp430.h   // Pins

#include "ledBlinker.h"

#include "config.h"





void App::configureSleepingGPIO() {
    /*
     * App retains state of an LED during sleep.
     * The LED is off during sleep, but the SoC chip is retaining its off state.
     */
    LEDBlinker::configureGPIO();
}

// prototype on launchpad uses different pins
#ifdef FULL_PROTO_LAUNCHPAD
void App::configureUnusedPinsLowPower() {
    /*
     * Not require LPM5 locked.
     *
     * Only configure pins not connected,
     * since configuring connected pins as output can trigger connected devices.
     *
     * Alternatives for low power:  input with pullup/pulldown or output.
     * Do not leave as input without pull: floats and wastes power.
     */

    // FUTURE optimize: configure entire register at once

    // P1.0 red LED on Launchpad
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    // P1.1 not used or green LED or LEDPWM
    // if launchpad, dark the sourced green led
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);

    // Used
    // P1.2 SPI Slave select (out)
    // P1.3 Alarm (in)

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN4);

    // Used: LEDP
    // GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);

    // Used: LEDN
    // GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
    // SPI data pins
    //GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
    //GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);
    //GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);

    // Only 5 pins on port 3
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN4);

    // Not ensure configuration is effective, since LPM5 might be locked
}
#endif



#ifdef MYPCB_BOARD_R3
void App::configureUnusedPinsLowPower() {

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN4);
    // App Use LEDP
    // GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);
    // App Use LEDN
    // GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    // Framework use P2.2 SPI Slave select (out)
    //GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    // Framework use P2.3 Alarm pin
    // GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
    // Framework Use SPI data pins
    //GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
    //GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);
    //GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);

    // Only 5 pins on port 3.
    // Datasheet says only 3 pins??
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN3);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN4);

    // Not ensure configuration is effective, since LPM5 might be locked
}
#endif



void App::configureUsedPins() {
    // App retains LED state during sleep (but usually off.)
    LEDBlinker::configureGPIO();
}
