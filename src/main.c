#include "csvprint.h"
#include "testCsvparse.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    char filepath[200];
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--test") == 0) {
            return runTests();
        }
    }
    snprintf(filepath, sizeof(filepath), "%s/%s", DATA_PATH, "input.csv");
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        printf("File open error\n");
        return 0;
    }
    snprintf(filepath, sizeof(filepath), "%s/%s", DATA_PATH, "output.txt");
    Table* table = parseCsv(file);
    FILE* out = fopen(filepath, "w");
    if (file == NULL) {
        printf("File open error\n");
        return 0;
    }

    printCsv(out, table);

    deleteTable(&table);
    fclose(file);
    fclose(out);
    return 0;
}