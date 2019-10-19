#pragma once

// msp430Drivers
#include <alarmClock/time/timeTypes.h>
#include <alarmClock/time/interval.h>
#include "circularBuffer.h"
#include <src/day/sunEventModel/circularBuffer.h>
#include <src/day/sunEventModel/periodicTimeSeries.h>

/*
 * SunEvent: sunrise or sunset, detected by light level sample.
 * Event is detected, and then we timestamp it.
 *
 * Estimate statistically from a time series of samples.
 * I.E. a model of the sinusoidal sunlight signal over many days.
 */

class SunEventModel {

private:
    // !!! Not static
    PeriodicTimeSeries timeSeries;

public:

    // No constructor    SunEventEstimate();

    // Called once at coldstart
    void init();

    // Timestamp for event
    // Class method, doesn't depend on instance
    static EpochTime getTimeForConfirmedSunEvent();

    /*
     * With a confirmed sun event, convert to time and feed to model.
     */
    void captureSunEventTime();

    /*
     * Is the model of sun event valid (enough samples, consistent)
     */
    bool isSunEventTimeValid();

    /*
     * Duration til next estimated sun event, minus lessDuration.
     *
     * Zero if next sun event is closer than lessDuration from now.
     *
     * Max result can be 24 hours.
     * That is, if called just after sun event, result will be almost 24 hours.
     */
    Duration durationUntilNextSunEventLessSeconds(Duration lessDuration);

    /*
     * Interval (signed time difference) to predicted nearest sun event.
     *
     * Requires estimated previous sun event > 12 hours ago.
     *
     * Result in [-12 hours, +12 hours]
     */
    Interval intervalFromPredictedNearestSunEvent();
};
