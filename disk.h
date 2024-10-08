#ifndef DISK_H
#define DISK_H

#include <string>
#include <math.h>
#include <vector>
#include "constants.h"
 
class Disk {
private:
    uchar* startAddress;
    size_t size;
    size_t blockSize;
    size_t recordSize;
    uint numOfUsedBlks;
    size_t currentBlkUsedMem;
    uint noOfRecords;

public:
    Disk(size_t size, size_t blockSize, size_t recordSize);

    ~Disk();

    int getNumBlks() {return numOfUsedBlks;}

    int getBlockSize() {return blockSize;} 

    int getBlkMaxRecords() {return floor(blockSize / recordSize);}

    int getNoOfRecords(){ return noOfRecords;}

    bool allocateBlock();

    Record* writeRecord(Record record);

    int getBlockId(Record *record);

    Record *getRecord(int blockIdx, size_t recordOffset); 
};

#endif