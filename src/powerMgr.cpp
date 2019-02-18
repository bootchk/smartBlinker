
#include <ADC/adc.h>

#include <src/parameters.h>
#include <src/powerMgr.h>




bool PowerMgr::isPowerForBlinking() {
    // Does not require GPIO configuration
    // Does not change GPIO configuration

    // Greater than 3 volts
    return ADC::measureVccCentiVolts() > Parameters::MinVccForBlinking ;
}



bool PowerMgr::isNearBrownOut() {
    // Does not require GPIO configuration
    // Does not change GPIO configuration

    // Less than 1.8 volts
    return ADC::measureVccCentiVolts() < Parameters::MinVccForBrownout ;
}
