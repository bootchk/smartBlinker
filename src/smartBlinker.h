
#include <time/timeTypes.h>  // EpochTime




/*
 * App:
 * - blink in two periods, after sunset and before sunset
 * - detect sunrise and sunset
 *
 * A set of tasks and events
 */

class SmartBlinker {
private:
    static void scheduleInitialTask();

    static void checkBlinkPeriodOverAndScheduleNextTask();

    static void checkBlinkingPowerExhaustedAndTerminateBlinkPeriod();

public:
    /*
     * API of generic app
     */
    static void init();
    static void onAlarm(void);
    static void configureGPIO();


    static EpochTime timeToWake();
    static Duration durationUntilWake();

    /*
     * Scheduled tasks
     */
    static void checkSunriseTask();
    static void checkSunsetTask();
    static void blinkTask();
    static void keepAliveTask();

    /*
     * Test harness to call all tasks to find unresolved symbols.
     */
    static void testTasks();

    /*
     * Events
     * Determined by tasks, from external events.
     */
    static void onSunriseDetected();
    static void onSunsetDetected();

    static void onPowerForEveningBlinking();
    static void onPowerForNightBlinking();
    static void onPowerForMorningBlinking();

    static void onEveningBlinkPeriodOver();
    static void onNightBlinkPeriodOver();
    static void onMorningBlinkPeriodOver();


    /*
     * Scheduling
     */
    static void scheduleBlinkTask();
    static void scheduleCheckSunriseTask();
    static void scheduleCheckSunsetTask();
    static void scheduleKeepAliveTask();

    static void scheduleFirstBlinkTaskOfPeriod(const Duration durationUntilFirstBlink);
    //static void scheduleFirstEveningBlinkTask();
    //static void scheduleFirstNightBlinkTask();
    //static void scheduleFirstMorningBlinkTask();

    // Is some kind of task scheduled?
    static bool isSomeTaskScheduled();

    ///static void scheduleLaggedCheckSunsetTask();


    /*
     * Timing etc
     */
    static EpochTime timeOfMorningBlinkPeriodStart();
    static Duration durationUntilMorningBlinkPeriodStart();


    /*
     * Resets if not in light.
     */
    static void calibrateLightSensor();
    static bool isNight();

    static void blinkLiveness();
    static void indicateEvent();
};
