/*
 * Morning blink period comprises a count of sub periods.
 *
 * This is super to sub periods.
 *
 * Each sub period checks for sunrise; if light terminates the super period.
 */


class MorningSuperBlinkPeriod {
public:
    static void init();

    // Cease all morning blink sub periods, one was terminated for lack of power
    static void terminate();

    // Side effect: advance count
    static bool isDone();
};
