
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

    static const Duration BetweenBlinks = 1;

    static const Duration BetweenSunsetAndBlinking = 5;
    static const Duration BetweenEveningAndNightBlinking = 5;
    // Between sunset and evening: 5
    // Evening blinking lasts 3 seconds (3 blinks every 1 seconds)
    // 5 seconds between
    // Night blinking lasts 5 seconds (5 blinks every 1 seconds)
    // 18 seconds total after sunset

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
    static const unsigned int BlinksEvening = 3;
    static const unsigned int BlinksNight = 5;
    static const unsigned int BlinksMorning = 3;


#elif defined( ACCELERATED_TIME_PARAMETERS)

    // See also

    // All duration units: seconds
    static const Duration BetweenBlinks = 1;
    static const Duration BetweenSunsetAndBlinking = 30;
    // Not expected to reach night blinking, should exhaust power first
    static const Duration BetweenEveningAndNightBlinking = 60;// One minute

    static const Duration BetweenSunChecks = 10;

    static const Duration BetweenMorningBlinkStartAndSunrise = 7200;// Two hours

    /*
     * Counts of blinks.
     */
    // Expect only about 10 to exhaust power
    static const unsigned int BlinksEvening = 100;
    static const unsigned int BlinksNight = 100;
    static const unsigned int BlinksMorning = 100;


#elif defined( PRODUCTION_PARAMETERS)
    // Normal, production parameters
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
 *
 * All units centivolts.
 */


#if defined( ACCELERATED_TIME_PARAMETERS)

// Accelerated blinks until power is just above brownout.
// Brownout detected by SW
// Vmon causes actual brownout at 1.7 plus BAT43 diode drop of 0.1 volt

// Vcc which indicates enough charge on storage for LED blinking function.
static const unsigned int MinVccForBlinking = 185; // 1.85V

// Vcc at which brownout might be imminent.
static const unsigned int MinVccForBrownout = 180; // 1.8V

#elif defined(PRODUCTION_PARAMETERS) or defined(COMPRESSED_TIME_PARAMETERS)

// Vcc which indicates enough charge on storage for LED blinking function.
static const unsigned int MinVccForBlinking = 220; // 2.2V

// Vcc at which brownout might be imminent.
static const unsigned int MinVccForBrownout = 180; // 1.8V


#endif


/*
 * Vcc voltage indicates a reserve of power above mcu Vmin (1.8V)
 * Required to finish booting and start app.
 * Defined in board.h, affects embeddedDutyCycle/powerMgr.cpp
 */
//static const unsigned int MinVccForStarting = 190; // centiVolts 1.9V
};

