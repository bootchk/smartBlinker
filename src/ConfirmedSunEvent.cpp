

#include <src/ConfirmedSunEvent.h>



namespace {

#pragma PERSISTENT
unsigned int countEvents;

}


bool ConfirmedSunEvent::doesThisEventConfirm( bool isPutativeEvent ) {
    bool result;

    if (isPutativeEvent) {
        countEvents += 1;
        result = (countEvents >= 2);

        /*
         * Since confirmed, prepare for opposite events.
         * E.G. if we confirmed sunrise, prepare for confirming sunset.
         * (Although the app could confirm sunrise again.)
         */
        ConfirmedSunEvent::reset();
    }
    else {
        ConfirmedSunEvent::reset();
        result = false;
    }
    return result;
}



void ConfirmedSunEvent::reset() {
    countEvents = 0;
}



#ifdef OLD
void ConfirmedSunEvent::markEvent() {
    countEvents += 1;
}


bool ConfirmedSunEvent::isConfirmedEvent() {
    return countEvents >= 2;
}
#endif
