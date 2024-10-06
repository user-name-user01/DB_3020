#include "disk.h"

#include <cstdlib>
#include <iostream>

using namespace std;

// Constructor: Initializes the disk with given size, block size, and record size.
Disk::Disk(size_t size, size_t blockSize, size_t recordSize) {
    startAddress = (uchar*)malloc(size);         // Allocates memory for the disk
    this->size = size;                           // Sets the total size of the disk
    this->blockSize = blockSize;                 // Sets the size of each block
    this->recordSize = recordSize;               // Sets the size of each record
    numOfUsedBlks = 0;                           // Initializes the number of used blocks to zero
    currentBlkUsedMem = 0;                       // Initializes the memory used in the current block to zero
    noOfRecords = 0;                             // Initializes the number of records to zero
}

// Destructor: Frees the allocated memory for the disk.
Disk::~Disk() {
    free(startAddress);
};

// This function checks if there is enough space to allocate a new block. If there is space, it increments the number of used blocks and resets the memory used in the current block.
bool Disk::allocateBlock() {
    // Check if there is enough space for another block allocation
    if ((numOfUsedBlks + 1) * blockSize > size) {
        cout << "Memory full" << endl;  // Output an error message if disk memory is full
        return false;                   // Return false indicating allocation failure
    }
    // Update the disk status after successful block allocation
    numOfUsedBlks++;                    
    currentBlkUsedMem = 0;             
    return true;                       
}

/// This function writes a record to a disk block and returns the record's address. If there's insufficient space in the current block, a new block is allocated.
Record* Disk::writeRecord(Record record) {
    // Check if the record fits into the current block, else allocate a new block.
    if (currentBlkUsedMem + recordSize > blockSize) {
        if (!allocateBlock())                    // If block allocation fails, return nullptr
            return nullptr;
    }
    // Calculate the address of the record in the current block.
    Record* recordAddress = reinterpret_cast<Record*>(startAddress + numOfUsedBlks * blockSize + currentBlkUsedMem);
    if (numOfUsedBlks == 0){
        numOfUsedBlks++;
    }
    *recordAddress = record;                     // Write the record to the calculated address                     
    currentBlkUsedMem += recordSize;             // Update used memory in the block
    noOfRecords++;                               // Increment the number of records
    return recordAddress;                        
}


// This function returns the block ID of a given record based on its address in memory.
int Disk::getBlockId(Record* record) {
    // Calculate the offset of the record from the start address in bytes
    ptrdiff_t offset = reinterpret_cast<uchar*>(record) - startAddress;
    
    // Compute and return the block ID by dividing the offset by the block size
    return offset / blockSize;
}


// This function retrieves a record by its block index and offset within the block.
Record* Disk::getRecord(int blockIdx, size_t recordOffset) {
    // Calculate the byte offset by multiplying the block index by block size and adding the record offset
    size_t offset = blockIdx * blockSize + recordOffset;
    
    // Return the address of the record by adding the offset to the start address
    return reinterpret_cast<Record*>(startAddress + offset);
}

