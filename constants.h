#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

#include <string.h>

#include <assert.h>

#include <stdio.h>

#include <math.h>

#include <string>

#define FIRST_DATA_BLOCK 1
#define DISK_SIZE 100 * 1024 * 1024
#define RECORD_SIZE sizeof(Record)
#define BLOCK_SIZE sizeof(DataBlock)
#define NUM_BLOCKS DISK_SIZE / BLOCK_SIZE
#define RECORDS_PER_BLOCK BLOCK_SIZE / RECORD_SIZE

#define ASSERT(cond, msg, args...) assert((cond) || !fprintf(stderr, (msg "\n"), args))

typedef struct Record {
    bool occupied;
    char GAME_DATE_EST[9];
    uint32_t TEAM_ID_home;
    uint16_t PTS_home;
    char FG_PCT_home[5];
    float FT_PCT_home;
    float FG3_PCT_home;
    uint16_t AST_home;
    uint16_t REB_home;
    bool HOME_TEAM_WINS;
}
Record;

typedef struct DataBlock {
    Record records[10];
}
DataBlock;

typedef struct RecordPointer {
    uint32_t blockNumber: 28;
    uint32_t recordIndex: 4;
}
RecordPointer;

#endif