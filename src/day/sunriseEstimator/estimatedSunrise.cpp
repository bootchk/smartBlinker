#include "estimatedSunrise.h"

#include "circularBuffer.h"
#include "periodicTimeSeriesState.h"
#include "sunriseCalculator.h"

#include "../../parameters.h"

#include <alarmClock/epochClock/epochClock.h>
#include <assert/myAssert.h>





/*
 * Sunrise was detected during this wake period.
 * Return the time of detection.
 *
 * Because of filtering, first time of detection was earlier.
 * Also, we may shift the time earlier than now.
 * Shift because:
 * - we don't check often enough, so shift by half the detect period
 * - if we were late to try detect (morning blink period overrun) ????
 */
EpochTime EstimatedSunrise::getSunriseTimeSampleForConfirmedSunEvent() {
    EpochTime now = EpochClock::timeNowOrReset();

    /*
     * Adjust for low-pass filter
     *
     * See ConfirmedSunEvent: we first detected on previous sun check.
     * We check sun every say 15 minutes.
     */
    return (now - Parameters::BetweenSunChecks);
}




void EstimatedSunrise::init() {
    CircularBuffer::empty();
    PeriodicTimeSeriesState::init();
}


/*
 * Called when a possibly filtered, short duration (say an hour) light level signal detected sunrise.
 * Such a signal is "confirmed."
 * Called at end of wake period in some implementations.
 *
 * Convert boolean signal to a time.
 *
 * Feed it through another filter, over days.
 * If it is periodic with samples from prior days, it is "good", else "bad".
 * The filter is a state machine.
 */
void EstimatedSunrise::captureSunriseTime() {
    EpochTime possibleSunriseTime = getSunriseTimeSampleForConfirmedSunEvent();
    // assert possibleSunriseTime > previousSunset

    if (SunriseCalculator::isGoodSample(possibleSunriseTime)) {
        PeriodicTimeSeriesState::recordGoodSample(possibleSunriseTime);
    }
    else {
        PeriodicTimeSeriesState::recordBadSample();
    }
}


/*
 * Valid if enough good samples and not too many bad samples.
 */
bool EstimatedSunrise::isSunriseTimeValid() { return PeriodicTimeSeriesState::isValid(); }


/*
 * Implementation:
 *
 * SunriseCalculator does not know EpochClock, nor how to calculate Durations.
 *
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

    return  (tilSunrise - lessDuration);
    /*
     * assert result >= 0
     */

}

