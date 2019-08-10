

#include "periodicTimeSeries.h"

#include "circularBuffer.h"
#include "sunriseCalculator.h"


// TODO invariant





// Private

void PeriodicTimeSeries::toConfirmed() {
    state = State::Confirmed;
}

void PeriodicTimeSeries::toUnconfirmed() {
    sampleSequence.empty();
    state = State::Unconfirmed;
}

void PeriodicTimeSeries::toConfirmedWBad() {
    state = State::ConfirmedWBad;
}





//#pragma PERSISTENT
//State PeriodicTimeSeries::state = State::Unconfirmed;

//#pragma PERSISTENT
//CircularBuffer PeriodicTimeSeries::sampleSequence = CircularBuffer();




void PeriodicTimeSeries::init() { this->toUnconfirmed(); }

/*
 * Is valid if confirmed or confirmed with few bad.
 * When ConfirmedWBad, we can still estimate sunrise, from several days ago.
 */
bool PeriodicTimeSeries::  isValid() {
    return (state == State::Confirmed)
            or (state == State::ConfirmedWBad);
}



void PeriodicTimeSeries::recordGoodSample(EpochTime sample) {
    switch (state) {
        case State::Unconfirmed:
            // append to empty or short sequence
            sampleSequence.addSample(sample);
            if (sampleSequence.isFull()) toConfirmed();
            break;

        case State::Confirmed:
            // replace oldest sample, remain in state Confirmed
            sampleSequence.addSample(sample);
            break;

        case State::ConfirmedWBad:
            // One bad was seen, but filter it out now.

            // replace oldest sample
            sampleSequence.addSample(sample);
            // A good sample obviates one intervening bad sample.
            toConfirmed();
            break;
        }
}

void PeriodicTimeSeries::recordBadSample() {
    // Not remember bad sample, just state
    // TODO should remember bad sample, it might reflect reality better than previous samples.
    // If we don't record it, it takes longer to come to confirmed state.

    switch (state) {
    case State::Unconfirmed:
        // Bad negates a partial string of good, must clear
        toUnconfirmed();
        break;

    case State::Confirmed:
        // Enter state for one bad suffix seen
        toConfirmedWBad();
        break;

    case State::ConfirmedWBad:
        // This is second bad in a row, clear sampleSet
        toUnconfirmed();
        break;
    }
}




/*
 * Delegate to calculator
 * Pass this->sampleSequence
 */
bool PeriodicTimeSeries::isGoodSample(EpochTime sample) {
    return SunriseCalculator::isGoodSample(sample, sampleSequence); }

EpochTime PeriodicTimeSeries::projectTimePastReferenceTime(EpochTime time,  EpochTime referenceTime) {
    return SunriseCalculator::projectTimePastReferenceTime(time, referenceTime);
}
EpochTime PeriodicTimeSeries::estimatePreviousSunrise() {
    return SunriseCalculator::estimatePreviousSunrise(sampleSequence);
}

