#include "graph.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct BinHeap {
    size_t cap;
    size_t size;
    Road* data;
} BinHeap;

typedef struct Graph {
    size_t size;
    BinHeap* vertices;
} Graph;

typedef struct State {
    size_t* data;
    size_t cap;
    size_t size;
} State;

Graph* initGraph()
{
    return calloc(1, sizeof(Graph));
}

int initCities(Graph* gr, size_t n)
{
    BinHeap* cities = calloc(n, sizeof(BinHeap));
    if (cities == NULL) {
        return -1;
    }

    for (size_t i = 0; i < n; i++) {
        cities[i].cap = 4;
        cities[i].data = calloc(4, sizeof(Road));
        if (cities[i].data == NULL) {
            while (i != 0) {
                free(cities[i].data);
                i--;
            }
            free(cities);
            return -1;
        }
    }

    gr->size = n;
    gr->vertices = cities;

    return 0;
}

static int shiftDown(BinHeap* heap, size_t i)
{
    if (i >= heap->size) {
        return 1;
    }
    while (2 * i + 1 < heap->size) {
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        size_t j = left;
        if (right < heap->size && heap->data[right].len < heap->data[left].len) {
            j = right;
        }
        if (heap->data[i].len <= heap->data[j].len) {
            break;
        }
        Road temp = heap->data[i];
        heap->data[i] = heap->data[j];
        heap->data[j] = temp;
        i = j;
    }
    return 0;
}

static int shiftUp(BinHeap* heap, size_t i)
{
    if (i >= heap->size) {
        return 1;
    }
    while (i > 0 && heap->data[i].len < (heap->data[(i - 1) / 2].len)) {
        Road temp = heap->data[i];
        heap->data[i] = heap->data[(i - 1) / 2];
        heap->data[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }

    return 0;
}

static size_t extractMin(BinHeap* heap, size_t currCity)
{
    if (heap->size == 0) {
        return currCity;
    }
    Road min = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    shiftDown(heap, 0);
    return (min.city1 == currCity) ? min.city2 : min.city1;
}

static int insert(BinHeap* heap, Road* road)
{
    if (heap->cap < heap->size + 1) {
        heap->cap *= 2;
        Road* temp = realloc(heap->data, sizeof(Road) * heap->cap);
        if (temp != NULL) {
            heap->data = temp;
        } else {
            return -1;
        }
    }

    heap->size++;
    heap->data[heap->size - 1] = *road;
    return shiftUp(heap, heap->size - 1);
}

int insertRoad(Graph* gr, Road* road)
{
    road->city1--;
    road->city2--;
    if ((insert(&gr->vertices[road->city1], road) != 0) || (insert(&gr->vertices[road->city2], road) != 0)) {
        return -1;
    }
    return 0;
}

void freeGraph(Graph** graph)
{
    if (graph == NULL || *graph == NULL) {
        return;
    }
    Graph* g = *graph;

    for (size_t i = 0; i < g->size; i++) {
        free(g->vertices[i].data);
    }
    free(g->vertices);
    free(g);
    *graph = NULL;
}

void printGraph(const Graph* gr)
{
    if (gr == NULL) {
        return;
    }

    for (size_t i = 0; i < gr->size; i++) {
        printf("%lu: ", i + 1);
        for (size_t j = 0; j < gr->vertices[i].size; j++) {
            printf("%u ", gr->vertices[i].data[j].len);
        }
        printf("\n");
    }
}

bool isEmpty(Graph* gr)
{
    for (size_t i = 0; i < gr->size; i++) {
        if (gr->vertices[i].size != 0) {
            return false;
        }
    }
    return true;
}

State* getCapitals(FILE* file, size_t capCount)
{

    State* states = calloc(capCount, sizeof(State));
    if (states == NULL) {
        return states;
    }
    for (size_t i = 0; i < capCount; i++) {
        states[i].cap = 4;
        states[i].data = calloc(4, sizeof(size_t));
        if (states[i].data == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(states[j].data);
            }
            free(states);
            return NULL;
        }
    }
    size_t city;

    for (size_t i = 0; i < capCount; i++) {
        fscanf(file, "%lu", &city);

        states[i].data[0] = city - 1;
        states[i].size++;
    }
    return states;
}

int getStates(State* states, Graph* gr, size_t capCount)
{
    int* visited = calloc(gr->size + 1, sizeof(int));
    if (visited == NULL) {
        return -1;
    }

    for (size_t i = 0; i < capCount; i++) {
        visited[states[i].data[0]] = 1;
    }

    size_t totalCaptured = capCount;

    while (totalCaptured < gr->size) {
        bool moved = false;

        for (size_t i = 0; i < capCount; i++) {
            BinHeap* bestHeap = NULL;
            size_t originCity = 0;
            uint32_t minLen = UINT32_MAX;

            for (size_t j = 0; j < states[i].size; j++) {
                size_t c = states[i].data[j];
                BinHeap* h = &gr->vertices[c];

                while (h->size > 0) {
                    Road top = h->data[0];
                    size_t target = (top.city1 == c) ? top.city2 : top.city1;

                    if (visited[target]) {
                        extractMin(h, c);
                    } else {
                        if (top.len < minLen) {
                            minLen = top.len;
                            bestHeap = h;
                            originCity = c;
                        }
                        break;
                    }
                }
            }

            if (bestHeap != NULL) {
                size_t newCity = extractMin(bestHeap, originCity);

                if (!visited[newCity]) {
                    visited[newCity] = 1;
                    totalCaptured++;
                    moved = true;

                    if (states[i].size >= states[i].cap) {
                        states[i].cap *= 2;
                        size_t* temp = realloc(states[i].data, states[i].cap * sizeof(size_t));
                        if (temp == NULL) {
                            free(visited);
                            return -1;
                        }
                        states[i].data = temp;
                    }
                    states[i].data[states[i].size++] = newCity;
                }
            }
        }

        if (!moved)
            break;
    }

    free(visited);
    return 0;
}

void freeStates(State** states, size_t k)
{
    if (states == NULL || *states == NULL) {
        return;
    }

    State* s = *states;
    for (size_t i = 0; i < k; i++) {
        free(s[i].data);
    }
    free(s);
    *states = NULL;
}

void printStates(const State* states, size_t capCount)
{
    for (size_t i = 0; i < capCount; i++) {
        printf("%lu: ", i + 1);
        for (size_t j = 0; j < states[i].size; j++) {
            printf("%lu ", states[i].data[j] + 1);
        }
        printf("\n");
    }
}
