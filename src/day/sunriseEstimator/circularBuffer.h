
// msp430Drivers
#include <alarmClock/time/timeTypes.h>




class CircularBuffer {
public:

    static void addSample(EpochTime sample);

    static void empty();
    static bool isFull();
    static bool isEmpty();

    // Returns [0, sizeof SampleSet]
    static unsigned int getCount();


    static void startIter();
    /*
     * Returns EpochTime or zero after last sample
     */
    static EpochTime nextIter();
};
