#pragma once
#include <stdbool.h>
#include <stdio.h>

typedef struct AVL AVL;

typedef struct Node Node;

typedef char iataCode[4];

AVL* initTree(void);
void deleteTree(AVL** tree);
bool avlContains(const AVL* tree, const iataCode code);
int insertTree(AVL* tree, const iataCode code, char* name);
int deleteValue(AVL* tree, const iataCode code);
AVL* parseAirports(FILE* file);
void printAir(const AVL* tree);
int printInFile(FILE* file, const AVL* tree);
size_t size(AVL* tree);
int getName(const AVL* tree, const iataCode code, char** result);
