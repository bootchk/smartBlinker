

#include "ConfirmedSunEvent.h"

// msp430Drivers
#include <assert/myAssert.h>


namespace {

#pragma PERSISTENT
unsigned int countSunEvents = 0;

}

namespace debugStats {

// Count how many times we filter out spikes, not confirmed by next sample
#pragma PERSISTENT
unsigned int filteredSunEventSpikes = 0;

}


bool ConfirmedSunEvent::doesThisEventConfirm( const bool isSampleIndicateEvent ) {
    bool isConfirmed;

    if (isSampleIndicateEvent)
    {
        countSunEvents += 1;
        isConfirmed = (countSunEvents >= 2);

        if (isConfirmed) {
            /*
             * Since confirmed, prepare for opposite events.
             * E.G. if we confirmed sunrise, prepare for confirming sunset.
             * (Although the app could confirm sunrise again.)
             */
            ConfirmedSunEvent::reset();
        }
    }
    else {
        /*
         * Record spurious events.  For testing how often they occur, calibrating sensors, understanding weather, etc.
         */
        if (countSunEvents == 1) {
            debugStats::filteredSunEventSpikes++;
        }

        ConfirmedSunEvent::reset();
        isConfirmed = false;
    }
    myAssert (countSunEvents == 0 or countSunEvents == 1);
    return isConfirmed;
}



void ConfirmedSunEvent::reset() {
    countSunEvents = 0;
}


void ConfirmedSunEvent::feedDaylightEvent() {
    countSunEvents += 1;
}


#ifdef OLD
void ConfirmedSunEvent::markEvent() {
    countEvents += 1;
}


bool ConfirmedSunEvent::isConfirmedEvent() {
    return countEvents >= 2;
}
#endif
