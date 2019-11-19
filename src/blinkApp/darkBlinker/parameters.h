

/*
 * DarkBlinker
 *
 * !!! No definitions for accelerated time
 */

#include "../../timeConstants.h"    // ONE_HOUR



namespace Parameters {


static const unsigned int BetweenBlinks = 10;

static const unsigned int BetweenDarkBlinkerDarkChecks = 10;

// TODO accelerated: shorten to 3 seconds
//static const unsigned int BetweenKeepAlive = 3;
static const unsigned int BetweenKeepAlive = ONE_HOUR;


}
