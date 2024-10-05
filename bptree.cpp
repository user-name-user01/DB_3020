#include "bptree.h"

#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

// Constructor for initializing the B+ tree with block size
BPTree::BPTree(int blkSize) {
    this->root = nullptr; // Initialize root to nullptr
    this->blkSize = blkSize;
    this->maxKeys = (blkSize - sizeof(float*)) / (sizeof(float) + sizeof(float*)); // Calculate maximum keys a node can hold
    this->numNodes = 0;  // Initialize the number of nodes to 0
    this->depth = 0;  // Initialize the depth of the tree to 0
}

BPTree::~BPTree() {}

// Print the keys of a given node
void BPTree::printNode(Node* node) {
    cout << "[ ";
    for (float key : node->keys)
        cout << key << " ";
    cout << "]\n";
}

void BPTree::setRoot(Node* r) {
    this->root = r;
}

// Print the B+ tree level by level using breadth-first traversal
void BPTree::printTree() {
    std::queue<Node*> q;
    q.push(root); // Start from the root node
    while (!q.empty()) {
        int n = q.size();
        for (int i = 0; i < n; i++) {
            Node* node = q.front();
            q.pop();
            printNode(node); // Print the current node
            // If the node is not a leaf, enqueue its child nodes
            if (!node->isLeaf) {
                for (Node* child : node->ptrs)
                    q.push(child);
            }
        }
        std::cout << std::endl;
    }
}

// Insert a key and corresponding record into the B+ tree
void BPTree::insert(float key, Record *recordPtr) {
    // Check if the key already exists in the tree
    std::vector<Record *> *records = this->searchRecord(key);
    if (records != nullptr) {
        records->push_back(recordPtr);  // Add record to existing key
        return;
    }
    // If the tree is empty, create a new root node
    if (this->root == nullptr) {
        this->root = new Node(true);  // Create a new leaf node
        this->numNodes++;
        this->depth++;
        this->root->keys.push_back(key);  // Insert the key
        this->root->records.push_back(std::vector<Record *>(1, recordPtr));  // Add record
        this->root->nextNodePtr = nullptr;
        return;
    }
    // Traverse the tree to find the leaf node to insert the key
    Node *currNode = this->root;  // Start from the root node
    std::vector<Node *> parNodes(1, nullptr);  // Store parent nodes
    int idx = 0;
    // Traverse until a leaf node is found
    while (!currNode->isLeaf) {
        idx = std::upper_bound(currNode->keys.begin(), currNode->keys.end(), key) - currNode->keys.begin();
        parNodes.push_back(currNode);  // Store parent node
        currNode = currNode->ptrs.at(idx);  // Move to the next child node
    }
    // Insert key and record into the leaf node
    idx = std::upper_bound(currNode->keys.begin(), currNode->keys.end(), key) - currNode->keys.begin();
    currNode->keys.insert(currNode->keys.begin() + idx, key);
    currNode->records.insert(currNode->records.begin() + idx, std::vector<Record *>(1, recordPtr));
    // If the leaf node exceeds maximum keys, split it
    if (currNode->keys.size() > this->maxKeys) {
        Node* newNode = this->splitLeafNode(currNode);  // Split leaf node
        Node* parNode = parNodes.back();  // Get the parent node
        parNodes.pop_back();
        key = newNode->keys.front();  // Get the new key
        // Adjust parent nodes if they exceed max keys
        while (parNode != nullptr && parNode->keys.size() == this->maxKeys) {
            idx = std::upper_bound(parNode->keys.begin(), parNode->keys.end(), key) - parNode->keys.begin();
            parNode->keys.insert(parNode->keys.begin() + idx, key);
            parNode->ptrs.insert(parNode->ptrs.begin() + idx + 1, newNode);
            newNode = this->splitInternalNode(parNode, &key);  // Split internal node
            currNode = parNode;
            parNode = parNodes.back();
            parNodes.pop_back();
        }
        // If root node needs to be split, create a new root
        if (parNode == nullptr) {
            parNode = new Node(false);  // Create new root node
            this->numNodes++;
            parNode->keys.push_back(key);
            parNode->ptrs.push_back(currNode);
            parNode->ptrs.push_back(newNode);
            this->root = parNode;
            this->depth++;
            return;
        } else {
            idx = std::upper_bound(parNode->keys.begin(), parNode->keys.end(), key) - parNode->keys.begin();
            parNode->keys.insert(parNode->keys.begin() + idx, key);  // Insert key in parent
            parNode->ptrs.insert(parNode->ptrs.begin() + idx + 1, newNode);  // Insert new node
        }
    }
}

// Split a leaf node and return the new node
Node* BPTree::splitLeafNode(Node* currNode) {
    Node* splitNode = new Node(true);  // Create a new leaf node
    this->numNodes++;
    // Move half the keys and records to the new node
    for (int i = 0; i < (this->maxKeys + 1) / 2; i++) {
        splitNode->keys.insert(splitNode->keys.begin(), currNode->keys.back());
        currNode->keys.pop_back();
        splitNode->records.insert(splitNode->records.begin(), currNode->records.back());
        currNode->records.pop_back();
    }
    splitNode->nextNodePtr = currNode->nextNodePtr;  // Link new node to next node
    currNode->nextNodePtr = splitNode;  // Update current node's next pointer
    return splitNode;
}

// Split an internal node and return the new node
Node* BPTree::splitInternalNode(Node* currNode, float *key) {
    Node* splitNode = new Node(false);  // Create a new internal node
    this->numNodes++;
    // Move half the keys and pointers to the new node
    for (int i = 0; i < this->maxKeys / 2; i++) {
        splitNode->keys.insert(splitNode->keys.begin(), currNode->keys.back());
        currNode->keys.pop_back();
        splitNode->ptrs.insert(splitNode->ptrs.begin(), currNode->ptrs.back());
        currNode->ptrs.pop_back();
    }
    splitNode->ptrs.insert(splitNode->ptrs.begin(), currNode->ptrs.back());  // Move last pointer
    currNode->ptrs.pop_back();
    *key = currNode->keys.back();  // Update split key
    currNode->keys.pop_back();
    return splitNode;
}

// Search the node that might contain the key
Node* BPTree::searchNode(float key) {
    Node* curNode = getRoot();
    if (curNode == nullptr)
        return nullptr;  // Return if tree is empty
    this->numNodesAcc++;
    int idx;
    while (!curNode->isLeaf) {  // Traverse until reaching a leaf node
        idx = std::upper_bound(curNode->keys.begin(), curNode->keys.end(), key) - curNode->keys.begin();
        curNode = curNode->ptrs.at(idx);  // Move to child node
        this->numNodesAcc++;
    }
    return curNode;
}

// Search for a record with the specified key
std::vector<Record*>* BPTree::searchRecord(float key) {
    Node* curNode = getRoot();
    if (curNode == nullptr)
        return nullptr;  // Return if tree is empty
    this->numNodesAcc++;
    int idx;
    // Traverse until reaching a leaf node
    while (!curNode->isLeaf) {
        idx = std::upper_bound(curNode->keys.begin(), curNode->keys.end(), key) - curNode->keys.begin();
        curNode = curNode->ptrs.at(idx);  // Move to child node
        this->numNodesAcc++;
    }
    // Search for the key in the leaf node
    idx = std::lower_bound(curNode->keys.begin(), curNode->keys.end(), key) - curNode->keys.begin();
    if (idx < curNode->keys.size() && curNode->keys.at(idx) == key)
        return &(curNode->records.at(idx));  // Return records if key is found
    return nullptr;
}