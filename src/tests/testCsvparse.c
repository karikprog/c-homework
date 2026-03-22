#include "csvprint.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void createCsvFile(const char* filename, const char* content)
{
    FILE* file = fopen(filename, "w");
    if (file) {
        fputs(content, file);
        fclose(file);
    } else {
        perror("Failed to create test file");
        exit(1);
    }
}

void testBasic()
{
    const char* filename = "test_basic.csv";
    createCsvFile(filename, "ID,Name,Age\n1,Ivan,25\n2,Anna,30\n");

    FILE* file = fopen(filename, "r");
    Table* t = parseCsv(file);

    assert(t != NULL);
    assert(t->columsLen == 3);
    assert(t->rowsLen == 3);

    deleteTable(&t);
    fclose(file);
    printf("[OK] test_basic passed\n");
}

void testEmptyFields()
{
    const char* filename = "test_empty_fields.csv";
    createCsvFile(filename, "Col1,Col2,Col3\n,,val3\nval1,,val3\nval1,val2,\n");

    FILE* file = fopen(filename, "r");
    Table* t = parseCsv(file);

    assert(t != NULL);
    assert(t->columsLen == 4);
    assert(t->rowsLen == 3);

    deleteTable(&t);
    fclose(file);
    printf("[OK] test_empty_fields passed\n");
}

void testQuotesAndCommas()
{
    const char* filename = "test_quotes.csv";

    createCsvFile(filename, "Name,Info\nIvan,\"Moscow, Russia\"\nAnna,\"He said \"\"Hello\"\"\"\n");

    FILE* file = fopen(filename, "r");
    Table* t = parseCsv(file);

    assert(t != NULL);
    assert(t->columsLen == 3);
    assert(t->rowsLen == 2);

    deleteTable(&t);
    fclose(file);
    printf("[OK] test_quotes_and_commas passed\n");
}

void testDifferentRowLengths()
{
    const char* filename = "test_diff_lengths.csv";
    createCsvFile(filename, "A\nB,C\nD,E,F\n");

    FILE* file = fopen(filename, "r");
    Table* t = parseCsv(file);

    assert(t != NULL);
    assert(t->columsLen == 3);
    assert(t->rowsLen == 3);

    deleteTable(&t);
    fclose(file);
    printf("[OK] test_different_row_lengths passed\n");
}

void testEmptyFile()
{
    const char* filename = "test_empty.csv";
    createCsvFile(filename, "");

    FILE* file = fopen(filename, "r");
    Table* t = parseCsv(file);

    assert(t != NULL);
    assert(t->columsLen == 0);
    assert(t->rowsLen == 0);

    deleteTable(&t);
    fclose(file);
    printf("[OK] test_empty_file passed\n");
}

int runTests()
{
    printf("Running CSV Parser tests...\n");

    testBasic();
    testEmptyFields();
    testQuotesAndCommas();
    testDifferentRowLengths();
    testEmptyFile();

    printf("All tests passed successfully!\n");
    return 0;
}