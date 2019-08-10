#include "circularBuffer.h"
#include "../../parameters.h"

// msp430Drivers
#include <assert/myAssert.h>
#include <src/day/sunEventEstimate/circularBuffer.h>
#include <src/day/sunEventEstimate/sunriseCalculator.h>





bool SunriseCalculator::doesSampleFitsSampleSet(EpochTime sample, CircularBuffer& sampleSequence) {
   myRequire(not sampleSequence.isEmpty());

   bool result;

   Interval interval;

   EpochTime previousSunset = SunriseCalculator::estimatePreviousSunrise(sampleSequence);
   myAssert(sample >= previousSunset);

   /*
    * Not use returned interval, only boolean result
    */
   result = canProjectTimetoReferenceTimeWithinDelta (
            previousSunset,
            sample,
            Parameters::MaxSunriseDelta,
            interval );
   return result;
}





Interval SunriseCalculator::averageIntervalToLatestSample(CircularBuffer& sampleSequence) {
    myRequire(not sampleSequence.isEmpty());

    sampleSequence.startIter();

    EpochTime latestSunriseSample = sampleSequence.nextIter();

    // latestSunriseSample has interval 0 to itself
    Interval intervalSum = 0;

    /*
     * Sum intervals from remaining samples.
     */
    EpochTime toProjectTime;
    while ( (toProjectTime = sampleSequence.nextIter()) != 0 ) {
        Interval interval;  // no init, function will write it
        bool didProject = canProjectTimetoReferenceTimeWithinDelta(toProjectTime,
                                              latestSunriseSample,
                                              2*Parameters::MaxSunriseDelta,
                                              interval);
        /*
         * Not invariant: all samples project to within delta of head: myAssert (didProject);
         * It is only invariant that all samples project to average,
         * or that all samples project to with 2*delta of head
         */
        myAssert (didProject);
        intervalSum += interval;
    }

    unsigned int countSamples = sampleSequence.getCount();
    // Signed integer division
    Interval averageInterval = intervalSum/countSamples;
    // long absolute value
    myAssert( labs(averageInterval) <= 2*Parameters::MaxSunriseDelta );
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
            // workingInterval is unsigned positive i.e. magnitude.
            // Return signed negative since within delta is before referenceTime
            resultInterval = -workingInterval;
            result = true;
        }
        else {
            // Can not project within delta
            result = false;
        }
    }
    // assert (result == true and abs(resultInterval) < delta) or result == false
    return result;
}




// public




EpochTime SunriseCalculator::estimatePreviousSunrise(CircularBuffer& sampleSequence) {
    myRequire(not sampleSequence.isEmpty());

    sampleSequence.startIter();
    EpochTime latestSunriseSample = sampleSequence.nextIter();

    // This also uses iterator
    Interval averageInterval = averageIntervalToLatestSample(sampleSequence);

    // EpochTime +/- Interval
    return latestSunriseSample + averageInterval;
    /*
     * Estimate is in the past, not projected to current time.
     * May be many days in the past.
     * Estimate may be earlier than latestSunriseSample.
     */
}





bool SunriseCalculator::isGoodSample(EpochTime sample, CircularBuffer& sampleSequence) {
    bool result = false;

    if (sampleSequence.isEmpty()) {
        result = true;
    }
    else {
        result = doesSampleFitsSampleSet(sample, sampleSequence );
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
