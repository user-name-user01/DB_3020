#ifndef DATE_H
#define DATE_H

#include "disk.h"
#include "bptree.h"

#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

namespace date {
    usint dateStringToDaysElapsed(const std::string& dateString) {

        // Split the date string (format: dd/mm/yyyy)
        int day, month, year;
        char delimiter;

        std::istringstream dateStream(dateString);
        dateStream >> day >> delimiter >> month >> delimiter >> year;

        // Define a tm structure for the given date
        std::tm given_date = {};
        given_date.tm_mday = day;
        given_date.tm_mon = month - 1; // month - 1 as UNIX Jan is 0
        given_date.tm_year = year - 1900; // years since 1900

        // Arithmetic time conversion
        time_t givenTime = mktime(&given_date);
        
        // debugging date format
        // cout << given_date.tm_mday << "/" << given_date.tm_mon << "/" << given_date.tm_year << endl;

        // cout << givenTime / (60 * 60 * 24) << endl;
        return givenTime / (60 * 60 * 24);  // seconds to days
    }
} 

#endif

