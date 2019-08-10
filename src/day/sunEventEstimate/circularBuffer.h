#pragma once

#include "../../parameters.h"   // SampleSetSize


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

class CircularBuffer {
private:


// head in range [0, SampleSetSize-1]
// Always points to most recent sample
//#pragma PERSISTENT
unsigned int head;

// count in range [0, SampleSetSize]
// not static
unsigned int count;

EpochTime sampleSet[Parameters::SampleSetSize]; // = {0,0};



void adjustHeadAndCount();


public:
    /*
     * Adds sample, replacing oldest one if necessary.
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
