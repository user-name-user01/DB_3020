#include "utils.h"
#include "disk.h"
#include "bptree.h"
#include <unordered_set>

#include <iostream>

using namespace std;

void experiment1(Disk *disk, BPTree *tree){
    cout << "Experiment 1:" << endl;
    cout << "Number of records: " << utils::readFileIntoDisk("games.txt", disk, tree) << endl;
    cout << "Size of a record: " << sizeof(Record) << " bytes" << endl;
    cout << "Number of records stored in a block: " << disk->getBlkMaxRecords() << endl;
    cout << "Number of blocks used to store data in the disk: " << disk->getNumBlks() << endl;
}

void experiment2(BPTree *tree) {
    cout << "Experiment 2:" << endl;
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

int main() {
    Disk* disk = new Disk(500000000, 4096, sizeof(Record));
    BPTree* tree = new BPTree(4096);
    cout << endl;
    experiment1(disk, tree);
    cout << endl;
    experiment2(tree);
    cout << endl;

    delete tree;
    delete disk;
}