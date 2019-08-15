

#include "ConfirmedSunEvent.h"

// msp430Drivers
#include <assert/myAssert.h>


namespace {

#pragma PERSISTENT
unsigned int lowPassFilterCount = 0;

}

namespace debugStats {

// Count how many times we filter out spikes, not confirmed by next sample
#pragma PERSISTENT
unsigned int filteredSunEventSpikes = 0;

}


bool ConfirmedSunEvent::doesThisEventConfirm( const bool isSampleIndicateEvent ) {
    bool result;

    if (isSampleIndicateEvent)
    {
        lowPassFilterCount += 1;
        result = (lowPassFilterCount >= 2);

#ifdef OLD
        if (result) {
            // Since confirmed, automatically prepare for new signal.
            ConfirmedSunEvent::reset();
        }
#endif
    }
    else {
        /*
         * Record spurious events.  For testing how often they occur, calibrating sensors, understanding weather, etc.
         */
        if (lowPassFilterCount == 1) {
            debugStats::filteredSunEventSpikes++;
        }

        ConfirmedSunEvent::reset();
        result = false;
    }
    // lowPassFilterCount can increment indefinitely
    // We could check that it is not too large, meaning failure higher in caller
    return result;
}



void ConfirmedSunEvent::reset() {
    lowPassFilterCount = 0;
}


void ConfirmedSunEvent::feedDaylightEvent() {
    lowPassFilterCount += 1;
}


#ifdef OLD
void ConfirmedSunEvent::markEvent() {
    countEvents += 1;
}


bool ConfirmedSunEvent::isConfirmedEvent() {
    return countEvents >= 2;
}
#endif
