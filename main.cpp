#include "utils.h"
#include "disk.h"
#include "bptree.h"
#include <unordered_set>
#include <chrono>
#include <vector>
#include <set>

#include <iostream>

using namespace std;

void task1(Disk *disk, BPTree *tree){
    cout << "Task 1:" << endl;
    cout << "Number of records: " << utils::readFileIntoDisk("games.txt", disk, tree) << endl;
    cout << "Size of a record: " << sizeof(Record) << " bytes" << endl;
    cout << "Number of records stored in a block: " << disk->getBlkMaxRecords() << endl;
    cout << "Number of blocks used to store data in the disk: " << disk->getNumBlks() << endl;
}

void task2(BPTree *tree) {
    cout << "Task 2:" << endl;
    cout << "Printing B+ tree out:" << endl;
    cout << " " << endl;
    tree->printTree();
    cout << "Parameter n of the B+ tree = " << tree->getMaxKeys() << endl;
    cout << "Number of nodes of the B+ tree = " << tree->getNumNodes() << endl;
    cout << "Number of levels of the B+ tree = " << tree->getDepth() << endl;
    cout << "Content of Root Node of the B+ tree: ";
    tree->printNode(tree->getRoot());
    cout << endl;
}

void task3(Disk *disk, BPTree *tree) {
    vector<Record *> result;
    vector<int> blockIds;
    vector<float> keys;
    float lower = 0.5;
    float upper = 0.8;
    int lowerIdx, upperIdx, leafNodesAccessed = 0;
    bool searching = true;
    bool firstNode = true;
    tree->resetNumNodesAcc();

    chrono::high_resolution_clock::time_point before = chrono::high_resolution_clock::now();
    Node *resultNode = tree->searchNode(lower);
    while (searching) {
        keys = resultNode->keys;
        if (firstNode) {
            lowerIdx = lower_bound(keys.begin(), keys.end(), lower) - keys.begin();
            firstNode = false;  // Mark that we have handled the first node
        } else {
            lowerIdx = 0;  // For subsequent nodes, start from the first key
        }
        upperIdx = lower_bound(keys.begin(), keys.end(), upper) - keys.begin();

        for (int i=lowerIdx; i<=upperIdx-1; i++) {
            result.insert(result.end(), resultNode->records[i].begin(), resultNode->records[i].end());
        }
        upperIdx = upperIdx == keys.size() ? upperIdx - 1 : upperIdx;
        if (keys.at(upperIdx) >= upper) {
            searching = false;
        } else {
            resultNode = resultNode->nextNodePtr;
            if (resultNode == nullptr) {
                break;
            }
            leafNodesAccessed++;
        }
    }
    chrono::high_resolution_clock::time_point after = chrono::high_resolution_clock::now();
    chrono::duration<double> timeTaken = chrono::duration_cast<chrono::duration<double>>(after - before);

    for (Record* r : result) {
        int blockId = disk->getBlockId(r); // Get the block ID for the record
        blockIds.push_back(blockId); // Store the block ID in blockIds vector
    }

    float total_FG3_PCT_home = 0;

    for (int i=0; i<result.size(); i++) {
        total_FG3_PCT_home = total_FG3_PCT_home + result[i]->fg3_pct_home;
    }
    total_FG3_PCT_home /= result.size();
    std::set<int> uniqueBlockIds(blockIds.begin(), blockIds.end()); // to retrieve unique blockIds from the vector

    cout << "Task 3:" << endl;
    cout << "Number of index nodes accessed = " << tree->getNumNodesAcc() + leafNodesAccessed << endl;
    cout << "Number of data blocks accessed = " << uniqueBlockIds.size() << endl;
    cout << "Average FG3_PCT_home = " << total_FG3_PCT_home << endl;
    cout << "Running time for retrieval process = " << timeTaken.count() << "s" << endl;
    cout << endl;
}

void bruteForce(Disk* disk) {
    
    // Initialize variables to keep track of the number of blocks and records accessed
    int numBlksAcc = 0;
    int numRecordsAcc = 0;
    // Start the timer
    chrono::high_resolution_clock::time_point before = chrono::high_resolution_clock::now();
    // Get the number of blocks in the disk
    int numBlks = disk->getNumBlks();
    // Get the size of a single record
    size_t recordSize = sizeof(Record);
    // Loop through each block in the disk
    for (size_t blockIdx = 0; blockIdx < numBlks; ++blockIdx) {
        // Increment the number of blocks accessed
        numBlksAcc++;
        
        // Loop through each record in the block
        for (size_t recordOffset = 0; recordOffset < disk->getBlockSize(); recordOffset += sizeof(Record)) {
            // Get the current record
            Record* record = disk->getRecord(blockIdx, recordOffset);
            
            if (0.5 <= record->fg_pct_home && record->fg_pct_home <= 0.8) {
                // Increment the number of records accessed
                numRecordsAcc++;
                // Continue to the next record
                continue;
            }
        }
    }
    // Stop the timer
    chrono::high_resolution_clock::time_point after = chrono::high_resolution_clock::now();
    // Calculate the time taken by the brute force method
    chrono::duration<double> bruteTimeTaken = chrono::duration_cast<chrono::duration<double>>(after - before);
    
    // Print the number of data blocks accessed
    cout << "Number of data blocks accessed by brute force method = " << numBlksAcc << endl;
    // Print the running time of the brute force method
    cout << "Running time for accessed by brute force method = " << bruteTimeTaken.count() << "s" << endl;
}


int main() {
    
    Disk* disk = new Disk(500000000, 512, sizeof(Record)); // random disk size provided, but block size is determined via computer.
    BPTree* tree = new BPTree(512);
    cout << endl;
    task1(disk, tree);
    cout << endl;
    task2(tree);
    cout << endl;
    task3(disk, tree);
    bruteForce(disk);
    cout << endl;
    
    delete tree;
    delete disk;
}