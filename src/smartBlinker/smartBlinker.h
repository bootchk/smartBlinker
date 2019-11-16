
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
     * Check if sun event occurred.
     * Side effects to ConfirmedSunEvent.
     *
     * Returns true if:
     * - event occurred and is confirmed by low pass filter
     * - is sane with model (fits valid model or model is invalid)
     */
    static bool checkIsSunset();
    static bool checkIsSunrise();

    /*
     * Simple checks of light conditions.
     * Does not use a low-pass filter.
     * Means nothing about the Day model.
     */
    static bool isNightDark();
    static bool isDayLight();


    // Called when we detect sun event abnormally, i.e. during moring blink that overruns
    // TODO Or when detect light after sunset.
    static void feedDaylightEventToFilter();


    // ultimate goal: blink for entertainment
    static void blinkDecorative();

    // Other indicateions (may be blinks): optional, dim
    static void indicateSunCheck();
    static void indicateSunEvent();

    /*
     * Test harness to call all tasks to find unresolved symbols.
     */
    static void testTasks();
};
