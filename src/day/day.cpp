#include "day.h"




// msp430Drivers
#include <alarmClock/epochClock/epochClock.h>
#include <assert/myAssert.h>


// choose implementation
#include "config.h"

#include "sunEvent/ConfirmedSunEvent.h"

#ifdef USE_SUNRISE_ESTIMATOR
#include <src/day/sunEventEstimate/sunEventEstimate.h>
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

// Count how many times we detected insane sun events
#pragma PERSISTENT
unsigned int earlySunrise = 0;
#pragma PERSISTENT
unsigned int lateSunrise = 0;

#pragma PERSISTENT
unsigned int earlySunset = 0;
#pragma PERSISTENT
unsigned int lateSunset = 0;

}


// Persistent
#pragma NOINIT
SunEventEstimate Day::sunriseEstimate;
#pragma NOINIT
SunEventEstimate Day::sunsetEstimate;




void Day::init() {
    sunriseEstimate.init();
    sunsetEstimate.init();

    ConfirmedSunEvent::reset();
}

void Day::captureSunriseTime() { sunriseEstimate.captureSunEventTime(); }
bool Day::isSunriseTimeValid() { return sunriseEstimate.isSunEventTimeValid(); }



/*
 * HACK.  For one, we flag and capture later, for sunset we capture when detected.
 */
void Day::onSunriseDetected() {
    // Set flag for later action
    _wasSunriseDetected = true;
}

void Day::onSunsetDetected() {
    sunsetEstimate.captureSunEventTime();
}

bool Day::wasSunriseDetected() { return  _wasSunriseDetected; }




// OLD EpochTime Day::timeOfNextSunriseAfterTime(EpochTime& now) { return SunEventEstimate::timeOfNextSunriseAfterTime(now); }
Duration Day::durationUntilNextSunriseLessSeconds(Duration lessDuration){ return sunriseEstimate.durationUntilNextSunEventLessSeconds(lessDuration) ; }





bool Day::isSunEventSane(SunEventKind kind) {
    bool result;
    SunEventSanity sanity;

    switch(kind) {
    case SunEventKind::Sunrise:
        sanity = isSunEventSane(sunriseEstimate);
        switch(sanity) {
        case SunEventSanity::Early:
            debugStats::earlySunrise++;
            result = false;
            break;
        case SunEventSanity::Late:
            debugStats::lateSunrise++;
            result = false;
            break;
        case SunEventSanity::Sane:
        case SunEventSanity::ModelInvalid:
            // Keep event if sane or building model.
            result = true;
            break;
        }
        break;

    case SunEventKind::Sunset:
        sanity = isSunEventSane(sunsetEstimate);
        switch(sanity) {
                case SunEventSanity::Early:
                    debugStats::earlySunset++;
                    result = false;
                    break;
                case SunEventSanity::Late:
                    debugStats::lateSunset++;
                    result = false;
                    break;
                case SunEventSanity::Sane:
                case SunEventSanity::ModelInvalid:
                    // Keep event if sane or building model.
                    result = true;
                    break;
        }
        break;
    }
    return result;
}


// The event is invisible parameter.  Event has occurred and we can get its attributes (time)
SunEventSanity Day::isSunEventSane(SunEventEstimate& estimate) {
    SunEventSanity result;

    if (estimate.isSunEventTimeValid()) {
        // Now time is not correct, event time is shifted due to low pass filter.
        // Call class method, not depend on kind
        EpochTime timeOfSunEvent = SunEventEstimate::getTimeForConfirmedSunEvent();

        /*
         * Sane:
         * -not too distant from estimate from valid model of nearest sun event.
         */
        Interval intervalFromNearestSunEvent = estimate.intervalFromNearestSunEvent();

        if ( intervalFromNearestSunEvent > Parameters::SaneSunEventLead)  {
            result = SunEventSanity::Late;
        }
        else if (intervalFromNearestSunEvent <= -Parameters::SaneSunEventLead) {
             result = SunEventSanity::Early;
        }
        else {
            result = SunEventSanity::Sane;
        }

#ifdef FLAWED
        Duration durationTilNextSunEvent = estimate.durationUntilNextSunEventLessSeconds(0);
        // Assert is positive or zero
        if (durationTilNextSunEvent > Parameters::SaneSunEventLead ) {
            // Much too early to be sun event according to current model
            result = false;

        }
        else {
            // Already past sunrise by unknown amount, could be way past.
            result = true;
        }
#endif
    }
    else {
        /*
         * Model is being built, feed this sample to model.
         * Sample could be first sample.
         * Or it could vastly revise earlier samples.
         */
        result = SunEventSanity::ModelInvalid;
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
