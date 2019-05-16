// msp430Drivers
#include <alarmClock/time/timeTypes.h>



/*
 * Knows how to calculate estimates
 * and when a sample fits sample set.
 */

class SunriseCalculator {
public:
    static EpochTime estimatePreviousSunrise();

    /*
     * Try project time to within delta of referenceTime (
     * Return interval of projected time to referenceTime if is possible.
     * Return false if is not possible.
     *
     * Project: advance time by 24 hours until it is near reference time.
     */
    static bool projectTimetoReferenceTimeWithinDelta (
                EpochTime time,
                EpochTime referenceTime,
                Duration delta,
                Interval& interval
                );

    static bool isGoodSample(EpochTime);
};
