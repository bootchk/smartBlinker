#pragma once


#include <alarmClock/time/timeTypes.h>
#include <src/day/sunEventModel/circularBuffer.h>



enum class DSPState {
    Unconfirmed, // SampleSet possibly non-empty, but not full, and no bad samples
    Confirmed,   // SampleSet full, without bad suffix
    ConfirmedWBad // SampleSet full, with one bad suffix
};


/*
 * A time series of signal samples.
 * Having a DSP model having a state.
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
    /*
     * Instance data members persistent if class instance is.
     */
    // state encodes a count of bad samples
    DSPState DSPstate;

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
    EpochTime estimatePreviousSunEvent();    // CircularBuffer& sampleSequence);

    // Methods using TimeMath
    EpochTime projectTimePastReferenceTime(EpochTime time,  EpochTime referenceTime);
};
