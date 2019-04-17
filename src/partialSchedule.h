
#pragma once

/*
 * A class that knows how to schedule a task at some subsequent time.
 * The duration to the task is unknown and not constant, and requires a running clock.
 * One method is called when the clock is not ready, to indicate desire to schedule.
 * The other method is called when the clock is ready, to actually schedule.
 *
 * A hack.  The task to be scheduled is hardcoded, not general, a specific task for this app.
 *
 * IOW, delayed scheduling.
 * IOW, a partial scheduling, in the partial function sense: the task is known, but not the duration.
 * Since the duration requires clock, not ready yet.
 */

class PartialSchedule {
public:
    static void schedule();
    static void complete();
};
