#pragma once


#include "circularBuffer.h"


// msp430Drivers
#include <alarmClock/time/timeTypes.h>



enum class State {
    Unconfirmed, // SampleSet possibly non-empty, but not full, and no bad samples
    Confirmed,   // SampleSet full, without bad suffix
    ConfirmedWBad // SampleSet full, with one bad suffix
};


/*
 * A time series with a state.
 *
 * A state machine that counts sequences of good and bad samples.
 *
 * Confirmed: An unbroken string of good samples (of a certain length, say 2)
 * or a string of good samples followed by a string of bad samples (length 1).
 *
 * Two consecutive bad samples unconfirms.
 */

class PeriodicTimeSeries {
private:
    // state encodes a count of bad samples
    State state;

    CircularBuffer sampleSequence;


    void toConfirmed();
    void toUnconfirmed();
    void toConfirmedWBad();


public:
    void init();

    /*
     * Does time series have enough good samples and not too many bad samples?
     */
    bool isValid();

    void recordGoodSample(EpochTime sample);
    void recordBadSample();



    // Methods delegated to SunriseCalculator
    bool isGoodSample(EpochTime);
    EpochTime projectTimePastReferenceTime(EpochTime time,  EpochTime referenceTime);
    EpochTime estimatePreviousSunrise();    // CircularBuffer& sampleSequence);
};
