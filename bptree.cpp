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

// Print the B+ tree level by level using breadth-first traversal
void BPTree::printTree() {
    std::queue<Node*> q;
    q.push(root); // Start from the root node
    int level = 0; // Track the current level for labeling

    while (!q.empty()) {
        int n = q.size();
        std::cout << "Level " << level << ":" << std::endl; // Print level info
        for (int i = 0; i < n; i++) {
            Node* node = q.front();
            q.pop();
            
            // Print the current node with label
            if (node == root) {
                std::cout << "Node (Root): ";
            } else {
                std::cout << "Node: ";
            }
            printNode(node); // Print the node contents
            cout << endl;
            // If the node is not a leaf, enqueue its child nodes
            if (!node->isLeaf) {
                for (Node* child : node->ptrs) {
                    q.push(child);
                }
            }
        }
        std::cout << std::endl;
        level++; // Increment the level after processing all nodes at the current level
    }
}


void BPTree::setRoot(Node* r) {
    this->root = r;
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

    Node *currNode = this->root;
    std::vector<Node *> parentNodes;

    // Traverse until we find a leaf
    while (!currNode->isLeaf) {
        int idx = std::upper_bound(currNode->keys.begin(), currNode->keys.end(), key) - currNode->keys.begin();
        parentNodes.push_back(currNode);
        currNode = currNode->ptrs.at(idx);
    }

    // Insert key and record into the leaf node
    int idx = std::upper_bound(currNode->keys.begin(), currNode->keys.end(), key) - currNode->keys.begin();
    currNode->keys.insert(currNode->keys.begin() + idx, key);
    currNode->records.insert(currNode->records.begin() + idx, std::vector<Record *>(1, recordPtr));

    // If the leaf exceeds the max keys, split it
    while (currNode->keys.size() > this->maxKeys) {
        if (parentNodes.empty()) {
            Node* newRoot = new Node(false);  // New root for split
            this->numNodes++;
            this->depth++;
            this->root = newRoot;
            newRoot->ptrs.push_back(currNode);  // Assign current node to new root
        }

        Node* parentNode = parentNodes.empty() ? this->root : parentNodes.back();
        parentNodes.pop_back();

        Node* newNode = this->splitLeafNode(currNode);  // Split the leaf node

        float newKey = newNode->keys.front();
        int insertIdx = std::upper_bound(parentNode->keys.begin(), parentNode->keys.end(), newKey) - parentNode->keys.begin();
        
        parentNode->keys.insert(parentNode->keys.begin() + insertIdx, newKey);
        parentNode->ptrs.insert(parentNode->ptrs.begin() + insertIdx + 1, newNode);

        currNode = parentNode;

        // If the parent node needs to split, propagate upwards
        if (currNode->keys.size() > this->maxKeys && parentNode == this->root) {
            Node* newInternal = this->splitInternalNode(parentNode, &newKey);
            Node* newRoot = new Node(false);
            this->numNodes++;
            this->depth++;
            newRoot->keys.push_back(newKey);
            newRoot->ptrs.push_back(parentNode);
            newRoot->ptrs.push_back(newInternal);
            this->root = newRoot;
            break;
        }
    }
}

// Split a leaf node and return the new node
Node* BPTree::splitLeafNode(Node* currNode) {
    Node* splitNode = new Node(true);  // Create a new leaf node
    this->numNodes++;

    // Determine the splitting point
    int splitIndex = currNode->keys.size() / 2;

    // Move half of the keys and records from the current node to the new node
    splitNode->keys.assign(currNode->keys.begin() + splitIndex, currNode->keys.end());
    splitNode->records.assign(currNode->records.begin() + splitIndex, currNode->records.end());

    // Resize the current node to reflect the keys and records that remain
    currNode->keys.resize(splitIndex);
    currNode->records.resize(splitIndex);

    // Link the new node to the current node's next
    splitNode->nextNodePtr = currNode->nextNodePtr;
    currNode->nextNodePtr = splitNode;

    return splitNode;
}


// Split an internal node and return the new node
Node* BPTree::splitInternalNode(Node* currNode, float *key) {
    Node* splitNode = new Node(false);  // Create a new internal node
    this->numNodes++;

    // Determine the split index
    int splitIndex = this->maxKeys / 2;

    // Move half of the keys and pointers from currNode to splitNode
    splitNode->keys.assign(currNode->keys.begin() + splitIndex + 1, currNode->keys.end());
    splitNode->ptrs.assign(currNode->ptrs.begin() + splitIndex + 1, currNode->ptrs.end());

    // Resize the current node to reflect the keys and pointers that remain
    currNode->keys.resize(splitIndex);
    currNode->ptrs.resize(splitIndex + 1);  // One more pointer than keys

    // Update the split key (middle key) to be pushed up to the parent
    *key = currNode->keys[splitIndex];

    return splitNode;
}

// Search the node that might contain the key
Node* BPTree::searchNode(float key) {
    Node* curNode = getRoot();
    if (curNode == nullptr) {
        return nullptr;  // Return if the tree is empty
    }

    // Traverse the tree to find the leaf node
    while (!curNode->isLeaf) {
        this->numNodesAcc++;
        auto it = std::upper_bound(curNode->keys.begin(), curNode->keys.end(), key);
        int idx = std::distance(curNode->keys.begin(), it);  // Find the correct child
        curNode = curNode->ptrs.at(idx);  // Move to the next child node
    }

    this->numNodesAcc++;  // Account for accessing the leaf node
    return curNode;
}

// Search for a record with the specified key
std::vector<Record*>* BPTree::searchRecord(float key) {
    Node* curNode = getRoot();
    if (curNode == nullptr) {
        return nullptr;  // Return if the tree is empty
    }

    // Traverse the tree to find the leaf node
    while (!curNode->isLeaf) {
        this->numNodesAcc++;
        auto it = std::upper_bound(curNode->keys.begin(), curNode->keys.end(), key);
        int idx = std::distance(curNode->keys.begin(), it);  // Find the correct child
        curNode = curNode->ptrs.at(idx);  // Move to the next child node
    }

    this->numNodesAcc++;  // Account for accessing the leaf node

    // Search for the key in the leaf node using lower_bound
    auto it = std::lower_bound(curNode->keys.begin(), curNode->keys.end(), key);
    int idx = std::distance(curNode->keys.begin(), it);
    
    // Check if the key exists and return the corresponding records
    if (idx < curNode->keys.size() && curNode->keys.at(idx) == key) {
        return &(curNode->records.at(idx));  // Return records if key is found
    }

    return nullptr;  // Return null if key is not found
}
