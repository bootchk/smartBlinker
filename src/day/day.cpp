#include "day.h"




// msp430Drivers
#include <alarmClock/epochClock/epochClock.h>
#include <assert/myAssert.h>


// choose implementation
#include "config.h"

#include "sunEvent/ConfirmedSunEvent.h"

#ifdef USE_SUNRISE_ESTIMATOR
#include "sunriseEstimator/estimatedSunrise.h"
#define SunriseEstimator EstimatedSunrise
#else
#include "simpleSunrise.h"
#define SunriseEstimator SimpleSunrise
#endif

#include "../parameters.h"




namespace {

/*
 * A flag between sunrise checking (when clock is not available) and later in the wake period (when clock is available.)
 *
 * Every waking period, c-startup initializes this to false.
 */
bool _wasSunriseDetected = false;

}


namespace debugStats {

// Count how many times we detected sunrise early
#pragma PERSISTENT
unsigned int earlySunriseDetect = 0;

}


void Day::init() {
    SunriseEstimator::init();

    ConfirmedSunEvent::reset();
}

void Day::captureSunriseTime() { SunriseEstimator::captureSunriseTime(); }
bool Day::isSunriseTimeValid() { return SunriseEstimator::isSunriseTimeValid(); }


void Day::onSunriseDetected() {  _wasSunriseDetected = true; }
// Not now keeping statistics about sunset.
void Day::onSunsetDetected() {   }

bool Day::wasSunriseDetected() { return  _wasSunriseDetected; }


// OLD EpochTime Day::timeOfNextSunriseAfterTime(EpochTime& now) { return SunriseEstimator::timeOfNextSunriseAfterTime(now); }

Duration Day::durationUntilNextSunriseLessSeconds(Duration lessDuration){ return SunriseEstimator::durationUntilNextSunriseLessSeconds(lessDuration) ; }



bool Day::isSunEventSane() {
    bool result;

    if (SunriseEstimator::isSunriseTimeValid()) {
        // Now time is not correct, event time is shifted due to low pass filter.
        EpochTime timeOfSunEvent = SunriseEstimator::getSunriseTimeSampleForConfirmedSunEvent();

        /*
         * Sane: within four hours of current model.
         */
        /*
         * TODO Way past current model
         */
        Duration durationTilNextSunrise = durationUntilNextSunriseLessSeconds(0);
        // Assert is positive
        if (durationTilNextSunrise > Parameters::SaneSunEventLead ) {
            // Much too early to be sunrise according to current model
            result = false;
            debugStats::earlySunriseDetect++;
        }
        else {
            // Already past sunrise by unknown amount, could be way past.
            result = true;
        }
    }
    else {
        /*
         * Model is being built, feed this sample to model.
         * Sample could be first sample.
         * Or it could vastly revise earlier samples.
         */
        result = true;
    }

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
