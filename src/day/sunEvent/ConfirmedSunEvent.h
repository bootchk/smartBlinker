#pragma once

/*
 * A simple low-pass filter.
 * Two consecutive events passes as an event (confirmed.)
 * One event followed by a negative event is filtered out.
 *
 * We sample sun events periodically (e.g. every 15 minutes.)
 * The real event (e.g. real sunrise) is thus confirmed on average about 15/2 minutes late.
 *
 * No instance, pure class.
 *
 * Same class is used alternately to detect event: sunrise or sunset.
 * We never detect sunrise and sunset concurrently.
 *
 * Caller need not ensure that consecutive events are all of the same kind (e.g. sunrise).
 * This is agnostic of the kind of event.
 *
 * Usually, after an event is confirmed, caller begins detecting opposite event.
 * But caller can do further filtering or sanity checks, and continue detecting same kind of event.
 */


class ConfirmedSunEvent
{

public:
    /*
     * Parameter is true iff sample indicated event.
     *
     * A sample that contraindicated an event unconfirms any immediately prior sample that did indicate event.
     *
     * Return true if this sample of an event confirms consecutive events.
     * Else mark no event occurred (side effect) and return false.
     */
    static bool doesThisEventConfirm(const bool wasEvent);


    /*
     * Called when daylight detected raw w/o calling doesThisEventConfirm.
     * For testing purposes?
     */
    static void feedDaylightEvent();

    /*
     * Show that we have no history of samples of events.
     */
    static void reset();
};


#ifdef OLD
    private:
    static void markEvent();
    static bool isConfirmedEvent();
#endif

