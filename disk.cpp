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

// This function checks if there is enough space to allocate a new block.
// If there is space, it increments the number of used blocks and resets the memory used in the current block.
bool Disk::allocateBlock() {
    // Check if adding another block would exceed the total disk size.
    if (blockSize * (numOfUsedBlks + 1) > size) {
        cout << "Memory full" << endl;           // Print error if memory is full
        return false;                            // Return false if block allocation fails
    }
    numOfUsedBlks++;                             // Increment number of used blocks
    currentBlkUsedMem = 0;                       // Reset current block's used memory
    return true;
}

/// This function writes a record to a disk block and returns the record's address.
// If there's insufficient space in the current block, a new block is allocated.
Record* Disk::writeRecord(Record record) {
    // Check if the record fits into the current block, else allocate a new block.
    if (currentBlkUsedMem + recordSize > blockSize) {
        if (!allocateBlock())                    // If block allocation fails, return nullptr
            return nullptr;
    }
    // Calculate the address of the record in the current block.
    Record* recordAddress = reinterpret_cast<Record*>(startAddress + numOfUsedBlks * blockSize + currentBlkUsedMem);
    if (numOfUsedBlks == 0)                      // If no blocks are used yet, increment the count
        numOfUsedBlks++;
    currentBlkUsedMem += recordSize;             // Update used memory in the block
    *recordAddress = record;                     // Write the record to the calculated address
    noOfRecords++;                               // Increment the number of records
    return recordAddress;                        
}

// This function returns the block ID of a given record based on its address in memory.
int Disk::getBlockId(Record* record) {
    uchar* recordBytes = reinterpret_cast<uchar*>(record);   // Convert record pointer to byte pointer
    ptrdiff_t offset = recordBytes - startAddress;           // Calculate the offset from the start address
    int blockId = offset / blockSize;                        // Compute the block ID
    return blockId;                                          
}

// This function retrieves a record by its block index and offset within the block.
Record* Disk::getRecord(int blockIdx, size_t recordOffset) {
    size_t offset = blockIdx * blockSize + recordOffset;      // Calculate the byte offset for the record
    return reinterpret_cast<Record*>(startAddress + offset);  // Return the address of the record
}
