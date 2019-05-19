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


/*
 * Implementation:
 *
 * SunriseCalculator does not know EpochClock, nor how to calculate Durations.
 * This handles exception: called so close to next sunrise that we can't subtract lessDuration;
 *
 */
Duration EstimatedSunrise::durationUntilNextSunriseLessSeconds(Duration lessDuration){
    myRequire(isSunriseTimeValid());

    EpochTime now = EpochClock::timeNowOrReset();

    EpochTime estimatedNextSunrise = SunriseCalculator::projectTimePastReferenceTime(
            SunriseCalculator::estimatePreviousSunrise(),
            now );

    Duration tilSunrise = estimatedNextSunrise - now;

    return  tilSunrise - lessDuration;
    /*
     * assert result >= 0
     */

}

