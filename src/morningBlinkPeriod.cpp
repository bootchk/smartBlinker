

#include "morningBlinkPeriod.h"
#include "parameters.h"



namespace {

#pragma PERSISTENT
unsigned int countMorningBlinkPeriods = 0;

#pragma PERISTENT
bool terminated = false;
}





void MorningBlinkPeriod::init() {
    countMorningBlinkPeriods = 0;
    terminated = false;
}


void MorningBlinkPeriod::terminate() { terminated = true; }

bool MorningBlinkPeriod::isDone() {
    bool result;
    countMorningBlinkPeriods++;
    if ( countMorningBlinkPeriods > Parameters::CountMorningBlinkPeriods ) {
        result = true;
    }

    if (terminated) result = true;

    return result;
}
