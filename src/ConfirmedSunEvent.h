#pragma once

/*
 * Enforces consecutive, putative sun events to confirm the sun event.
 * We detect putative sun events periodically (e.g. every 15 minutes.)
 * The real event (e.g. real sunrise) is thus confirmed on average about 15/2 minutes late.
 *
 * No instance, pure class.
 *
 * Same class is used alternately to detect event: sunrise or sunset.
 * We never detect sunrise and sunset concurrently.
 *
 * Consecutive putative events are all of the same type (e.g. sunrise).
 */


class ConfirmedSunEvent
{

public:
    /*
     * Parameter is true if putative event occurred.
     * Parameter is false if event did not occur (unconfirms any immediatley prior putative event.)
     *
     * Return true if this putative event confirms consecutive events.
     * Else mark no event occurred (side effect) and return false.
     */
    static bool doesThisEventConfirm(const bool wasEvent);


    /*
     * Called when daylight detected raw w/o calling doesThisEventConfirm
     */
    static void feedDaylightEvent();

    /*
     * Show that we have no history of putative events.
     */
    static void reset();
};


#ifdef OLD
    private:
    static void markEvent();
    static bool isConfirmedEvent();
#endif

