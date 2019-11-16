
// msp430Drivers
#include <alarmClock/time/timeTypes.h>  // EpochTime

// embeddedDutyCycle
#include <OS/task.h>    // MomentMethodPtr



/*
 * Capability (essential behaviour) is: blink decoratively when dark and power permits or blink keep alive.
 * Framework for managing power, detecting light, and blinking.
 *
 * Thus every SmartBlinker uses:
 * - LEDBlinker
 * - LightSensor
 * - PowerMgr
 *
 * Not every SmartBlinker understands Day, which is more specialized than "light conditions."
 *
 * Uses a BlinkStrategy (set of tasks and events) that further defines behaviour.
 */

class SmartBlinker {

public:

    static void scheduleInitialTask();

    /*
     * API of generic app
     */
    static void init();
    static void onAlarm(void);
    static void configureGPIO();


    static EpochTime timeToWake();
    static Duration durationUntilWake();

    // Tasks not dependent on strategy
    static bool transitionToKeepAlive();
    static void keepAliveTask();
    static void scheduleKeepAliveTask();


    // Is some kind of task scheduled?
    static bool isSomeTaskScheduled();

    ///static void scheduleLaggedCheckSunsetTask();


    // Resets if not in light.
    static void calibrateLightSensor();




    /*
     * Simple checks of light conditions.
     * Does not use a low-pass filter.
     * Means nothing about the Day model.
     */
    static bool isNightDark();
    static bool isDayLight();





    // ultimate goal: blink for entertainment
    static void blinkDecorativeBright();
    static void blinkDecorativeDim();

    // Other indicateions (may be blinks): optional, dim
    static void indicateSunCheck();
    static void indicateSunEvent();

    /*
     * Test harness to call all tasks to find unresolved symbols.
     */
    static void testTasks();
};
