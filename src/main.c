#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

void run(FILE* file)
{
    Graph* graph = initGraph();
    if (graph == NULL) {
        printf("Memory alloc err\n");
        return;
    }

    size_t n, m;
    fscanf(file, "%lu", &n);
    fscanf(file, "%lu", &m);

    if (initCities(graph, n) != 0) {
        printf("Memory alloc err\n");
        freeGraph(&graph);
        return;
    }

    Road road;
    for (size_t i = 0; i < m; i++) {
        fscanf(file, "%lu", &road.city1);
        fscanf(file, "%lu", &road.city2);
        fscanf(file, "%u", &road.len);
        int status = insertRoad(graph, &road);
        if (status == -1) {
            printf("Memory alloc err\n");
            freeGraph(&graph);
            return;
        }
    }

    size_t k;
    fscanf(file, "%lu", &k);

    State* states = getCapitals(file, k);
    if (states == NULL) {
        printf("Memory alloc error\n");
        freeGraph(&graph);
        freeStates(&states, k);
        return;
    }

    if (getStates(states, graph, k) != 0) {
        printf("Memory alloc error\n");
        freeGraph(&graph);
        freeStates(&states, k);
        return;
    }
    printStates(states, k);

    freeStates(&states, k);

    freeGraph(&graph);
}

int main(void)
{

    FILE* file = fopen("city.txt", "r");
    if (file == NULL) {
        printf("file not found\n");
        return -1;
    }

    run(file);

    fclose(file);
    return 0;
}
