#include "estimatedSunrise.h"

#include "circularBuffer.h"
#include "periodicTimeSeriesState.h"
#include "sunriseCalculator.h"

#include "../../parameters.h"

#include <alarmClock/epochClock/epochClock.h>
#include <assert/myAssert.h>





void EstimatedSunrise::init() {
    CircularBuffer::empty();
    PeriodicTimeSeriesState::init();
}


/*
 * Called at time sunrise ostensibly detected.
 * If it is periodic with prior samples,
 * add it to sample set and feed to state machine.
 */
void EstimatedSunrise::captureSunriseTime() {
    EpochTime possibleSunriseTime = EpochClock::timeNowOrReset();
    // assert possibleSunriseTime > previousSunset

    if (SunriseCalculator::isGoodSample(possibleSunriseTime)) {
        PeriodicTimeSeriesState::recordGoodSample(possibleSunriseTime);
    }
    else {
        PeriodicTimeSeriesState::recordBadSample();
    }
}



bool EstimatedSunrise::isSunriseTimeValid() { return PeriodicTimeSeriesState::isConfirmed(); }


/*
 * Implementation:
 *
 */
EpochTime EstimatedSunrise::timeOfNextSunriseAfterTime(EpochTime& now) {

    EpochTime nextSunrise = now;

    // TODO

    return nextSunrise;
}

Duration EstimatedSunrise::durationUntilNextSunriseLessSeconds(Duration lessDuration){
    myRequire(isSunriseTimeValid());
    // TODO
    EpochTime estimatedPreviousSunrise = SunriseCalculator::estimatePreviousSunrise();

}

