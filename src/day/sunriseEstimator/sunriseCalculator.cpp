#include "sunriseCalculator.h"

#include "circularBuffer.h"
#include "../../parameters.h"

// msp430Drivers
#include <assert/myAssert.h>





bool SunriseCalculator::doesSampleFitsSampleSet(EpochTime sample) {
   myRequire(not CircularBuffer::isEmpty());

   bool result;

   Interval interval;

   EpochTime previousSunset = SunriseCalculator::estimatePreviousSunrise();
   myAssert(sample >= previousSunset);

   /*
    * Does estimatedPreviousSunrise project to within delta of sample?
    */
   result = canProjectTimetoReferenceTimeWithinDelta (
            previousSunset,
            sample,
            Parameters::SunriseDelta,
            interval );
   return result;
}





Interval SunriseCalculator::averageIntervalToLatestSample() {
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
        bool didProject = canProjectTimetoReferenceTimeWithinDelta(toProjectTime,
                                              latestSunriseSample,
                                              Parameters::SunriseDelta,
                                              interval);
        // Invariant is that all samples project
        myAssert (didProject);
        intervalSum += interval;
    }

    // Signed integer division
    return intervalSum/CircularBuffer::getCount();
    // assert abs(interval) <= delta
}




bool SunriseCalculator::canProjectTimetoReferenceTimeWithinDelta (
                EpochTime time,
                EpochTime referenceTime,
                Duration delta,
                Interval& interval
                ) {
    myRequire(referenceTime >= time);

    bool result;

    EpochTime workingProjection = projectTimePastReferenceTime(time, referenceTime);

    Interval workingInterval;
    workingInterval = workingProjection - referenceTime;
    myAssert(workingInterval >=0);
    if ( workingInterval < delta.seconds) {
       interval = workingInterval;
       result = true;
    }
    else {
        // Projection is more than delta beyond reference time.  Back up and test again.
        workingProjection -= Parameters::SunrisePeriod;
        workingInterval = referenceTime - workingProjection;
        myAssert(workingInterval >=0);
        if ( workingInterval < delta.seconds) {
            // we have been working with absolute value i.e. magnitude.
            // Return negative since within delta is before referenceTime
            interval = -workingInterval;
            result = true;
        }
        else {
            // Can not project within delta
            result = false;
        }
    }
    return result;
}




// public




EpochTime SunriseCalculator::estimatePreviousSunrise() {
    myRequire(not CircularBuffer::isEmpty());

    CircularBuffer::startIter();
    EpochTime latestSunriseSample = CircularBuffer::nextIter();

    // This also uses iterator
    Interval averageInterval = averageIntervalToLatestSample();

    return latestSunriseSample + averageInterval;
    /*
     * Estimate is in the past, not projected to current time.
     * May be many days in the past.
     * Estimate may be earlier than latestSunriseSample.
     */
}





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




EpochTime SunriseCalculator::projectTimePastReferenceTime(EpochTime time,
                                                  EpochTime referenceTime) {
    EpochTime workingProjection = time;

    // Project forward by 24 hours
    do {
        workingProjection += Parameters::SunrisePeriod;
    }
    while (workingProjection < referenceTime);
    return workingProjection;
}
