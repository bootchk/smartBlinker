
#include "blinkPeriod.h"
#include "morningBlinkPeriod.h"




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

// State variable
#pragma PERSISTENT
BlinkPeriodKind kind = BlinkPeriodKind::Evening;

#pragma PERSISTENT
bool _isActive = false;

}

// FUTURE enum for period kind
// FUTURE _isActive => _isOver

void BlinkPeriod::initForEveningBlinking()
{
    blinkCounter = Parameters::BlinksEvening;
    kind = BlinkPeriodKind::Evening;
    _isActive = true;
}


/*
 * !!! Don't init the sequence of morning blink periods here, only the subperiod.
 */
void BlinkPeriod::initForMorningBlinking()
{
    blinkCounter = Parameters::BlinksMorningSubperiod;
    kind = BlinkPeriodKind::Morning;
    _isActive = true;
}


void BlinkPeriod::initForNightBlinking()
{
    blinkCounter = Parameters::BlinksNight;
    kind = BlinkPeriodKind::Night;
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


BlinkPeriodKind BlinkPeriod::getKind() { return kind; }


void BlinkPeriod::advance() { blinkCounter--; }


void BlinkPeriod::terminatePrematurely() { _isActive = false; }
