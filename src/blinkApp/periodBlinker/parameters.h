
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
 * Note a schedule of one second really takes two seconds (Lamport's rule adds an average of half second.)
 */
#include "config.h"   // chooses time acceleration

#include "../../timeConstants.h"




/*
 * !!! This is now parameters only for PeriodBlinker
 *
 * See also day.h for parameters about Day
 * See also circularBuffer.h for parameters about SunEventModel (size of sample)
 */

// TODO lots of cruft here



// class Parameters {
// public:

namespace Parameters {



    //static constexpr Duration TwentyFourHours = {86400};
    //static const Duration TwentyFourHours(86400);  // most vexing parse
    //static const Duration TwentyFourHours= Duration(86400);
    //static constexpr Duration TwentyFourHours {Duration{86400}};


// Eight subperiods each same time as checkSun duration (15 minutes) equals 2 hours total
static constexpr unsigned int CountMorningBlinkPeriods = 8;








#ifdef COMPRESSED_TIME_PARAMETERS
    // Most durations very short so time compressed

    static constexpr Duration BetweenBlinks = 1;
    static const unsigned int BetweenKeepAlive = 3;


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

#define REALLY_ACCELERATED
#ifdef REALLY_ACCELERATED

// Testing w unlimited power on Launchpad, but full integration test with sunrises and morning blinking using SunriseEstimator

    static const unsigned int BetweenBlinks = 1;
    static const unsigned int BetweenKeepAlive = 3;


static const unsigned int BetweenSunChecks = 3;

// Blink Period parameters

static const unsigned int BetweenSunsetAndBlinking = 3;
// Not expected to reach night blinking, should exhaust power first
static const unsigned int BetweenEveningAndNightBlinking = 1;

// Enough time for entire morning blink: eight blinks (8 at 2 seconds = 16 seconds)
static const unsigned int BetweenMorningBlinkStartAndSunrise = 20;

// Counts of blinks.
// Expect only about 10 to exhaust power with small Cstor
static const unsigned int BlinksEvening = 3;
static const unsigned int BlinksNight = 1;
// 1 blink per subperiod times 8 subperiods
static const unsigned int BlinksMorningSubperiod = 1;



// TODO why are some durations long and others just ints?




#else

    static const unsigned int BetweenSunChecks = 3;
    static const unsigned int BetweenKeepAlive = 3;


    // Blink Period parameters
    static const unsigned int BetweenBlinks = 2;
    static const unsigned int BetweenSunsetAndBlinking = 10;  // 30
    // Not expected to reach night blinking, should exhaust power first
    static const unsigned int BetweenEveningAndNightBlinking = 10;  // 60

    // For accelerated testing, this doesn't matter since we will force sunrise early
    static const unsigned int BetweenMorningBlinkStartAndSunrise = 7200;// Two hours

    // Counts of blinks.
    // Expect only about 10 to exhaust power with small Cstor
    static const unsigned int BlinksEvening = 8;
    static const unsigned int BlinksNight = 5;
    static const unsigned int BlinksMorning = 3;

    // Average over seasons of length of day (not length of daylight, includes night.)
    static constexpr unsigned long int SunrisePeriod = 300;    // 5 minutes
    static constexpr unsigned long int SunriseDelta = 30; // 30 seconds
#endif // REALLY_ACCELERATED






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
    static constexpr unsigned int BetweenKeepAlive = ONE_HOUR;







// Since detection takes two sun checks, will be fifteen minutes after sunset
static constexpr unsigned int BetweenSunsetAndBlinking = ONE_MINUTE;
static constexpr unsigned int BetweenEveningAndNightBlinking = ONE_MINUTE;

static constexpr unsigned int BetweenSunChecks = FIFTEEN_MINUTES;

// Allow fifteen extra minutes beyond expected blink time of two hours
static constexpr unsigned int BetweenMorningBlinkStartAndSunrise = TWO_HOURS + FIFTEEN_MINUTES;






// Evening and night blink for 2 hours every 10 seconds
// 6 ticks/minute * 60 minutes/hour * x hours
static constexpr unsigned int BlinksEvening = 6 * 60 * 2;

static constexpr unsigned int BlinksNight = 6 * 60 * 2;

// Morning blink for 2 hours every 10 seconds (6 * 60 * 2 = 720 total)
// 8 subperiods of 15 minutes (90 blinks) is 720 total
// !!!
static constexpr unsigned int BlinksMorningSubperiod = 6 * 15;

#endif








};

