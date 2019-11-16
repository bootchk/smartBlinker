#pragma once


/*
 * Set of tasks and events for blinking behaviour:
 * - whenever dark
 *
 * Blinking is dim and takes little power.
 * If it is light, we don't blink.  There should be <enough light> to both check light conditions and build up a reserve of power.
 * Usually light is indirect sunlight or strong artificial light.
 * Not designed to succeed where light is not <enough light>.
 *
 * Checking light conditions power budget: TODO
 *
 * We check light conditions every ten seconds.
 * If you wake in the dark, it should be blinking.
 * If you turn on the lights, it should stop blinking within ten seconds.
 * If you turn the lights back off, it should start blinking within ten seconds.
 *
 * FUTURE adaptive:  if it was dark within the last five minutes but light now,
 * check for dark more frequently so blinking will resume in a short time.
 */


class DarkBlinker {
public:
    static void init();

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
