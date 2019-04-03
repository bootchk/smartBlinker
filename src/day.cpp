
#include <realTimeClock/epochClock/epochClock.h>
#include <assert/myAssert.h>
#include <src/day.h>
#include <src/parameters.h>





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




void Day::init() {
    previousSunrise = 0;
}



void Day::setSunriseTime() {
    previousSunrise = EpochClock::timeNowOrReset();
}

bool Day::isSunriseTimeValid() { return previousSunrise != 0; }








/*
 * Next sunrise is previous + 24 hours.
 * If called more than 24 hours after previous sunrise, must keep adding 24 .
 */
EpochTime Day::timeOfNextSunriseAfterTime(EpochTime& now) {

    EpochTime nextSunrise = now;
    while (nextSunrise < now) {
        nextSunrise += Parameters::TwentyFourHours.seconds;
    }
    // assert nextSunrise > now ( is after the present moment )
    // assert nextSunrise < (now + 24hours)
    return nextSunrise;
}



Duration Day::durationUntilNextSunriseLessSeconds(Duration lessDuration){
    /*
     * Effectively:  nextSunriseTime - nowTime - lessDuration
     *
     * Complications:
     * - should not access nowTime unless epochClock is running: assertions in nowTime()
     * - should not call nowTime() more than once (and setAlarm() will also call it?):  FUTURE memoize it
     */
    EpochTime now = EpochClock::timeNowOrReset();

    EpochTime nextSunrise = timeOfNextSunriseAfterTime(now);


    Duration durationToNextSunrise;
    durationToNextSunrise.seconds = nextSunrise - now;

    /*
     * Unsigned arithmetic.  Avoid overflow on subtraction, which yields a very large unsigned int.
     * Method must not be called within lessDuration seconds of nextSunrise
     */
    // TODO this should be an operator on Duration, safe
    // Really don't want to assert, want to proceed with a small result
    myAssert( durationToNextSunrise.seconds > lessDuration.seconds);

    Duration result;
    result.seconds = durationToNextSunrise.seconds - lessDuration.seconds;
    return result;
}




#ifdef OLD
// Now we schedule by duration, not time

/*
 * Time ahead of next sunrise by given seconds.
 *
 * Must not be called if now is less than duration from next sunrise.
 */
static EpochTime timeBeforeNextSunriseBySeconds(Duration);

EpochTime Day::timeBeforeNextSunriseBySeconds(Duration duration) {

    myAssert(isSunriseTimeValid());
    return EpochClock::timeDurationFromTime(previousSunrise, Parameters::TwentyFourHours - duration);
}
#endif
