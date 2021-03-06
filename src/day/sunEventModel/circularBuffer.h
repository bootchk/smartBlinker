#pragma once

// msp430Drivers
#include <alarmClock/time/timeTypes.h>


/*
 * Circular: add() replaces eldest if full.
 *
 *
 * Iteration:
 *     Possible even if empty or not full.
 *     Returns most recent add() first, then backwards.
 *     nextIter() returns 0 at end.
 *     Does not remove elements
 *
 * Algebra:
 *    When empty: init(); isEmpty()=>true; startIter(); nextIter=>0
 *    After one add:  init(): add(x); isEmpty()=>false; startIter(); nextIter=>x; nextIter=>0; isEmpty()=>false; getCount()=>1
 *    After two adds: init(): add(x); add(y); isEmpty()=>false; startIter(); nextIter=>y; nextIter=>x; nextIter=>0; isEmpty()=>false; getCount()=>2
 *    Emptying:  init(): add(x); isEmpty()=>false; getCount()=>1; empty(); isEmpty()=>true(); getCount()=>0
 */


#include "../../../config.h"

#ifdef ACCELERATED_TIME_PARAMETERS
// Parameter at compile time
static constexpr unsigned int SampleSetSize = 2;
static constexpr unsigned int MaxSampleSetIndex = (SampleSetSize-1);

#elif defined(PRODUCTION_PARAMETERS)
// Accelerated

// Require 3 consecutive sunrises to confirm
static constexpr unsigned int SampleSetSize = 3;
static constexpr unsigned int MaxSampleSetIndex = (SampleSetSize-1);
#else
#error "Production versus accelerated not defined"
#endif




class CircularBuffer {
private:


    /*
     * Instance data members are persistent if owning object is.
     * Usually they are persistent, we are buffering through a LPM45 reset.
     */
// head in range [0, SampleSetSize-1]
// Always points to most recent sample
unsigned int head;

// count in range [0, SampleSetSize]
unsigned int count;

EpochTime sampleSet[SampleSetSize]; // = {0,0};



void adjustHeadAndCount();


public:
    /*
     * Replacing oldest one if necessary.
     */
    void addSample(EpochTime sample);

    void empty();
    bool isFull();
    bool isEmpty();

    // Returns [0, sizeof SampleSet]
    unsigned int getCount();


    void startIter();
    /*
     * Returns EpochTime or zero after last sample
     */
    EpochTime nextIter();
};
