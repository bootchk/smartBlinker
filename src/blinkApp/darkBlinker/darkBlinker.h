#pragma once


/*
 * Set of tasks and events for blinking behaviour:
 * - whenever dark
 */


class DarkBlinker {
public:
    static void scheduleInitialTask();

    /*
     * Scheduled tasks
     */
    static void checkDarkTask();
    static void blinkTask();


    /*
     * Events
     * Determined by tasks, from external events.
     */

    // Events from KeepAlive task
    static void onPowerGoodAtNight();
    static void onPowerGoodAtDay();

    // Internal events
    static void onDarkDetected();


    /*
     * Scheduling
     */
    static void scheduleBlinkTask();
    static void scheduleCheckDarkTask();
};
