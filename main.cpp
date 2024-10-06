#include "readfile.h"
#include "disk.h"
#include "bptree.h"
#include <unordered_set>
#include <chrono>
#include <vector>
#include <set>

#include <iostream>

using namespace std;

void task1(Disk *disk, BPTree *tree){
    cout << "Task 1: Disk and Record Information" << endl;
    cout << "Total records loaded: " << readfile::readFileIntoDisk("games.txt", disk, tree) << endl;
    cout << "Size of each record: " << sizeof(Record) << " bytes" << endl;
    cout << "Records per block: " << disk->getBlkMaxRecords() << endl;
    cout << "Total blocks utilized for data storage: " << disk->getNumBlks() << endl;
}

void task2(BPTree *tree) {
    cout << "Task 2: B+ Tree Overview" << endl;
    cout << "Displaying B+ tree structure:" << endl;
    cout << " " << endl;
    tree->printTree();
    cout << "Max keys (n) in the B+ tree: " << tree->getMaxKeys() << endl;
    cout << "Total nodes in the B+ tree: " << tree->getNumNodes() << endl;
    cout << "Levels in the B+ tree: " << tree->getDepth() << endl;
    cout << "Contents of the Root Node: ";
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

    cout << "Task 3: Record Retrieval Statistics" << endl;
    cout << "Total index nodes accessed: " << tree->getNumNodesAcc() + leafNodesAccessed << endl;
    cout << "Number of data blocks accessed: " << uniqueBlockIds.size() << endl;
    cout << "Average FG3_PCT_home: " << total_FG3_PCT_home << endl;
    cout << "Time taken for retrieval: " << timeTaken.count() << " seconds" << endl;
    cout << endl;

}


void bruteForce(Disk* disk) {
    // Initialize counters for accessed blocks and records
    int numBlocksAccessed = 0;
    int numRecordsAccessed = 0;

    // Start timing the brute force search
    auto startTime = chrono::high_resolution_clock::now();

    // Retrieve the total number of blocks on the disk
    int totalBlocks = disk->getNumBlks();
    
    // Loop through each block in the disk
    for (size_t blockIdx = 0; blockIdx < totalBlocks; ++blockIdx) {
        // Increment the number of blocks accessed
        numBlocksAccessed++;

        // Loop through each record within the current block
        for (size_t recordOffset = 0; recordOffset < disk->getBlockSize(); recordOffset += sizeof(Record)) {
            // Retrieve the current record pointer
            Record* record = disk->getRecord(blockIdx, recordOffset);

            // Check if the record's fg_pct_home is within the desired range
            if (record->fg_pct_home >= 0.5 && record->fg_pct_home <= 0.8) {
                // Increment the count of accessed records
                numRecordsAccessed++;
            }
        }
    }

    // Stop timing the brute force search
    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsedTime = chrono::duration_cast<chrono::duration<double>>(endTime - startTime);

    // Output the results of the brute force search
    cout << "Brute Force Search Results:" << endl;
    cout << "Total number of data blocks accessed: " << numBlocksAccessed << endl;
    cout << "Total number of records accessed: " << numRecordsAccessed << endl;
    cout << "Elapsed time for brute force method: " << elapsedTime.count() << " seconds" << endl;
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