

#include "morningBlinkPeriod.h"
#include "../parameters.h"



namespace {

// counts up
#pragma PERSISTENT
unsigned int countSubperiods = 0;

#pragma PERSISTENT
bool terminated = false;
}





void MorningSuperBlinkPeriod::init() {
    countSubperiods = 0;
    terminated = false;
}


void MorningSuperBlinkPeriod::terminate() { terminated = true; }

// Side effect on state
bool MorningSuperBlinkPeriod::isDone() {
    bool result = false;

    countSubperiods++;
    if ( countSubperiods > Parameters::CountMorningBlinkPeriods ) {
        result = true;
    }

    if (terminated) result = true;

    return result;
}
