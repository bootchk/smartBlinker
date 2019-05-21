
#include "circularBuffer.h"




namespace {

// head in range [0, SampleSetSize-1]
#pragma PERSISTENT
unsigned int head = 0;  // Always points to most recent sample

// count in range [0, SampleSetSize]
#pragma PERSISTENT
unsigned int count = 0;


//#define SampleSetSize 3
#define SampleSetSize 2
#define MaxSampleSetIndex (SampleSetSize - 1)



#pragma PERSISTENT
EpochTime sampleSet[SampleSetSize]; //  = 0;


// Before adding at head.
void adjustHeadAndCount() {
    head++;
    if (head > MaxSampleSetIndex )  head = 0;

    // Max count is 3
    if (count < SampleSetSize) count++;
}

}



namespace iter {

// not persistent
unsigned int index;
unsigned int count;

void adjustIterIndexandCountAfterNextIter() {
    iter::count--;
    iter::index++;
    if (iter::index > MaxSampleSetIndex) iter::index = 0;
}

}





void CircularBuffer::empty(){
    head = 0;
    count = 0;
}

void CircularBuffer::addSample(EpochTime sample){
    adjustHeadAndCount();
    sampleSet[head] = sample;
}



bool CircularBuffer::isFull(){ return count == SampleSetSize; }
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

