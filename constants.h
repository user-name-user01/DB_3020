#ifndef STRUCTS_H
#define STRUCTS_H

#include <vector>
#include <string>

typedef unsigned short int usint; // 16-bit integer
typedef unsigned int uint; // 32-bit integer
typedef unsigned char uchar; // 8-bit value
typedef unsigned char uint8; // 8-bit unsigned integer

// Columns in record is rearranged to account for padding
struct Record {
    uint team_id_home; // 4 bytes
    float fg_pct_home; // 4 bytes
    float ft_pct_home; // 4 bytes
    float fg3_pct_home; // 4 bytes
    usint game_date_int; // number of days that elapsed since 01/01/1900 - 2 bytes
    short pts_home; // 2 bytes
    short ast_home; // 2 bytes
    short reb_home; // 2 bytes
    bool home_team_wins; // 1 byte
}; // 3 bytes padding after home_team_wins

// Structure representing a node in a B+ tree
struct Node {
    bool isLeaf;                                 // Flag indicating if the node is a leaf node
    std::vector<float> keys;                     // Keys in the node
    std::vector<Node*> ptrs;                     // Pointers to child nodes (for internal nodes)
    std::vector<std::vector<Record*>> records;   // Pointers to records (only for leaf nodes)
    Node* nextNodePtr;                           // Pointer to the next leaf node (only for leaf nodes)

    Node(bool isLeaf) {                          // Constructor to initialize the node type
        this->isLeaf = isLeaf;
    }
};
#endif