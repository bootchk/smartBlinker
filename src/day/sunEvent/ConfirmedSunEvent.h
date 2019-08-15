#pragma once

/*
 * A simple low-pass filter.
 * Two consecutive events passes the filter (confirmed.)
 * One event followed by a negative event is filtered out.
 *
 * TT=>T
 * TF=>F
 * FT=>F
 *
 * TTT=>T
 * OLD: TTT=>F, since filter resets itself.
 * NEW: TTResetT=>F, caller must explicitly reset filter.
 *
 * We sample sun events periodically (e.g. every 15 minutes.)
 * The real event (e.g. real sunrise) is thus confirmed on average about 15/2 minutes late.
 *
 * No instance, pure class.
 *
 * Same class is used alternately to detect event: sunrise or sunset.
 * We never detect sunrise and sunset concurrently.
 * Caller should ensure that consecutive events are from same signal ( e.g. sunrise).
 * This is agnostic of the signal kind.
 * Usually, after an event is confirmed, caller begins detecting opposite event.
 * But caller can do further filtering or sanity checks, and continue detecting same kind of event.
 * Call reset() when signal changes.
 */


class ConfirmedSunEvent
{

public:
    /*
     * Parameter is true iff sample indicated event.
     *
     * Return true if this sample of an event confirms consecutive events, i.e. a T preceded this T.
     * When true is returned, any subsequent calls with true should also return true.
     *
     * Else mark no event occurred (side effect) and return false.
     */
    static bool doesThisEventConfirm(const bool wasEvent);


    /*
     * Called when daylight detected raw w/o calling doesThisEventConfirm.
     * For testing purposes?
     */
    static void feedDaylightEvent();

    /*
     * Begin filtering a new signal.
     * Show that we have no history of samples of events.
     */
    static void reset();
};


#ifdef OLD
    private:
    static void markEvent();
    static bool isConfirmedEvent();
#endif

