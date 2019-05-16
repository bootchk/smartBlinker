#include "sunriseCalculator.h"

#include "circularBuffer.h"

#include <assert/myAssert.h>



namespace {

bool doesSampleFitsSampleSet(EpochTime sample) {
   myRequire(not CircularBuffer::isEmpty());

   bool result;

   Interval interval;

   EpochTime previousSunset = SunriseCalculator::estimatePreviousSunrise();
   myAssert(sample >= previousSunset);

   /*
    * Does estimatedPreviousSunrise project to within delta of sample?
    */
   result = SunriseCalculator::projectTimetoReferenceTimeWithinDelta (
            previousSunset,
            sample,
            1,// TODO
            interval );
   return result;
}




/*
 * Project samples to latestSample and sum intervals to latestSample
 *
 * Assert all samples are projectable (invariant.)
 */
Interval averageIntervalToLatestSample() {
    myRequire(not CircularBuffer::isEmpty());

    CircularBuffer::startIter();

    EpochTime latestSunriseSample = CircularBuffer::nextIter();

    // latestSunriseSample has interval 0 to itself
    Interval intervalSum = 0;

    /*
     * Sum intervals from remaining samples.
     */
    EpochTime toProjectTime;
    while ( (toProjectTime = CircularBuffer::nextIter()) != 0 ) {
        Interval interval;
        SunriseCalculator::projectTimetoReferenceTimeWithinDelta(toProjectTime,
                                              latestSunriseSample,
                                              1,
                                              interval );
        intervalSum += interval;
    }

    // Signed integer division
    return intervalSum/CircularBuffer::getCount();
}


}





EpochTime SunriseCalculator::estimatePreviousSunrise() {
    myRequire(not CircularBuffer::isEmpty());

    CircularBuffer::startIter();
    EpochTime latestSunriseSample = CircularBuffer::nextIter();

    // This also uses iterator
    Interval averageInterval = averageIntervalToLatestSample();

    return latestSunriseSample + averageInterval;
    // estimate is in the past, not projected to current time
}




bool SunriseCalculator::projectTimetoReferenceTimeWithinDelta (
                EpochTime time,
                EpochTime referenceTime,
                Duration delta,
                Interval& interval
                ) {
    myRequire(referenceTime >= time);

    bool result;

    EpochTime workingProjection = time;

    // Project forward
    do {
        workingProjection += 24; // TODO hours
    }
    while (workingProjection < referenceTime);

    Interval workingInterval;
    workingInterval = workingProjection - referenceTime;
    myAssert(workingInterval >=0);
    if ( workingInterval < delta.seconds) {
       interval = workingInterval;
       result = true;
    }
    else {
        // Projection is more than delta beyond reference time.  Back up and test again.
        workingProjection -= 24; // TODO hours
        workingInterval = referenceTime - workingProjection;
        if ( -workingInterval < delta.seconds) {
              interval = workingInterval;
              result = true;
        }
        else {
            // Can not project within delta
            result = false;
        }
    }
    return result;
}




/*
 * Sample is good if sampleSet empty
 * OR fits previous samples
 */
bool SunriseCalculator::isGoodSample(EpochTime sample) {
    bool result = false;

    if (CircularBuffer::isEmpty()) {
        result = true;
    }
    else {
        result = doesSampleFitsSampleSet(sample);
    }

    return result;
}
