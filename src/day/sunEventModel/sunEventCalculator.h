
#pragma once


#include "circularBuffer.h"


// msp430Drivers
#include <alarmClock/time/timeTypes.h>
#include <alarmClock/time/interval.h>





/*
 * Understands a sample set of detected sunrise.
 *
 * SampleSet is kept such that samples are roughly 24 hours apart (periodic.)
 * The caller can determine whether a given sample suits the sampleSet
 *
 * Knows how to calculate estimates
 * and when a sample fits sample set.
 *
 * Invariant: for all samples in SampleSet, projected sample is within delta of average projected samples.
 * ???
 */

class SunEventCalculator {
private:
    /*
     * Try project time to within delta of referenceTime (
     * Return interval of projected time to referenceTime if is possible.
     * Return false if is not possible.
     *
     * Project: advance time by 24 hours until it is near reference time.
     */
    static bool canProjectTimetoReferenceTimeWithinDelta(EpochTime time,
                                                      EpochTime referenceTime,
                                                      Duration delta,
                                                      Interval& interval);

    static bool doesSampleFitsSampleSet(EpochTime sample, CircularBuffer& sampleSequence);

    /*
     * Project samples to latestSample and sum intervals to latestSample
     *
     * Assert all samples when projected to latestSample are within delta of latestSample ????
     */
    static Interval averageIntervalToLatestSample(CircularBuffer& sampleSequence);


public:
    /*
     * Requires sampleSet not empty.
     * Not require sampleSet full: estimates using whatever samples exist.
     *
     * Estimate may be many days in past.
     */
    static EpochTime estimatePreviousSunEvent(CircularBuffer& sampleSequence);


    static EpochTime estimateNextSunEvent(CircularBuffer& sampleSequence);

    /*
     * Sample is good if sampleSet empty
     * OR sample fits previous samples in sampleSet
     */
    static bool isGoodSample(EpochTime, CircularBuffer& sampleSequence);
};
