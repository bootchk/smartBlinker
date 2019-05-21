

#include <src/ConfirmedSunEvent.h>



namespace {

#pragma PERSISTENT
unsigned int countEvents = 0;

}


bool ConfirmedSunEvent::doesThisEventConfirm( const bool isPutativeEvent ) {
    bool isConfirmed;

    if (isPutativeEvent)
    {
        countEvents += 1;
        isConfirmed = (countEvents >= 2);

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
        ConfirmedSunEvent::reset();
        isConfirmed = false;
    }
    // assert countEvents in [0,1]
    return isConfirmed;
}



void ConfirmedSunEvent::reset() {
    countEvents = 0;
}


void ConfirmedSunEvent::feedDaylightEvent() {
    countEvents += 1;
}


#ifdef OLD
void ConfirmedSunEvent::markEvent() {
    countEvents += 1;
}


bool ConfirmedSunEvent::isConfirmedEvent() {
    return countEvents >= 2;
}
#endif
