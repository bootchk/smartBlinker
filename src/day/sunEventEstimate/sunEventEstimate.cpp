
#include "sunEventEstimate.h"

#include "../../parameters.h"

#include <alarmClock/epochClock/epochClock.h>
#include <assert/myAssert.h>

// periodicTimeSeries has same API as sunriseCalculator, delegates to sunriseCalculator
#include "periodicTimeSeries.h"

// msp430Drivers
#include <alarmClock/timeMath/timeMath.h>









#ifdef NOTUSED
SunEventEstimate::SunEventEstimate() {
    timeSeries = PeriodicTimeSeries();
}
#endif



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
    timeSeries.init();
}


/*
 * Called when a possibly filtered, short duration (say an hour) light level signal detected sun event.
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
 * This handles case: called so close to next sun event that we can't subtract lessDuration.
 *
 * But this returns a Duration in range [0, 24 hours]
 */
Duration SunEventEstimate::durationUntilNextSunEventLessSeconds(Duration lessDuration){
    myRequire(isSunEventTimeValid());

    EpochTime now = EpochClock::timeNowOrReset();

    EpochTime estimatedNextSunEvent = TimeMath::projectTimePastReferenceTime(
            timeSeries.estimatePreviousSunEvent(),
            now,
            Parameters::SunrisePeriod);
    // assert estimatedNextSunEvent >= now

    Duration tilSunEvent = estimatedNextSunEvent - now;
    // assert tilSunEvent >= 0

    return  (tilSunEvent - lessDuration);
    // assert result >= 0, since subtraction operator on Duration guarantees that.
}



/*
 * Returns Interval in range [-12 hours, +12]
 */
Interval SunEventEstimate::intervalFromNearestSunEvent() {
    myRequire(isSunEventTimeValid());

    EpochTime now = EpochClock::timeNowOrReset();

    EpochTime estimatedNearestSunEvent = TimeMath::projectTimePastReferenceTime(
                timeSeries.estimatePreviousSunEvent(),
                now,
                Parameters::HalfDayPeriod);

    Interval intervalFromSunEvent = estimatedNearestSunEvent - now;
    // assert -12 hours < intervalFromSunEvent <= 12
    myAssert( intervalFromSunEvent >= -Parameters::HalfDayPeriod);
    myAssert( intervalFromSunEvent <= Parameters::HalfDayPeriod);

    return intervalFromSunEvent;
}

