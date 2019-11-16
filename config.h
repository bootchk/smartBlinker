

/*
 * Configures the app
 */


// See other config in smartBlinker.cpp, for testing



// For testing
/*
 * PRODUCTION: Normal, real time parameters
 * COMPRESSED: Normal operation, but compressed time.  For bench testing using hand and Launchpad power.
 * ACCELERATED: Quasi normal operation, also compressed. For stand-alone testing solar powered PCB.
 * Capacitor might be smaller to accelerate events.
 *
 * Uncomment exactly one
 */
//#define PRODUCTION_PARAMETERS
//#define COMPRESSED_TIME_PARAMETERS
#define ACCELERATED_TIME_PARAMETERS


/*
 * Choose kind of BlinkerApp
 */
#define USE_PERIOD_BLINKER
//#define USE_DARK_BLINKER


// Define macro giving a class name
#ifdef USE_PERIOD_BLINKER
#define BlinkStrategy PeriodedBlinker
#elif defined USE_DARK_BLINKER
#define BlinkStrategy DarkBlinker
#else
#error "Kind of app not chosen"
#endif


/*  Choose board design:
 *
 * - PROTO_BOARD: prototype on launchpad  with GPIO configured different from MYPCB (obsolete
 *
 * - MYPCB_BOARD: custom PCB !!! There are multiple revisions of the PCB
 *
 * - FULL_PROTO_LAUNCHPAD  launchpad connected with proto boards full emulation of MYPCB (RTC breakout and LED breakout)
 * !!! but with possibly different connections since Launchpad does not bring out P3.0
 *
 * This affects:
 * - board.h
 * - appConfigure.cpp
 *
 * Uncomment exactly one.
 */
//#define PROTO_BOARD
//#define FULL_PROTO_LAUNCHPAD
//#define MYPCB_BOARD_R3  // Obsolete PCB
//#define MYPCB_BOARD_R5  // Obsolete, solar cell obsolete
#define BLINKERBQ_BOARD








/*
 * Whether to blink LED every check sun to indicate liveness.
 * When on launchpad, not required, waking is obvious on debugger probe LED
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
 * Whether isNight() always returns false (forego use of light sensor.)
 * A hack for testing, light sensor uses power, is sensitive to parameters, etc.
 */
//#define OMIT_LIGHT_SENSOR

