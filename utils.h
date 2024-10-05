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
#include <cstring>

using namespace std;

namespace utils {

    // Function to read a file and load the records into disk and B+ tree.
    // The file is expected to be tab-separated, containing game data.
    // Returns the total number of valid records processed.
    int readFileIntoDisk(std::string fileName, Disk* disk, BPTree* tree) {
        ifstream inputFile;
        inputFile.open(fileName);
        if (!inputFile.is_open()) {
            cout << "File failed to Open" << endl;
            return 0;
        }
        string line;
        getline(inputFile, line);  // Skip the header line
        while (getline(inputFile, line)) {
            istringstream iss(line); // String stream for parsing the line
            string GAME_DATE_EST, TEAM_ID_home, PTS_home, FG_PCT_home, FT_PCT_home, FG3_PCT_home, AST_home, REB_home, HOME_TEAM_WINS;
            
            // Extract values from the tab-separated line
            getline(iss, GAME_DATE_EST, '\t');
            getline(iss, TEAM_ID_home, '\t');
            getline(iss, PTS_home, '\t');
            getline(iss, FG_PCT_home, '\t');
            getline(iss, FT_PCT_home, '\t');
            getline(iss, FG3_PCT_home, '\t');
            getline(iss, AST_home, '\t');
            getline(iss, REB_home, '\t');
            getline(iss, HOME_TEAM_WINS, '\t');

            if (PTS_home.empty() || FG_PCT_home.empty() || FT_PCT_home.empty() || FG3_PCT_home.empty()) {
                continue; // Skip this record if any critical field is empty
            }

            // Create a Record object using the parsed data
            Record record = {
                stoul(TEAM_ID_home),
                stof(FG_PCT_home),
                stof(FT_PCT_home),
                stof(FG3_PCT_home),
                usint(date::dateStringToDaysElapsed(GAME_DATE_EST)),
                static_cast<short>(stoi(PTS_home)),
                static_cast<short>(stoi(AST_home)),
                static_cast<short>(stoi(REB_home)),
                HOME_TEAM_WINS == "1" ? true : false
            };

            // cout << record.pts_home << " | " << endl;
            // Write the record to the disk and insert into the B+ tree
            Record* recordPtr = (*disk).writeRecord(record);
            
            if (recordPtr){
                tree->insert(record.fg_pct_home, recordPtr); // Insert into the tree if record was successfully written to disk
            }
        }
        inputFile.close(); // Close the input file
        return disk->getNoOfRecords(); // Return the total number of records processed
    }
}

#endif