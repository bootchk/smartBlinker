
#pragma once

// msp430Drivers
#include <time/timeTypes.h>

/*
 * Parameters of application (smart blinking)
 *
 * Two sets:
 * - normal
 * - for testing in compressed time
 *
 */
#include "config.h"   // chooses




class Parameters {
public:

    static const Duration TwentyFourHours = 86400;


#ifdef COMPRESSED_TIME_PARAMETERS
    // Most durations very short so time compressed

    static const Duration BetweenBlinks = 3;
    static const Duration BetweenSunsetAndBlinking = 10;
    // Evening blinking lasts 15 seconds (5 blinks every 3 seconds)

    /*
     * Start morning one minute after fake sunrise,sunset.
     * Because of other built-in delays, allow much greater than 40 seconds
     *
     * Next sunrise is 24 hours from fake sunrise.
     * So 24 hours - 30 seconds is a large duration from next sunrise, only 30 seconds from previou
     */
    static const Duration BetweenMorningBlinkStartAndSunrise = TwentyFourHours - 30;

    // Check sun every 15-30 seconds
    static const Duration BetweenSunChecks = 10;


    // counts for periods
    static const unsigned int BlinksEvening = 5;
    static const unsigned int BlinksMorning = 5;


#else
    static const Duration BetweenBlinks = 10;
    static const Duration BetweenSunsetAndBlinking = 1800;  // Thirty minutes
    static const Duration BetweenEveningAndNightBlinking = 60; // One minute

    // Check sun every 3 minutes
    ///static const Duration BetweenSunChecks = 120;
    static const Duration BetweenSunChecks = 900;  // Fifteen minutes

    static const Duration BetweenMorningBlinkStartAndSunrise = 7200;    // Two hours

    /*
     * Counts of blinks.
     */
    // Evening blink for 2 hours every 10 seconds
    // 6 ticks/minute * 60 minutes/hour * 4 hours
    static const unsigned int BlinksEvening = 6 * 60 * 2;

    static const unsigned int BlinksNight = 6 * 60 * 2;

    // Morning blink for 2 hours every 10 seconds
    static const unsigned int BlinksMorning = 6 * 60 * 2;
#endif







/*
 * Voltages.
 * Depends on:
 * - power supply Vccmax (3.3V on Launchpad, 3.6V on PCB)
 * - sunlight detector
 */


/*
 * Vcc voltage indicates full charge on storage.
 * Required for LED blinking function.
 */
static const unsigned int MinVccForBlinking = 280; // 2.8V
// 300;  // centiVolts, 3V


/*
 * Vcc voltage indicates a reserve of power above mcu Vmin (1.8V)
 * Required to finish booting and start app.
 */
//static const unsigned int MinVccForStarting = 190; // centiVolts 1.9V
};

