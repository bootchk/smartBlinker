
/*
 * Some of the methods of PeriodBlinker
 */

#include "periodBlinker.h"

// Superclass
#include "../../smartBlinker/smartBlinker.h"


#include "../../day/sunEvent/ConfirmedSunEvent.h"
#include "../../day/day.h"



/*
 * Pair of similar functions
 */
bool PeriodedBlinker::checkIsSunrise() {
    bool result;

    bool isLight =  SmartBlinker::isDayLight();

    // Feed event to low-pass filter.
    bool isConfirmed = ConfirmedSunEvent::doesThisEventConfirm( isLight );

    // If not low-pass filtered
    if (isConfirmed) {
        // Is sane with respect to model of sunrise
        result = Day::doesSunEventFit(SunEventKind::Sunrise);
        /*
         * When result false, Seems like daylight, but too early.  Keep checking for daylight.
         * We should eventually find seeming daylight closer to modeled sunrise.
         */
    }
    else {
        // Not confirmed, wait for confirmation (two signals in a row.)
        result = false;
    }

    return result;
}


bool PeriodedBlinker::checkIsSunset() {
    bool result;

    bool isDark =  SmartBlinker::isNightDark();
    bool isConfirmed = ConfirmedSunEvent::doesThisEventConfirm( isDark );

    if (isConfirmed) {
            // Is sane with respect to model of sunset
            result = Day::doesSunEventFit(SunEventKind::Sunset);
    }
    else {
            result = false;
    }

    return result;
}


void PeriodedBlinker::feedDaylightEventToFilter() {
    ConfirmedSunEvent::feedDaylightEvent();
}

