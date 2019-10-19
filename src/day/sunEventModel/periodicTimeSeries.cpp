
#include <src/day/sunEventModel/periodicTimeSeries.h>
#include <src/day/sunEventModel/sunEventCalculator.h>

// TODO invariant





// Private

void PeriodicTimeSeries::toConfirmed() {
    DSPstate = DSPState::Confirmed;
}

void PeriodicTimeSeries::toUnconfirmed() {
    sampleSequence.empty();
    DSPstate = DSPState::Unconfirmed;
}

void PeriodicTimeSeries::toConfirmedWBad() {
    DSPstate = DSPState::ConfirmedWBad;
}





//#pragma PERSISTENT
//DSPState PeriodicTimeSeries::DSPstate = DSPState::Unconfirmed;

//#pragma PERSISTENT
//CircularBuffer PeriodicTimeSeries::sampleSequence = CircularBuffer();




void PeriodicTimeSeries::init() {
    // state
    this->toUnconfirmed();

    sampleSequence.empty();
}

/*
 * Is valid if confirmed or confirmed with few bad.
 * When ConfirmedWBad, we can still estimate sunrise, from several days ago.
 */
bool PeriodicTimeSeries::  isValid() {
    return (DSPstate == DSPState::Confirmed)
            or (DSPstate == DSPState::ConfirmedWBad);
}



void PeriodicTimeSeries::recordGoodSample(EpochTime sample) {
    switch (DSPstate) {
        case DSPState::Unconfirmed:
            // append to empty or short sequence
            sampleSequence.addSample(sample);
            if (sampleSequence.isFull()) toConfirmed();
            break;

        case DSPState::Confirmed:
            // replace oldest sample, remain in state Confirmed
            sampleSequence.addSample(sample);
            break;

        case DSPState::ConfirmedWBad:
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

    switch (DSPstate) {
    case DSPState::Unconfirmed:
        // Bad negates a partial string of good, must clear
        toUnconfirmed();
        break;

    case DSPState::Confirmed:
        // Enter state for one bad suffix seen
        toConfirmedWBad();
        break;

    case DSPState::ConfirmedWBad:
        // This is second bad in a row, clear sampleSet
        toUnconfirmed();
        break;
    }
}





// Delegate to calculator.  Pass this->sampleSequence
bool PeriodicTimeSeries::isGoodSample(EpochTime sample) {
    return SunEventCalculator::isGoodSample(sample, sampleSequence); }


EpochTime PeriodicTimeSeries::estimatePreviousSunEvent() {
    return SunEventCalculator::estimatePreviousSunEvent(sampleSequence);
}


#ifdef OLD
// Delegate to TimeMath
EpochTime PeriodicTimeSeries::projectTimePastReferenceTime(EpochTime time,  EpochTime referenceTime) {
    return TimeMath::projectTimePastReferenceTime(time, referenceTime, Parameters::SunrisePeriod);
}
#endif

