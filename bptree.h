#ifndef BPTREE_H
#define BPTREE_H

#include "constants.h"

class BPTree {
private:
    Node* root;
    int maxKeys;
    int numNodes;
    int depth;
    size_t blkSize;
    int numNodesAcc; // number of nodes accessed during operations

public:
    BPTree(int blkSize);

    ~BPTree();

    Node* getRoot() {return root;}
    
    int getMaxKeys() {return maxKeys;}

    int getNumNodes() {return numNodes;}

    int getDepth() {return depth;}

    void resetNumNodesAcc() { this->numNodesAcc = 0; }

    int getNumNodesAcc() { return numNodesAcc; }

    void printNode(Node* node);
    
    void printTree();

    void setRoot(Node *node);

    void insert(float key, Record *recordPtr);

    Node* splitLeafNode(Node* currNode);

    Node* splitInternalNode(Node* currNode, float* key);

    Node* searchNode(float key);

    std::vector<Record*>* searchRecord(float key);

};

#endif