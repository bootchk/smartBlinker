#pragma once

// msp430Drivers
#include <alarmClock/time/timeTypes.h>
#include <src/day/sunEventEstimate/periodicTimeSeries.h>

#include "circularBuffer.h"

/*
 * SunEvent: sunrise or sunset, detected by light level sample.
 * Event is detected, and then we timestamp it.
 *
 * Estimate statistically from a time series of samples.
 * I.E. a model of the sinusoidal sunlight signal over many days.
 */

class SunEventEstimate {

private:
    // static CircularBuffer sampleSequence;
    static PeriodicTimeSeries timeSeries;

public:

    SunEventEstimate();

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
     * Is the model of sunrise valid (enough samples, consistent)
     */
    bool isSunEventTimeValid();

    /*
     * Duration til next estimated sunrise.
     * Zero if next sunrise is closer than lessDuration from now.
     */
    Duration durationUntilNextSunEventLessSeconds(Duration lessDuration);
};
