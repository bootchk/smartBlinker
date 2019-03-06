
#include <realTimeClock/epochClock/epochClock.h>
#include <assert/myAssert.h>
#include <src/day.h>
#include <src/parameters.h>





namespace {
/*
 * In persistent memory, to survive low power mode.
 * They must be initialized on cold restart by a call to init.
 */
#pragma PERSISTENT
EpochTime previousSunrise = 0;

#pragma PERSISTENT
bool _isSunriseTimeValid = false;

}




void Day::init() {
    _isSunriseTimeValid = false;
}



void Day::setSunriseTime() {
    previousSunrise = EpochClock::timeNow();
    _isSunriseTimeValid = true;
}


bool Day::isSunriseTimeValid() { return _isSunriseTimeValid; }


/*
 * Next sunrise is previous + 24 hours.
 *
 * If called more than 24 hours after previous sunrise, the result time may be in the past.
 */
EpochTime Day::timeOfNextSunrise() { return previousSunrise + Parameters::TwentyFourHours.seconds; }


Duration Day::durationUntilNextSunriseLessSeconds(Duration lessDuration){
    // TODO
    /*
     * Effectively:  nextSunriseTime - nowTime - lessDuration
     * But we don't want to access nowTime unless epochClock is running.
     */
}




#ifdef OLD
broken.  Need timeDurationBeforeTime(timeOfNextSunrise(), duration);
EpochTime Day::timeBeforeNextSunriseBySeconds(Duration duration) {

    myAssert(isSunriseTimeValid());
    return EpochClock::timeDurationFromTime(previousSunrise, Parameters::TwentyFourHours - duration);
}
#endif
