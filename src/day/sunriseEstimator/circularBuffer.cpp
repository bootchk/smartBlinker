
#include "circularBuffer.h"




namespace {

#pragma PERSISTENT
unsigned int head;  // Always points to most recent sample

#pragma PERSISTENT
unsigned int count = 0;

#pragma PERSISTENT
EpochTime sampleSet[3]; //  = 0;


// not persistent
unsigned int iterIndex;
unsigned int iterCount;



void adjustHeadAndCount() {
    head++;
    if (head > 2 )  head = 0;

    // Max count is 3
    if (count < 3) count++;
}

void adjustIterIndexandCount() {
    count--;
    iterIndex++;
    if (iterIndex > 2) iterIndex = 0;
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



bool CircularBuffer::isFull(){ return count == 3; }
bool CircularBuffer::isEmpty(){ return count == 0; }
unsigned int CircularBuffer::getCount(){ return count; }

void CircularBuffer::startIter(){
    iterIndex = head;
    iterCount = count;
}

EpochTime CircularBuffer::nextIter(){
    EpochTime result;

    if (iterCount == 0)
        result = 0;
    else {
        result = sampleSet[iterIndex];
        count--;
        adjustIterIndexandCount();
    }
    return result;
}

