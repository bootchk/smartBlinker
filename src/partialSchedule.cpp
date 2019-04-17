#include "config.h"


// For completing partial, i.e. calculating Duration
#include "parameters.h"
#include "day.h"


// Task to schedule
#include "smartBlinker.h"

// embeddedDutyCycle
#include <OS/taskScheduler.h>
#include <src/partialSchedule.h>

#include <assert/myAssert.h>




namespace {

bool desireSchedule = false;

Duration durationUntilMorningBlinkPeriodStart() {
#ifdef ACCELERATED_TIME_PARAMETERS
    return Parameters::BetweenEveningAndNightBlinking;
#else
    return Day::durationUntilNextSunriseLessSeconds(Parameters::BetweenMorningBlinkStartAndSunrise);
#endif
}


}

void PartialSchedule::schedule() {
    // illegal to schedule() twice without intervening complete()
    myAssert(desireSchedule == false);
    desireSchedule = true;
}

/*
 * Specialized to schedule start of morning blink period.
 */
void PartialSchedule::complete() {
    if (desireSchedule) {
        TaskScheduler::scheduleTask(
                   0,
                   SmartBlinker::blinkTask,
                   durationUntilMorningBlinkPeriodStart() );
    }
    desireSchedule = false;
}



