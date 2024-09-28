#ifndef STRUCTS_H
#define STRUCTS_H

#include <vector>

typedef unsigned short int usint; //16-bit integer
typedef unsigned int uint; //32-bit integer
typedef unsigned char uchar; //8-bit value

// Columns in record is rearranged to account for padding
struct Record {
    uint team_id_home; // 4 bytes
    float fg_pct_home; // 4 bytes
    float ft_pct_home; // 4 bytes
    float fg3_pct_home; // 4 bytes
    usint game_date_int; // number of days that elapsed since 01/01/1900 - 2 bytes
    char pts_home; // 1 byte
    char ast_home; // 1 byte
    char reb_home; //1 byte
    bool home_team_wins; // 1 byte
};

struct Node {
    bool isLeaf;
    std::vector<float> keys; 
    std::vector<Node*> ptrs; // pointers to blocks
    std::vector<std::vector<Record*>> records; // pointers to records (only for leaf nodes)
    Node* nextNodePtr; // pointer to next node (only for leaf nodes)

    Node(bool isLeaf) {
        this->isLeaf = isLeaf;
    }
};

#endif 