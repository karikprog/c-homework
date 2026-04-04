#pragma once
#include <stdint.h>
#include <stdio.h>

typedef struct Graph Graph;
typedef struct State State;

typedef struct Road {
    uint32_t len;
    size_t city1;
    size_t city2;
} Road;

typedef struct BinHeap BinHeap;

Graph* initGraph();
int initCities(Graph* gr, size_t n);
int insertRoad(Graph* gr, Road* road);
void freeGraph(Graph** graph);
void printGraph(const Graph* gr);
State* getCapitals(FILE* file, size_t capCount);
void freeStates(State** states, size_t k);
void printStates(const State* states, size_t capCount);
int getStates(State* states, Graph* gr, size_t capCount);