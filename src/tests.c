#include "graph.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Graph {
    size_t size;
    BinHeap* vertices;
} Graph;

typedef struct State {
    size_t* data;
    size_t cap;
    size_t size;
} State;

typedef struct BinHeap {
    size_t cap;
    size_t size;
    Road* data;
} BinHeap;

void testInit()
{
    Graph* gr = initGraph();
    assert(gr != NULL);
    assert(initCities(gr, 5) == 0);
    assert(gr->size == 5);
    assert(gr->vertices != NULL);
    freeGraph(&gr);
    printf("Test Init: PASSED\n");
}

void testInsertRoad()
{
    Graph* gr = initGraph();
    initCities(gr, 2);

    Road r = { 10, 1, 2 };
    assert(insertRoad(gr, &r) == 0);

    assert(gr->vertices[0].size == 1);
    assert(gr->vertices[1].size == 1);
    assert(gr->vertices[0].data[0].len == 10);

    freeGraph(&gr);
    printf("Test Insert Road: PASSED\n");
}

void testGetStatesLogic()
{
    Graph* gr = initGraph();
    initCities(gr, 3);

    Road r1 = { 5, 1, 2 };
    Road r2 = { 10, 2, 3 };
    insertRoad(gr, &r1);
    insertRoad(gr, &r2);

    State* states = calloc(1, sizeof(State));
    states[0].cap = 4;
    states[0].size = 1;
    states[0].data = calloc(4, sizeof(size_t));
    states[0].data[0] = 0;

    int res = getStates(states, gr, 1);

    assert(res == 0);
    assert(states[0].size == 3);
    assert(states[0].data[1] == 1);
    assert(states[0].data[2] == 2);

    free(states[0].data);
    free(states);
    freeGraph(&gr);
    printf("Test GetStates Logic: PASSED\n");
}

int main()
{
    printf("Running tests...\n");
    testInit();
    testInsertRoad();
    testGetStatesLogic();
    printf("All tests passed successfully!\n");
    return 0;
}