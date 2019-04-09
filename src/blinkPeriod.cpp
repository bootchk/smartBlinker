
#include <src/blinkPeriod.h>




/*
 * !!! Not static, so they appear in map.
 * Also, if static, might exist multiple instances, whenever this file is included without #pragma once
 */
namespace {

/*
 * Blink period must persist through low power sleep
 */
#pragma PERSISTENT
int blinkCounter = 0;

// State variable, evening and morning blink subperiods
#pragma PERSISTENT
bool _isEvening = false;
#pragma PERSISTENT
bool _isNight = false;

#pragma PERSISTENT
bool _isActive = false;

}

// FUTURE enum for period kind
// FUTURE _isActive => _isOver

void BlinkPeriod::initForEveningBlinking()
{
    blinkCounter = Parameters::BlinksEvening;
    _isEvening = true;
    _isActive = true;
}


void BlinkPeriod::initForMorningBlinking()
{

    blinkCounter = Parameters::BlinksMorning;
    _isEvening = false;
    _isNight = false;
    _isActive = true;
}

void BlinkPeriod::initForNightBlinking()
{
    blinkCounter = Parameters::BlinksNight;
    _isEvening = false;
    _isNight = true;
    _isActive = true;
}


bool BlinkPeriod::isActive()
{
    return _isActive;
}


/*
 * Side effect: deactivate when over
 */
bool BlinkPeriod::isOver()
{
    bool result;

    // if terminated prematurely
    if (not _isActive) {
        result = true;
    }
    else {
        // if all blinks completed
        if (blinkCounter <= 0) {
            _isActive = false;
            result = true;
        }
        else {
            // still active and blinks not complete
            result = false;
        }
    }

    // assert not _isActive or blinkCounter > 0
    return result;
}


bool BlinkPeriod::isEvening() { return _isEvening; }
bool BlinkPeriod::isNight() { return _isNight; }


void BlinkPeriod::advance()
{
    blinkCounter--;
}


void BlinkPeriod::terminatePrematurely() {
    _isActive = false;
}
