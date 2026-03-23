#include "avl.h"
#include "tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--test") == 0) {
            return runTests();
        }
    }

    FILE* fileR = fopen(argv[1], "r");
    if (fileR == NULL) {
        printf("File open error\n");
        return 0;
    }

    AVL* tree = parseAirports(fileR);
    fclose(fileR);

    char buffer[512];

    while (true) {
        printf("> ");

        if (!fgets(buffer, sizeof(buffer), stdin))
            break;

        buffer[strcspn(buffer, "\n")] = '\0';
        char cmd[20];

        if (sscanf(buffer, "%19s", cmd) != 1)
            continue;

        if (strcmp(cmd, "find") == 0) {
            iataCode code;
            if (sscanf(buffer + 4, "%3s", code)) {
                char* res = NULL;
                if (getName(tree, code, &res) != -1)
                    printf("%s\n", res);
                else
                    printf("Code not found\n");
            } else {
                printf("Usage: find <code>\n");
            }

        } else if (strcmp(cmd, "add") == 0) {
            iataCode code;
            char name[256];
            if (sscanf(buffer + 3, " %3[^:]:%255[^\n]", code, name) == 2) {
                int status = insertTree(tree, code, strdup(name));
                if (status == 0) {
                    printf("Successful\n");
                } else if (status == 1)
                    printf("Airport already exist\n");
                else
                    printf("Tree must be not null\n");
            } else {
                printf("Usage: add <code>:<name>\n");
            }

        } else if (strcmp(cmd, "delete") == 0) {
            iataCode code;
            if (sscanf(buffer + 6, "%3s", code)) {
                if (deleteValue(tree, code) == 0) {
                    printf("Successful\n");
                } else
                    printf("Tree is empty\n");
            } else {
                printf("Usage: find <code>\n");
            }

        } else if (strcmp(cmd, "save") == 0) {
            FILE* file = fopen(argv[1], "w");
            if (file == NULL) {
                printf("File open error\n");
            } else {
                if (printInFile(file, tree) != 0) {
                    printf("Tree is empty\n");
                    continue;
                }
                printf("Succesfull save\n");
                fclose(file);
            }
        } else if (strcmp(cmd, "quit") == 0) {
            break;
        }
    }
    deleteTree(&tree);
    return 0;
}
