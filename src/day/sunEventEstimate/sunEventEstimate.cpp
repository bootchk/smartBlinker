
#include "sunEventEstimate.h"

#include "../../parameters.h"

#include <alarmClock/epochClock/epochClock.h>
#include <assert/myAssert.h>



#include "periodicTimeSeries.h"
//#include <src/day/sunEventEstimate/sunriseCalculator.h>



//CircularBuffer SunEventEstimate::sampleSequence;
PeriodicTimeSeries SunEventEstimate::timeSeries;





SunEventEstimate::SunEventEstimate() {
    timeSeries = PeriodicTimeSeries();
}




/*
 * SunEvent was detected during this wake period.
 * Return the time of detection.
 *
 * Because of filtering, first time of detection was earlier.
 * Also, we may shift the time earlier than now.
 * Shift because:
 * - we don't check often enough, so shift by half the detect period
 * - if we were late to try detect (morning blink period overrun) ????
 */
EpochTime SunEventEstimate::getTimeForConfirmedSunEvent() {
    EpochTime now = EpochClock::timeNowOrReset();

    /*
     * Adjust for low-pass filter
     *
     * See ConfirmedSunEvent: we first detected on previous sun check.
     * We check sun every say 15 minutes.
     */
    return (now - Parameters::BetweenSunChecks);
}




void SunEventEstimate::init() {
    // OLD sampleSequence.empty();
    timeSeries.init();
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
void SunEventEstimate::captureSunEventTime() {
    EpochTime possibleSunEventTime = SunEventEstimate::getTimeForConfirmedSunEvent();
    // assert possibleSunEventTime (sunrise) > previous opposite sun event (Sunset)

    if (timeSeries.isGoodSample(possibleSunEventTime)) {
        timeSeries.recordGoodSample(possibleSunEventTime);
    }
    else {
        timeSeries.recordBadSample();
    }
}


/*
 * Valid if enough good samples and not too many bad samples.
 */
bool SunEventEstimate::isSunEventTimeValid() { return timeSeries.isValid(); }


/*
 * Implementation:
 *
 * SunriseCalculator does not know EpochClock, nor how to calculate Durations.
 *
 * This handles case: called so close to next sunrise that we can't subtract lessDuration;
 */
Duration SunEventEstimate::durationUntilNextSunEventLessSeconds(Duration lessDuration){
    myRequire(isSunEventTimeValid());

    EpochTime now = EpochClock::timeNowOrReset();

    EpochTime estimatedNextSunEvent = timeSeries.projectTimePastReferenceTime(
            timeSeries.estimatePreviousSunrise(),
            now );
    // assert estimatedNextSunrise >= now

    Duration tilSunrise = estimatedNextSunEvent - now;
    // assert tilSunrise >= 0

    return  (tilSunrise - lessDuration);
    // assert result >= 0, since subtraction operator on Duration guarantees that.
}

