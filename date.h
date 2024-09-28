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
        std::tm date = {};
        date.tm_mday = day;
        date.tm_mon = month - 1;  // months since January (0-11)
        date.tm_year = year - 1900;  // years since 1900

        // Convert the time_t to epoch date 
        std::time_t givenTime = std::mktime(&date);

        // Define the epoch time (1st January 1900)
        std::tm epoch = {};
        epoch.tm_mday = 1;
        epoch.tm_mon = 0;
        epoch.tm_year = 0;  // 1900

        // Convert the epoch date to time_t
        std::time_t epochTime = std::mktime(&epoch);

        // Calculate the difference in seconds and convert to days
        double difference = std::difftime(givenTime, epochTime);
        return difference / (60 * 60 * 24);  // seconds to days
    }
} 

#endif

