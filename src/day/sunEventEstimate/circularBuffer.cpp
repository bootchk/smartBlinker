
#include "../../parameters.h"   // SampleSetSize

// msp430Drivers
#include <assert/myAssert.h>
#include <src/day/sunEventEstimate/circularBuffer.h>










// Before adding at head.
void CircularBuffer::adjustHeadAndCount() {
    this->head++;
    if (head > Parameters::MaxSampleSetIndex )  head = 0;

    // Max count is SampleSetSize
    if (count < Parameters::SampleSetSize) count++;

    // head points to most recent sample
}





namespace iter {

// not persistent
unsigned int index;
unsigned int count;

/*
 * Called after getting nextIter.
 * Counts down, iterates in reverse order of add.
 */
void adjustIterIndexandCountAfterNextIter() {
    iter::count--;

    // Iter in reverse.  Modulo.
    if (iter::index == 0)
        iter::index = Parameters::MaxSampleSetIndex;
    else
        iter::index--;
}

}




//#pragma PERSISTENT
//unsigned int CircularBuffer::head; //{0};  // Always points to most recent sample

//#pragma PERSISTENT
//unsigned int CircularBuffer::count; // = 0;  // Always points to most recent sample

//#pragma PERSISTENT
//EpochTime CircularBuffersampleSet[Parameters::SampleSetSize] = {0,0};



void CircularBuffer::empty(){
    head = 0;
    count = 0;
    myAssert(isEmpty());
}

void CircularBuffer::addSample(EpochTime sample){
    adjustHeadAndCount();
    sampleSet[head] = sample;
    myAssert(not isEmpty());
}



bool CircularBuffer::isFull(){ return count == Parameters::SampleSetSize; }
bool CircularBuffer::isEmpty(){ return count == 0; }
unsigned int CircularBuffer::getCount(){ return count; }







void CircularBuffer::startIter(){
    // iteration starts at most recent sample
    iter::index = head;
    iter::count = count;
}


EpochTime CircularBuffer::nextIter(){
    EpochTime result;

    if (iter::count == 0)
        result = 0;
    else {
        result = sampleSet[iter::index];
        iter::adjustIterIndexandCountAfterNextIter();
    }
    return result;
}

