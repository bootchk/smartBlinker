#include "day.h"





#include <alarmClock/epochClock/epochClock.h>
#include <assert/myAssert.h>


// choose implementation
//#include "simpleSunrise.h"
//#define SunriseEstimator SimpleSunrise

#include "estimatedSunrise.h"
#define SunriseEstimator EstimatedSunrise



namespace {

/*
 * Every waking period, c-startup initializes this to false.
 */
bool _wasSunriseDetected = false;

}




void Day::init() {  SunriseEstimator::init(); }

void Day::captureSunriseTime() { SunriseEstimator::captureSunriseTime(); }
bool Day::isSunriseTimeValid() { return SunriseEstimator::isSunriseTimeValid(); }


void Day::setSunriseDetected() {  _wasSunriseDetected = true; }
bool Day::wasSunriseDetected() { return  _wasSunriseDetected; }


EpochTime Day::timeOfNextSunriseAfterTime(EpochTime& now) { return SunriseEstimator::timeOfNextSunriseAfterTime(now); }

Duration Day::durationUntilNextSunriseLessSeconds(Duration lessDuration){ return SunriseEstimator::durationUntilNextSunriseLessSeconds(lessDuration) ; }






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
