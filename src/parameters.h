
#pragma once

// msp430Drivers
//#include <alarmClock/time/timeTypes.h>

/*
 * Parameters of application (smart blinking)
 *
 * Two sets:
 * - normal
 * - for testing in compressed time
 *
 */
#include "config.h"   // chooses







// class Parameters {
// public:
namespace Parameters {



    //static constexpr Duration TwentyFourHours = {86400};
    //static const Duration TwentyFourHours(86400);  // most vexing parse
    //static const Duration TwentyFourHours= Duration(86400);
    //static constexpr Duration TwentyFourHours {Duration{86400}};
static constexpr unsigned long int TwentyFourHours = 86400;




#ifdef COMPRESSED_TIME_PARAMETERS
    // Most durations very short so time compressed

    static constexpr Duration BetweenBlinks = 1;

    static constexpr Duration BetweenSunsetAndBlinking = 5;
    static constexpr Duration BetweenEveningAndNightBlinking = 5;
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
    static constexpr Duration BetweenMorningBlinkStartAndSunrise = TwentyFourHours - 30;

    // Check sun every 15-30 seconds
    static constexpr Duration BetweenSunChecks = 10;


    // counts for periods
    static const unsigned int BlinksEvening = 3;
    static const unsigned int BlinksNight = 5;
    static const unsigned int BlinksMorning = 3;


#elif defined( ACCELERATED_TIME_PARAMETERS)

    // See also

    // All duration units: seconds
    static const unsigned int BetweenBlinks = 2;
    static const unsigned int BetweenSunsetAndBlinking = 10;  // 30
    // Not expected to reach night blinking, should exhaust power first
    static const unsigned int BetweenEveningAndNightBlinking = 10;  // 60

    static const unsigned int BetweenSunChecks = 3;

    // For accelerated testing, this doesn't matter since we will force sunrise early
    static const unsigned int BetweenMorningBlinkStartAndSunrise = 7200;// Two hours

    static const unsigned int BetweenKeepAlive = 3;

    static const unsigned int BetweenDarkChecks = 10;

    /*
     * Counts of blinks.
     */
    // Expect only about 10 to exhaust power with small Cstor
    static const unsigned int BlinksEvening = 8;
    static const unsigned int BlinksNight = 5;
    static const unsigned int BlinksMorning = 3;

    // SunriseEstimator
    // Average over seasons of length of day (not length of daylight, includes night.)
    static constexpr unsigned long int SunrisePeriod = 300;    // 5 minutes
    static constexpr unsigned long int SunriseDelta = 30; // 30 seconds


#elif defined( PRODUCTION_PARAMETERS)
    // Normal, production parameters

#ifdef OLD
    static constexpr Duration BetweenBlinks = {10};
    static constexpr Duration BetweenSunsetAndBlinking = {1800};  // Thirty minutes
    static constexpr Duration BetweenEveningAndNightBlinking = {60}; // One minute

    // Check sun every 3 minutes
    ///static constexpr Duration BetweenSunChecks = 120};
    static constexpr Duration BetweenSunChecks = {900};  // Fifteen minutes

    static constexpr Duration BetweenMorningBlinkStartAndSunrise = {7200};    // Two hours

    static constexpr Duration BetweenKeepAlive = {3600};  // hour

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

static constexpr unsigned int BetweenBlinks = 10;
static constexpr unsigned int BetweenKeepAlive = 3600;  // hour
static constexpr unsigned int BetweenDarkChecks = 300;  // 5 minutes


static constexpr unsigned int BetweenSunsetAndBlinking = 1800;  // Thirty minutes
static constexpr unsigned int BetweenEveningAndNightBlinking = 60; // One minute

// Check sun every 3 minutes
///static constexpr unsigned int BetweenSunChecks = 120};
static constexpr unsigned int BetweenSunChecks = 900;  // Fifteen minutes

static constexpr unsigned int BetweenMorningBlinkStartAndSunrise = 7200; // Two hours

// SunriseEstimator
// Average over seasons of length of day (not length of daylight, includes night.)
static constexpr unsigned long int SunrisePeriod = 86400;    // 24 hours
static constexpr unsigned long int SunriseDelta = 1800; // half hour

/*
 * Counts of blinks.
 */
// Evening blink for 2 hours every 10 seconds
// 6 ticks/minute * 60 minutes/hour * 4 hours
static constexpr unsigned int BlinksEvening = 6 * 60 * 2;

static constexpr unsigned int BlinksNight = 6 * 60 * 2;

// Morning blink for 2 hours every 10 seconds
static constexpr unsigned int BlinksMorning = 6 * 60 * 2;

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
static const unsigned int MinVccForBlinking = 200; //185; // 1.85V

// Vcc at which brownout might be imminent.
static const unsigned int MinVccForBrownout = 190; // 180 // 1.8V

#elif defined(PRODUCTION_PARAMETERS) or defined(COMPRESSED_TIME_PARAMETERS)

// Vcc which indicates enough charge on storage for LED blinking function.
static const unsigned int MinVccForBlinking = 220; // 2.2V

// Vcc at which brownout might be imminent.
static const unsigned int MinVccForBrownout = 190; // 1.9V


#endif


/*
 * Vcc voltage indicates a reserve of power above mcu Vmin (1.8V)
 * Required to finish booting and start app.
 * Defined in board.h, affects embeddedDutyCycle/powerMgr.cpp
 */
//static const unsigned int MinVccForStarting = 190; // centiVolts 1.9V
};

