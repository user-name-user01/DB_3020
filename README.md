## information

Ensure that games.txt is present in the same working directory as the codes. 

Before you start, make sure you have **MinGW64** installed with the **g++ compiler**. You can download MinGW from [MinGW-w64](http://mingw-w64.org/doku.php).

command to compile:

`g++ -o main main.cpp bptree.cpp disk.cpp`

then run .exe file

`./main` 

**main.exe** is an executable from three compiled files: *main.cpp* , *bptree.cpp* and *disk.cpp*

## File Descriptions

bptree.cpp:
Implements the B+ Tree data structure functionalities, including insertion, search, and traversal methods.

bptree.h:
Header file for bptree.cpp, containing the class definition for the B+ Tree and its associated methods and attributes.

disk.cpp:
Contains the implementation for disk operations, including reading and writing records to disk blocks.

disk.h:
Header file for disk.cpp, defining the disk operations and data structures required to manage disk storage.

games.txt:
A text file that contains the dataset of games, which is read and processed by the program.

constants.h:
Defines constants used throughout the program to ensure consistency and readability.

readfile.h:
Contains the function declarations for reading data from files, specifically for processing the games.txt file.

date.h:
Defines structures and methods for handling date-related operations, especially for the date field in games.txt.

main.cpp:
The main entry point of the application, orchestrating the program flow, including initializing the B+ Tree, reading data, and displaying results.

main.exe:
The compiled executable file created from main.cpp, bptree.cpp, and disk.cpp. Running this file executes the program.