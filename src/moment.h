

#include <alarmClock/time/timeTypes.h>  // Duration



/*
 * Knows how to calculate moments,
 * i.e. durations til task.
 *
 * Hides that some Durations are not known at time of scheduling (not constants)
 * but must be calculated at time of ready()ing the task.
 *
 * These methods are passed as MethodMomentPtrs to schedule a task.
 * That is, we defer calculation of Duration until task is ready()ed.
 *
 * They all return immutable values, only some are constants.
 */

class Moment {
public:
    /*
     * These return constants, but to satisfy MomentMethodPtr, they are not declared such.
     */
    static Duration betweenBlinks(void);
    static Duration betweenDarkChecks(void);
    static Duration betweenSunChecks(void);
    static Duration betweenKeepAlive(void);
    static Duration betweenSunsetAndBlinking(void);
    static Duration betweenEveningAndNightBlinking(void);

    /*
     * This returns a varying Duration.
     */
    static Duration untilMorningBlinkPeriodStart(void);

    static Duration untilSubsequentMorningBlinkPeriodStart(void);
};


