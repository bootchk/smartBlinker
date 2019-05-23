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
    * Not use returned interval, only boolean result
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
        Interval interval;  // no init, function will write it
        bool didProject = canProjectTimetoReferenceTimeWithinDelta(toProjectTime,
                                              latestSunriseSample,
                                              2*Parameters::SunriseDelta,
                                              interval);
        /*
         * Not invariant: all samples project to within delta of head: myAssert (didProject);
         * It is only invariant that all samples project to average,
         * or that all samples project to with 2*delta of head
         */
        myAssert (didProject);
        intervalSum += interval;
    }

    unsigned int countSamples = CircularBuffer::getCount();
    // Signed integer division
    Interval averageInterval = intervalSum/countSamples;
    myAssert( abs(averageInterval) <= 2*Parameters::SunriseDelta );
    return averageInterval ;
}




bool SunriseCalculator::canProjectTimetoReferenceTimeWithinDelta (
                EpochTime time,
                EpochTime referenceTime,
                Duration delta,
                Interval& resultInterval
                ) {
    myRequire(referenceTime >= time);

    bool result;

    EpochTime workingProjection = projectTimePastReferenceTime(time, referenceTime);

    Interval workingInterval;
    workingInterval = workingProjection - referenceTime;
    myAssert(workingInterval >=0 );
    if ( workingInterval < delta.seconds) {
       resultInterval = workingInterval;
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
            resultInterval = -workingInterval;
            result = true;
        }
        else {
            // Can not project within delta
            result = false;
        }
    }
    // assert abs(resultInterval) < delta
    return result;
}




// public




EpochTime SunriseCalculator::estimatePreviousSunrise() {
    myRequire(not CircularBuffer::isEmpty());

    CircularBuffer::startIter();
    EpochTime latestSunriseSample = CircularBuffer::nextIter();

    // This also uses iterator
    Interval averageInterval = averageIntervalToLatestSample();

    // EpochTime +/- Interval
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
    /*
     * One design is to not require referenceTime > time.
     * (That design uses while() do {} )
     * The external design does ensure referenceTime > time,
     * so we check that here, and use do {} while(), which otherwise might project too much.
     *
     * Strictly > (in the past)
     */
    myRequire( referenceTime > time);

    EpochTime workingProjection = time;

    // Project forward by the period (say 24 hours)
    do {
        workingProjection += Parameters::SunrisePeriod;
    }
    while (workingProjection < referenceTime);
    // assert workingProjection >= referenceTime and workingProjection < referenceTime + Parameters::SunrisePeriod
    return workingProjection;
}
