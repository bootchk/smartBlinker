#pragma once

// embeddedDutyCycle
#include <OS/task.h>    // MomentMethodPtr


/*
 * Set of tasks and events for blinking behaviour:
 * - in defined periods.
 * - in two periods, after sunset and before sunset
 * - detect sunrise and sunset
 */


class PeriodedBlinker {
public:
    static void init();

    static void scheduleInitialTask();


    static void checkBlinkPeriodOverAndScheduleNextTask();

    static void checkBlinkingPowerExhaustedAndTerminateBlinkPeriod();

    /*
     * Scheduled tasks
     */
    static void checkSunriseTask();
    static void checkSunsetTask();
    static void blinkTask();


    /*
     * Events
     * Determined by tasks, from external events.
     */

    // Events from KeepAlive task
    static void onPowerGoodAtNight();
    static void onPowerGoodAtDay();

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

    static void scheduleFirstBlinkTaskOfPeriod(MomentMethodPtr momentMethod);

    static void scheduleFirstMorningBlinkTask();
    static void scheduleSubsequentMorningBlinkTask();

    //static void scheduleFirstEveningBlinkTask();
    //static void scheduleFirstNightBlinkTask();
};
