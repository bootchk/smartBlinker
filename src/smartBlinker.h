
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

    static void everySunCheck();

    static void checkBlinkPeriodOver();

    static void checkBlinkingPowerExhausted();

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

    static void scheduleFirstEveningBlinkTask();
    static void scheduleFirstNightBlinkTask();
    static void scheduleFirstMorningBlinkTask();

    ///static void scheduleLaggedCheckSunsetTask();


    /*
     * Timing etc
     */
    static EpochTime timeOfMorningBlinkPeriodStart();
    static Duration durationUntilMorningBlinkPeriodStart();


    static bool isNight();
    static void indicateEvent();
};
