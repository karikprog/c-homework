#include "csvprint.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Return -1 on memory allocation error
int reallocTableRows(Table* table, int rowIndex)
{
    if (rowIndex + 1 > table->capacityRows) {
        table->capacityRows *= 2;
        uint16_t* tempMaxvalCount = realloc(table->maxValCol, table->capacityRows * sizeof(uint16_t));
        if (tempMaxvalCount == NULL) {
            return -1;
        } else {
            table->maxValCol = tempMaxvalCount;
            for (int i = table->capacityRows / 2; i < table->capacityRows; i++) {
                table->maxValCol[i] = 0;
            }
        }
        for (int i = 0; i < table->capacityColums; i++) {
            char** tempPtr = realloc(table->data[i], table->capacityRows * sizeof(char*));
            if (tempPtr == NULL) {
                return -1;
            } else {
                table->data[i] = tempPtr;
                for (int j = table->capacityRows / 2; j < table->capacityRows; j++) {
                    table->data[i][j] = NULL;
                }
            }
        }
    }
    return 0;
}
// Return -1 on memory allocation error
int realloctableColls(Table* table, int colLen)
{
    if (colLen + 1 > table->capacityColums) {
        table->capacityColums *= 2;
        char*** tempColPtr = realloc(table->data, table->capacityColums * sizeof(char**));
        if (tempColPtr == NULL) {
            return -1;
        } else {
            table->data = tempColPtr;
        }
        for (int i = table->capacityColums / 2; i < table->capacityColums; i++) {
            table->data[i] = calloc(table->capacityRows, sizeof(char*));
            if (table->data[i] == NULL) {
                return -1;
            }
        }
    }
    return 0;
}

void deleteTable(Table** table)
{
    if (table == NULL || *table == NULL)
        return;

    Table* t = *table;

    if (t->data != NULL) {
        for (int i = 0; i < t->capacityColums; i++) {
            if (t->data[i] != NULL) {
                for (int j = 0; j < t->capacityRows; j++) {
                    if (t->data[i][j] != NULL) {
                        free(t->data[i][j]);
                    }
                }
                free(t->data[i]);
            }
        }
    }
    free(t->data);
    if (t->maxValCol != NULL) {
        free(t->maxValCol);
    }
    free(t);

    *table = NULL;
}

// Return NULL on memory allocation error
Table* initTable()
{
    Table* table = (Table*)malloc(sizeof(Table));
    if (table == NULL) {
        return NULL;
    }
    table->capacityColums = 3;
    table->capacityRows = 2;
    table->columsLen = 0;
    table->rowsLen = 0;

    table->maxValCol = malloc(table->capacityRows * sizeof(uint16_t));
    if (table->maxValCol == NULL) {
        free(table);
        return NULL;
    }
    for (int i = 0; i < table->capacityRows; i++) {
        table->maxValCol[i] = 0;
    }

    table->data = calloc(table->capacityColums, sizeof(char**));
    if (table->data == NULL) {
        deleteTable(&table);
        return NULL;
    }
    for (int i = 0; i < table->capacityColums; i++) {
        table->data[i] = calloc(table->capacityRows, sizeof(char*));
        if (table->data[i] == NULL) {
            deleteTable(&table);
            return NULL;
        }
    }
    return table;
}

//  Return NULL on memory allocation error
Table* parseCsv(FILE* file)
{
    Table* table = initTable();
    if (table == NULL) {
        return NULL;
    }
    char* line = NULL;
    size_t len = 0;
    int colLen = 0;
    int rowLen = 0;

    while ((getline(&line, &len, file)) != -1) {
        line[strcspn(line, "\r\n")] = 0;
        int err = realloctableColls(table, colLen);
        if (err != 0) {
            free(line);
            deleteTable(&table);
            return NULL;
        }
        char* buffer = malloc(sizeof(char) * strlen(line) + 1);
        if (buffer == NULL) {
            free(line);
            deleteTable(&table);
            return NULL;
        }
        const char* ptr = line;
        int rowIndex = 0;
        while (true) {
            int buffInd = 0;
            bool isQuotes = false;

            if (*ptr == '"') {
                isQuotes = true;
                ptr++;
            }

            while (*ptr != '\0') {
                if (isQuotes) {
                    if (*ptr == '"') {
                        if (*(ptr + 1) == '"') {
                            buffer[buffInd++] = '"';
                            ptr += 2;
                            continue;
                        } else {
                            ptr++;
                            while (*ptr != ',' && *ptr != '\0' && *ptr != '\n' && *ptr != '\r') {
                                ptr++;
                            }
                            break;
                        }
                    }
                } else if (*ptr == ',') {
                    break;
                }
                buffer[buffInd++] = *ptr++;
            }
            buffer[buffInd] = '\0';
            err = reallocTableRows(table, rowIndex);
            if (err != 0) {
                free(buffer);
                free(line);
                deleteTable(&table);
                return NULL;
            }
            if (strlen(buffer) > table->maxValCol[rowIndex]) {
                table->maxValCol[rowIndex] = strlen(buffer);
            }
            table->data[colLen][rowIndex++] = strdup(buffer);
            if (*ptr == ',') {
                ptr++;

                if (*ptr == '\0' || *ptr == '\n' || *ptr == '\r') {
                    err = reallocTableRows(table, rowIndex);
                    if (err != 0) {
                        free(buffer);
                        free(line);
                        deleteTable(&table);
                        return NULL;
                    }
                    table->data[colLen][rowIndex++] = strdup("");
                    if (rowLen < rowIndex) {
                        rowLen = rowIndex;
                    }
                    break;
                }
            } else {
                if (rowLen < rowIndex) {
                    rowLen = rowIndex;
                }
                break;
            }
        }
        free(buffer);
        colLen++;
    }

    table->rowsLen = rowLen;
    table->columsLen = colLen;
    free(line);
    return table;
}

bool isNumber(const char* str)
{
    if (str == NULL || *str == '\0') {
        return false;
    }

    char* endptr;
    strtod(str, &endptr);

    if (endptr == str) {
        return false;
    }
    while (*endptr != '\0') {
        if (!isspace((unsigned char)*endptr)) {
            return false;
        }
        endptr++;
    }
    return true;
}

void printCsv(FILE* file, Table* table)
{
    int charLen = 0;
    for (int i = 0; i < table->rowsLen; i++) {
        charLen += table->maxValCol[i];
    }
    charLen += (table->rowsLen * 2) + (table->rowsLen + 1);

    for (int i = 0; i < table->columsLen; i++) {
        if (i < 1) {
            fprintf(file, "+");
            for (int j = 0; j < charLen - 2; j++) {
                fprintf(file, "=");
            }
            fprintf(file, "+\n");
            for (int j = 0; j < table->rowsLen; j++) {
                fprintf(file, "| ");
                int padding = (int)table->maxValCol[j];
                if (isNumber(table->data[i][j])) {
                    fprintf(file, "%*s ", padding, table->data[i][j]);
                } else {
                    if (table->data[i][j] == NULL) {
                        fprintf(file, "%*s ", padding, "");
                    } else {
                        fprintf(file, "%-*s ", padding, table->data[i][j]);
                    }
                }
            }
            fprintf(file, "|\n");
            fprintf(file, "+");
            for (int j = 0; j < charLen - 2; j++) {
                fprintf(file, "=");
            }
            fprintf(file, "+\n");
        } else {
            for (int j = 0; j < table->rowsLen; j++) {
                fprintf(file, "| ");
                int padding = (int)table->maxValCol[j];
                if (isNumber(table->data[i][j])) {
                    fprintf(file, "%*s ", padding, table->data[i][j]);
                } else {
                    if (table->data[i][j] == NULL) {
                        fprintf(file, "%*s ", padding, "");
                    } else {
                        fprintf(file, "%-*s ", padding, table->data[i][j]);
                    }
                }
            }
            fprintf(file, "|\n");
            fprintf(file, "+");
            for (int j = 0; j < charLen - 2; j++) {
                fprintf(file, "-");
            }
            fprintf(file, "+\n");
        }
    }
}
