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
         * Check if sun event occurred.
         * Side effects to ConfirmedSunEvent.
         *
         * Returns true if:
         * - event occurred and is confirmed by low pass filter
         * - is sane with model (fits valid model or model is invalid)
         */
        static bool checkIsSunset();
        static bool checkIsSunrise();

        // Called when we detect sun event abnormally, i.e. during moring blink that overruns
        // TODO Or when detect light after sunset.
        static void feedDaylightEventToFilter();

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
