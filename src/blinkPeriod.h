
#pragma once

#include "parameters.h"




/*
 * BlinkPeriod is an interval of time during which we blink (not necessarily continuously).
 * Typically at night.
 * BlinkPeriod comprises two subperiods, evening and morning, separated by period of non-blinking.

 * Knows count of blinks in blink periods.
 *
 * We schedule the blinking task, not an end of period task.
 *
 * BlinkPeriod comprises two subperiods, evening and morning, separated by time.
 *
 * Sequence of calls is:
 * initForEveningBlinking,
 * advance, isOver, ..., advance, isOver=>true,
 * isEvening()=>false
 * initForMorningBlinking
 * advance, isOver, ..., advance, isOver=>true,
 */


enum class BlinkPeriodKind {
    Evening,
    Night,
    Morning
};


class BlinkPeriod {

public:
    static void initForEveningBlinking();
    static void initForNightBlinking();
    static void initForMorningBlinking();

    static bool isActive();

    /*
     * Side effect: deactivate when over
     */
    static bool isOver();

    static BlinkPeriodKind getKind();

    static void advance();

    /*
     * Terminate before blink count exceeded.
     * Ensures isOver()=>true
     */
    static void terminatePrematurely();
};
