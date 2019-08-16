
#pragma once

#include "../parameters.h"




/*
 * BlinkPeriod is a period during which we blink (not necessarily continuously).
 * Typically at night.
 *
 * Might be many BlinkPeriods: e.g. evening, night, and morning.
 *
 * BlinkPeriods can be separated by period of non-blinking.

 * Knows count of blinks in blink periods.
 *
 * BlinkPeriod is over when:
 * - countdown
 * - OR premature termination
 * --- detect sun event
 * --- power exhausted
 *
 *
 * BlinkPeriod is not a task.
 * We schedule the blinking task, not an end of period task.
 * The blinking task always checks whether BlinkPeriod is over,
 * and then schedules appropriate task.
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
