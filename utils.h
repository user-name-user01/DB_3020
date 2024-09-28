#ifndef UTILS_H
#define UTILS_H

#include "disk.h"
#include "bptree.h"
#include "date.h"

#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

namespace utils {

    int readFileIntoDisk(std::string fileName, Disk* disk, BPTree* tree) {
        ifstream inputFile;
        inputFile.open(fileName);
        if (!inputFile.is_open()) {
            cout << "File failed to Open" << endl;
            return 0;
        }
        string line;
        int numOfRecords = 0;
        getline(inputFile, line);
        while (getline(inputFile, line)) {
            istringstream iss(line);
            string GAME_DATE_EST, TEAM_ID_home, PTS_home, FG_PCT_home, FT_PCT_home, FG3_PCT_home, AST_home, REB_home, HOME_TEAM_WINS;
            getline(iss, GAME_DATE_EST, '\t');
            getline(iss, TEAM_ID_home, '\t');
            getline(iss, PTS_home, '\t');
            getline(iss, FG_PCT_home, '\t');
            getline(iss, FT_PCT_home, '\t');
            getline(iss, FG3_PCT_home, '\t');
            getline(iss, AST_home, '\t');
            getline(iss, REB_home, '\t');
            getline(iss, HOME_TEAM_WINS, '\t');

            // Check if the record is relevant (PTS_home field should not be empty)
            if (PTS_home == "") 
                continue;          
            Record record = {
                stoul(TEAM_ID_home),
                stof(FG_PCT_home),
                stof(FT_PCT_home),
                stof(FG3_PCT_home),
                usint(date::dateStringToDaysElapsed(GAME_DATE_EST)),
                static_cast<char>(stoi(PTS_home)),
                static_cast<char>(stoi(AST_home)),
                static_cast<char>(stoi(REB_home)),
                HOME_TEAM_WINS == "1" ? true : false
            };
            Record* recordPtr = (*disk).writeRecord(record);
            tree->insert(record.fg_pct_home, recordPtr);
            numOfRecords++; 
        }
        inputFile.close(); // Close the input file
        return numOfRecords; // Return the total number of records processed
    }
}

#endif