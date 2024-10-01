#include "disk.h"

#include <cstdlib>
#include <iostream>

using namespace std;

Disk::Disk(size_t size, size_t blockSize, size_t recordSize) {
    startAddress = (uchar*)malloc(size);
    this->size = size;
    this->blockSize = blockSize;
    this->recordSize = recordSize;
    numOfUsedBlks = 0;
    currentBlkUsedMem = 0;
    noOfRecords = 0;
}

Disk::~Disk() {
    free(startAddress);
};

// This function checks if there is enough memory available to allocate a new block.
// If there is enough memory, it increments the number of used blocks and resets the current block's used memory to zero.
// If there is not enough memory, it prints an error message and returns false.
bool Disk::allocateBlock() {
    // Check if allocating a new block will exceed the total size of the disk.
    if (blockSize * (numOfUsedBlks + 1) > size) {
        cout << "Memory full" << endl;
        return false;
    }
    numOfUsedBlks++;
    currentBlkUsedMem = 0;
    return true;
}

// This function writes a record to a disk block and returns the address of the record.
// If there is not enough space in the current block, a new block is allocated.
Record* Disk::writeRecord(Record record) {
    // Check if there is enough space in the current block for the record.
    if (currentBlkUsedMem + recordSize > blockSize) {
        if (!allocateBlock())
            return nullptr;
    }
    // Calculate the address of the record within the block.
    Record* recordAddress = reinterpret_cast<Record*>(startAddress + numOfUsedBlks * blockSize + currentBlkUsedMem);
    if (numOfUsedBlks == 0)
        numOfUsedBlks++;
    currentBlkUsedMem += recordSize;
    *recordAddress = record;
    noOfRecords++;
    return recordAddress;
}

int Disk::getBlockId(Record* record){
    //this function takes a pointer to a record and returns an int value which represents the block ID of the 
    //record in the disk storage system
    uchar* recordBytes = reinterpret_cast<uchar*>(record);
    ptrdiff_t offset = recordBytes - startAddress;
    int blockId = offset / blockSize;
    return blockId;
}

Record* Disk::getRecord(int blockIdx, size_t recordOffset){
    // in this function, we use the block index and record offset to determine the address
    // of the corresponding record
    size_t offset = blockIdx * blockSize + recordOffset;
    return reinterpret_cast<Record *>(startAddress + offset);
}
