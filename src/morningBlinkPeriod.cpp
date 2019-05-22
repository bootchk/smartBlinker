

#include "morningBlinkPeriod.h"
#include "parameters.h"



namespace {

#pragma PERSISTENT
unsigned int countMorningBlinkPeriods = 0;

#pragma PERSISTENT
bool terminated = false;
}





void MorningBlinkPeriod::init() {
    countMorningBlinkPeriods = 0;
    terminated = false;
}


void MorningBlinkPeriod::terminate() { terminated = true; }

bool MorningBlinkPeriod::isDone() {
    bool result = false;

    countMorningBlinkPeriods++;
    if ( countMorningBlinkPeriods > Parameters::CountMorningBlinkPeriods ) {
        result = true;
    }

    if (terminated) result = true;

    return result;
}
