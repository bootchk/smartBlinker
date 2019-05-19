
#include "circularBuffer.h"




namespace {

#pragma PERSISTENT
unsigned int head;  // Always points to most recent sample

#pragma PERSISTENT
unsigned int count = 0;



#pragma PERSISTENT
EpochTime sampleSet[3]; //  = 0;


// After adding at head.
void adjustHeadAndCount() {
    head++;
    if (head > 2 )  head = 0;

    // Max count is 3
    if (count < 3) count++;
}

}



namespace iter {

// not persistent
unsigned int index;
unsigned int count;

void adjustIterIndexandCountAfterNextIter() {
    iter::count--;
    iter::index++;
    if (iter::index > 2) iter::index = 0;
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

