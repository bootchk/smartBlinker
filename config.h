

/*
 * Configures the app
 */


// See other config in smartBlinker.cpp, for testing



// For testing
/*
 * Normal, real time parameters
 */
//#define PRODUCTION_PARAMETERS
/*
 * Normal operation, but compressed time.  For bench testing using hand and Launchpad power.
 */
//#define COMPRESSED_TIME_PARAMETERS
/*
 * Quasi normal operation, also compressed. For stand-alone testing solar powered PCB.
 * Capacitor is smaller to accelerate events.
 */
#define ACCELERATED_TIME_PARAMETERS



/*  Choose board design:
 *
 * - PROTO_BOARD: prototype on launchpad  with GPIO configured different from MYPCB (obsolete
 *
 * - MYPCB_BOARD: custom PCB
 *
 * - FULL_PROTO_LAUNCHPAD  launchpad connected with proto boards full emulation of MYPCB (RTC breakout and LED breakout)
 * !!! but with slightly different connections since Launchpad does not bring out P3.0
 *
 * !!! There are multiple revisions of the PCB
 *
 * This affects:
 * - board.h
 * - appConfigure.cpp
 */

/// Choose one
///#define PROTO_BOARD
//#define FULL_PROTO_LAUNCHPAD
//#define MYPCB_BOARD_R3
#define MYPCB_BOARD_R5



/*
 * Whether to blink LED every check sun to indicate liveness
 */
#define BLINK_ON_SUN_CHECK


/*
 * Whether to blink LED when sunrise and sunset detected.
 */
//#define BLINK_ON_SUN_EVENT



/*
 * Whether to use SunriseEstimator
 */
#define USE_SUNRISE_ESTIMATOR




/*
 * Whether to use PeriodedBlinker
 */
#define USE_PERIODED_BLINKER
#ifdef USE_PERIODED_BLINKER
#define BlinkStrategy PeriodedBlinker
#else
#endif

/*
 * Whether isNight() always returns false (forego use of light sensor.)
 * A hack for testing, light sensor uses power, is sensitive to parameters, etc.
 */
//#define OMIT_LIGHT_SENSOR

