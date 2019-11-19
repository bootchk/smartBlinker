
// periodicTimeSeries has same API as sunriseCalculator, delegates to sunriseCalculator
#include <src/day/sunEventModel/periodicTimeSeries.h>
#include <src/day/sunEventModel/sunEventModel.h>

#include "../day.h"


// Parameters from the user of Day
#include "../../blinkApp/periodBlinker/parameters.h"


// msp430Drivers
#include <alarmClock/time/timeMath.h>
#include <alarmClock/epochClock/epochClock.h>
#include <assert/myAssert.h>











#ifdef NOTUSED
SunEventModel::SunEventEstimate() {
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
EpochTime SunEventModel::getTimeForConfirmedSunEvent() {
    EpochTime now = EpochClock::timeNowOrReset();

    /*
     * Adjust for low-pass filter
     *
     * See ConfirmedSunEvent: we first detected on previous sun check.
     * We check sun every say 15 minutes.
     */
    return (now - Parameters::BetweenSunChecks);
}




void SunEventModel::init() {
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
void SunEventModel::captureSunEventTime() {
    EpochTime possibleSunEventTime = SunEventModel::getTimeForConfirmedSunEvent();
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
bool SunEventModel::isSunEventTimeValid() { return timeSeries.isValid(); }


/*
 * Implementation:
 *
 * This handles case: called so close to next sun event that we can't subtract lessDuration.
 *
 * But this returns a Duration in range [0, 24 hours]
 */
Duration SunEventModel::durationUntilNextSunEventLessSeconds(Duration lessDuration){
    myRequire(isSunEventTimeValid());

    EpochTime now = EpochClock::timeNowOrReset();

    EpochTime estimatedNextSunEvent = TimeMath::projectTimePastReferenceTime(
            timeSeries.estimatePreviousSunEvent(),
            now,
            DayPeriod);
    // assert estimatedNextSunEvent >= now

    Duration tilSunEvent = estimatedNextSunEvent - now;
    // assert tilSunEvent >= 0

    return  (tilSunEvent - lessDuration);
    // assert result >= 0, since subtraction operator on Duration guarantees that.
}


//namespace debugSunEventModel {

// Persistent for ease of debugging (instead of looking at registers.)
#pragma PERSISTENT
Interval intervalFromSunEvent = 0;

#pragma PERSISTENT
EpochTime now = 0;

#pragma PERSISTENT
EpochTime estimatedPreviousSunrise = 0;



/*
 * Returns Interval in range [-12 hours, +12]
 */
Interval SunEventModel::intervalFromPredictedNearestSunEvent() {
    myRequire(isSunEventTimeValid());

    now = EpochClock::timeNowOrReset();
    estimatedPreviousSunrise = timeSeries.estimatePreviousSunEvent();

    EpochTime estimatedNearestSunEvent = TimeMath::projectTimeByPeriodToNearReferenceTime(
            estimatedPreviousSunrise,
            now,
            DayPeriod,
            HalfDayPeriod,
            intervalFromSunEvent
            );

    // projectTimeByPeriodToNearReferenceTime guarantees interval is in range [-12 hours, 12]

#ifdef NOTUSED
    // Subtraction of two unsigned longs, coerce to signed long
    intervalFromSunEvent = estimatedNearestSunEvent - now;

    // assert -12 hours <= intervalFromSunEvent <= 12
    // !!! Use signed arithmetic.  C will gladly coerce to unsigned, with wrong result.
    myAssert( intervalFromSunEvent.inRange(Parameters::HalfDayPeriod) == RangeResult::InRange );
#endif

    return intervalFromSunEvent;
}

