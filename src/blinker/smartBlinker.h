
// msp430Drivers
#include <alarmClock/time/timeTypes.h>  // EpochTime

// embeddedDutyCycle
#include <OS/task.h>    // MomentMethodPtr



/*
 * App: blink decoratively when dark and power permits
 * or blink keep alive
 *
 * Framework for essential behaviour (detecting light and blinking.)
 * Uses a set of tasks and events that further defines behaviour.
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


    static bool isNight();
    static bool isDay();


    // ultimate goal: blink for entertainment
    static void blinkDecorative();
    // blink to show system still live
    static void blinkLiveness();

    static void indicateEvent();

    /*
     * Test harness to call all tasks to find unresolved symbols.
     */
    static void testTasks();
};
