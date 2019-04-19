

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
 */

class Moment {
public:
    static Duration betweenBlinks(void);
    static Duration betweenSunChecks(void);
    static Duration betweenKeepAlive(void);
    static Duration betweenSunsetAndBlinking(void);
    static Duration betweenEveningAndNightBlinking(void);

    static Duration untilMorningBlinkPeriodStart(void);
};


