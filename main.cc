//#include <iomanip>
#include <sstream>

#include <iostream>

#include <fstream>

#include <chrono>

#include <algorithm>

#include <vector>

#include <bits/stdc++.h>

#include "constants.h"

#include "disk.h"

using namespace std;

Disk * disk = new Disk();
uint32_t LAST_DATA_BLOCK;

void Experiment1(string filename) {

    cout << "" << '\n';
    cout << "Running Experiment 1" << endl;
    uint32_t numRecords = 0;
    uint32_t numDataBlocks = 0;
    uint32_t currentFreeBlock = FIRST_DATA_BLOCK;

    ifstream inputFile(filename);
    assert(inputFile.good());

    string line;
    getline(inputFile, line);

    DataBlock * block = (DataBlock * ) disk -> ReadBlock(currentFreeBlock); 
    uint32_t index = 0;
    int count = 0;

    size_t dateSize = sizeof(block -> records[index].GAME_DATE_EST);
    size_t primaryKeySize = sizeof(block -> records[index].FG_PCT_home);

    while (getline(inputFile, line)) {

        int column = 1;

        if (line.empty()) continue;
        numRecords++;

        istringstream ss(line);
        string substr;

        block -> records[index].occupied = true;

        while (getline(ss, substr, '\t')) {

            // cout << substr << '\n';

            switch (column) {
            case 1:
                memcpy(block -> records[index].GAME_DATE_EST, substr.c_str(), dateSize);
                // cout << block -> records[index].GAME_DATE_EST << '\n';
                break;
            case 2:
                block -> records[index].TEAM_ID_home = std::stoi(substr);
                // cout << block -> records[index].TEAM_ID_home << '\n';
                break;
            case 3:
                if (substr == "") break; // check for empty field
                block -> records[index].PTS_home = std::stoi(substr);
                // cout << block -> records[index].PTS_home << '\n';
                break;
            case 4:
                if (substr == "") break;
                memcpy(block -> records[index].FG_PCT_home, substr.c_str(), primaryKeySize);
                // cout << block -> records[index].FG_PCT_home << '\n';
                break;
            case 5:
                if (substr == "") break;
                block -> records[index].FT_PCT_home = std::stof(substr);
                // cout << block -> records[index].FT_PCT_home << '\n';
                break;
            case 6:
                if (substr == "") break;
                block -> records[index].FG3_PCT_home = std::stof(substr);
                // cout << block -> records[index].FG3_PCT_home << '\n';
                break;
            case 7:
                if (substr == "") break;
                block -> records[index].AST_home = std::stoi(substr);
                // cout << block -> records[index].AST_home << '\n';
                break;
            case 8:
                if (substr == "") break;
                block -> records[index].REB_home = std::stoi(substr);
                // cout << block -> records[index].REB_home << '\n';
                break;
            case 9:
                if (substr == "1") {
                    block -> records[index].HOME_TEAM_WINS = true;
                } else {
                    block -> records[index].HOME_TEAM_WINS = false;
                }
                // cout << block -> records[index].HOME_TEAM_WINS << '\n';
                break;
            default:
                cout << "column not found" << endl;
                break;
            }

            column++;
        }

        count++;
        ++index;
        if (index == RECORDS_PER_BLOCK) {
            disk -> WriteBlock(currentFreeBlock, (uint8_t * ) block);
            free(block);
            currentFreeBlock += 1;
            block = (DataBlock * ) disk -> ReadBlock(currentFreeBlock);
            index = 0;
        }
    }

    if (index != 0) {
        disk -> WriteBlock(currentFreeBlock, (uint8_t * ) block);
        currentFreeBlock += 1;
    }
    free(block);

    numDataBlocks = currentFreeBlock - 1;
    LAST_DATA_BLOCK = numDataBlocks;

    cout << "Statistics:" << endl;
    cout << "a) " << numRecords << " Records" << endl;
    cout << "b) Each record has a size of " << RECORD_SIZE << " Bytes" << endl;
    cout << "c) " << RECORDS_PER_BLOCK << " Records per Block" << endl;
    cout << "d) " << numDataBlocks << " Blocks used" << endl;
}

int main() {
    Experiment1("games.txt");
    delete disk;
}