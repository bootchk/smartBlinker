

// msp430Drivers
#include <alarmClock/time/timeTypes.h>



/*
 * Estimated statistically
 */
class EstimatedSunrise {
public:
    static void init();
    static void captureSunriseTime();
    static bool isSunriseTimeValid();

    /*
     * Next sunrise is previous + 24 hours.
     * If called more than 24 hours after previous sunrise, must keep adding 24 .
     */
    static EpochTime timeOfNextSunriseAfterTime(EpochTime& now);

    static Duration durationUntilNextSunriseLessSeconds(Duration lessDuration);
};
