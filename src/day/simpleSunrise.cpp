#include "simpleSunrise.h"

#include "../parameters.h"

#include <alarmClock/epochClock/epochClock.h>




namespace {
/*
 * In persistent memory, to survive low power mode.
 * This MUST be initialized on cold restart by a call to init, since it is in FRAM.
 *
 * Note that EpochClock starts at zero and is not zero for a long time after that (practically never rolls over.)
 */
#pragma PERSISTENT
EpochTime previousSunrise = 0;

}




void SimpleSunrise::init() {
    previousSunrise = 0;
}



void SimpleSunrise::captureSunriseTime() {
    previousSunrise = EpochClock::timeNowOrReset();
}


bool SimpleSunrise::isSunriseTimeValid() {
    return previousSunrise != 0;
}


/*
 * Implementation:
 * Next sunrise is previous + 24 hours.
 * If called more than 24 hours after previous sunrise, must keep adding 24 .
 */
EpochTime SimpleSunrise::timeOfNextSunriseAfterTime(EpochTime& now) {

    EpochTime nextSunrise = now;
    while (nextSunrise < now) {
        // FUTURE Implement operator + for EpochTime   nextSunrise += Duration(Parameters::TwentyFourHours);
        nextSunrise += Parameters::TwentyFourHours;
    }
    // assert nextSunrise > now ( is after the present moment )
    // assert nextSunrise < (now + 24hours)
    return nextSunrise;
}

Duration SimpleSunrise::durationUntilNextSunriseLessSeconds(Duration lessDuration){
    /*
     * Effectively:  nextSunriseTime - nowTime - lessDuration
     *
     * Complications:
     * - should not access nowTime unless epochClock is running: assertions in nowTime()
     * - should not call nowTime() more than once (and setAlarm() will also call it?):  FUTURE memoize it
     */
    EpochTime now = EpochClock::timeNowOrReset();

    EpochTime nextSunrise = timeOfNextSunriseAfterTime(now);

    // Subtract two EpochTimes yields a Duration
    Duration durationToNextSunrise = Duration(nextSunrise - now);

    // OLD
    // Duration result;
    //result.seconds = durationToNextSunrise.seconds - lessDuration.seconds;
    // return result;

    /*
     * Unsigned arithmetic.  Duration subtraction avoids overflow on subtraction, which yields a very large unsigned int.
     * If method is called within lessDuration seconds of nextSunrise, result is zero.
     */
    // assert result is zero or a duration smaller than the max practical duration (a day), but not a huge number.
    return durationToNextSunrise - lessDuration;
}

