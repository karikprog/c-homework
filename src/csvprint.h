#pragma once
#include <stdint.h>
#include <stdio.h>

typedef struct {
    char*** data;
    int rowsLen;
    int columsLen;
    int capacityRows;
    int capacityColums;
    uint16_t* maxValCol;
} Table;

Table* parseCsv(FILE* file);
void deleteTable(Table** table);
void printCsv(FILE* file, Table* table);