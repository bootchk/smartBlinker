// msp430Drivers
#include <alarmClock/time/timeTypes.h>




/*
 * A state machine that counts sequences of good and bad samples.
 *
 * Confirmed: An unbroken string of good samples (of a certain length) or a string of good samples followed by a string of bad samples (length 1)
 *
 */

class PeriodicTimeSeriesState {
public:
    static void init();

    /*
     * Does time series have enough good samples and not too many bad samples?
     */
    static bool isValid();

    static void recordGoodSample(EpochTime sample);
    static void recordBadSample();
};
