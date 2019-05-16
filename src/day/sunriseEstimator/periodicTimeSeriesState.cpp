
#include "periodicTimeSeriesState.h"

#include "circularBuffer.h"


// TODO invariant


enum class State {
    Unconfirmed, // SampleSet possibly non-empty, but not full, and no bad samples
    Confirmed,   // SampleSet full, without bad suffix
    ConfirmedWBad // SampleSet full, with one bad suffix
};

namespace {

#pragma PERSISTENT
State state = State::Unconfirmed;

// note state encodes a count of bad samples

void toConfirmed() {
    state = State::Confirmed;
}

void toUnconfirmed() {
    CircularBuffer::empty();
    state = State::Unconfirmed;
}

void toConfirmedWBad() {
    state = State::ConfirmedWBad;
}


}




void PeriodicTimeSeriesState::init() { state = State::Unconfirmed; }
bool PeriodicTimeSeriesState::  isConfirmed() { return state == State::Confirmed; }



void PeriodicTimeSeriesState::recordGoodSample(EpochTime sample) {
    switch (state) {
        case State::Unconfirmed:
            // append to empty or short sequence
            CircularBuffer::addSample(sample);
            if (CircularBuffer::isFull()) toConfirmed();
            break;

        case State::Confirmed:
            // replace oldest sample
            CircularBuffer::addSample(sample);
            break;

        case State::ConfirmedWBad:
            // One bad was seen, but filter it out now.

            // replace oldest sample
            CircularBuffer::addSample(sample);
            // A good sample obviates one intervening bad sample.
            toConfirmed();
            break;
        }
}

void PeriodicTimeSeriesState::recordBadSample() {
    // Not remember bad sample, just state

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
        // This is second bad in a row
        toUnconfirmed();
        break;
    }
}


