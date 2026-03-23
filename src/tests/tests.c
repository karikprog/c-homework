#include "avl.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AVL {
    size_t size;
    Node* root;
} AVL;

typedef struct Node {
    iataCode code;
    char* name;
    Node* right;
    Node* left;
    int balance;
} Node;

int testsPassed = 0;
int testsFailed = 0;

void checkInt(const char* testName, int expected, int actual)
{
    if (expected != actual) {
        fprintf(stderr, "FAILED: %s | expected=%d actual=%d\n", testName, expected, actual);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void checkSize(const char* testName, size_t expected, size_t actual)
{
    if (expected != actual) {
        fprintf(stderr, "FAILED: %s | expected=%zu actual=%zu\n", testName, expected, actual);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void checkBool(const char* testName, bool expected, bool actual)
{
    if (expected != actual) {
        fprintf(stderr, "FAILED: %s | expected=%d actual=%d\n", testName, expected, actual);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void checkPtrNotNull(const char* testName, void* ptr)
{
    if (ptr == NULL) {
        fprintf(stderr, "FAILED: %s | pointer is NULL\n", testName);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void checkPtrNull(const char* testName, void* ptr)
{
    if (ptr != NULL) {
        fprintf(stderr, "FAILED: %s | pointer is NOT NULL\n", testName);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void checkString(const char* testName, const char* expected, const char* actual)
{
    if (expected == NULL || actual == NULL) {
        if (expected != actual) {
            fprintf(stderr, "FAILED: %s | expected=%s actual=%s\n",
                testName, expected ? expected : "NULL", actual ? actual : "NULL");
            testsFailed++;
        } else {
            testsPassed++;
        }
        return;
    }

    if (strcmp(expected, actual) != 0) {
        fprintf(stderr, "FAILED: %s | expected='%s' actual='%s'\n", testName, expected, actual);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void testInitTree()
{
    AVL* tree = initTree();
    checkPtrNotNull("initTree returns non-NULL", tree);
    if (tree != NULL) {
        checkSize("initTree initial size is 0", 0, size(tree));
        checkPtrNull("initTree initial root is NULL", tree->root);
    }
    deleteTree(&tree);
}

void testInsertTree()
{
    AVL* tree = initTree();

    int res = insertTree(tree, "SVO", strdup("Sheremetyevo"));
    checkInt("insertTree base insert returns 0", 0, res);
    checkSize("insertTree size increments", 1, size(tree));

    res = insertTree(tree, "SVO", strdup("Duplicate"));
    checkInt("insertTree duplicate returns 1", 1, res);
    checkSize("insertTree size remains same on duplicate", 1, size(tree));

    insertTree(tree, "AAA", strdup("A"));
    insertTree(tree, "BBB", strdup("B"));
    insertTree(tree, "CCC", strdup("C"));

    checkString("insertTree RR balance correct root", "BBB", tree->root->code);

    deleteTree(&tree);
}

void testAvlContains()
{
    AVL* tree = initTree();

    checkBool("avlContains empty tree returns false", false, avlContains(tree, "SVO"));

    insertTree(tree, "SVO", strdup("Sheremetyevo"));
    checkBool("avlContains existing code returns true", true, avlContains(tree, "SVO"));
    checkBool("avlContains missing code returns false", false, avlContains(tree, "DME"));

    deleteTree(&tree);
}

void testGetName()
{
    AVL* tree = initTree();
    insertTree(tree, "LED", strdup("Pulkovo"));

    char* name = NULL;
    int res = getName(tree, "LED", &name);

    checkInt("getName existing returns 0", 0, res);
    checkString("getName returns correct string", "Pulkovo", name);

    name = NULL;
    res = getName(tree, "ZZZ", &name);
    checkInt("getName missing returns -1", -1, res);
    checkPtrNull("getName missing sets pointer to NULL", name);

    deleteTree(&tree);
}

void testDeleteValue()
{
    AVL* tree = initTree();
    insertTree(tree, "AAA", strdup("A"));
    insertTree(tree, "BBB", strdup("B"));
    insertTree(tree, "CCC", strdup("C"));

    checkSize("deleteValue size before", 3, size(tree));

    int res = deleteValue(tree, "BBB");
    checkInt("deleteValue existing returns 0", 0, res);
    checkSize("deleteValue size decrements", 2, size(tree));
    checkBool("deleteValue item removed", false, avlContains(tree, "BBB"));

    res = deleteValue(tree, "ZZZ");
    checkInt("deleteValue missing returns 0 (per logic)", 0, res);
    checkSize("deleteValue size remains same", 2, size(tree));

    deleteTree(&tree);
}

void testDeleteTree()
{
    AVL* tree = initTree();
    insertTree(tree, "SVO", strdup("Sheremetyevo"));

    deleteTree(&tree);
    checkPtrNull("deleteTree sets pointer to NULL", tree);
}

void testSize()
{
    AVL* tree = initTree();
    checkSize("size empty tree is 0", 0, size(tree));

    insertTree(tree, "AAA", strdup("A"));
    insertTree(tree, "BBB", strdup("B"));
    checkSize("size after 2 inserts is 2", 2, size(tree));

    deleteValue(tree, "AAA");
    checkSize("size after deletion is 1", 1, size(tree));

    deleteTree(&tree);
}

void testStructureInsertionRotations()
{
    AVL* treeLL = initTree();
    insertTree(treeLL, "CCC", strdup("C"));
    insertTree(treeLL, "BBB", strdup("B"));
    insertTree(treeLL, "AAA", strdup("A"));

    checkString("LL: Root should be BBB", "BBB", treeLL->root->code);
    checkInt("LL: Root balance should be 0", 0, treeLL->root->balance);
    checkString("LL: Left is AAA", "AAA", treeLL->root->left->code);
    checkString("LL: Right is CCC", "CCC", treeLL->root->right->code);
    deleteTree(&treeLL);

    AVL* treeRR = initTree();
    insertTree(treeRR, "AAA", strdup("A"));
    insertTree(treeRR, "BBB", strdup("B"));
    insertTree(treeRR, "CCC", strdup("C"));

    checkString("RR: Root should be BBB", "BBB", treeRR->root->code);
    checkString("RR: Left is AAA", "AAA", treeRR->root->left->code);
    checkString("RR: Right is CCC", "CCC", treeRR->root->right->code);
    deleteTree(&treeRR);

    AVL* treeLR = initTree();
    insertTree(treeLR, "CCC", strdup("C"));
    insertTree(treeLR, "AAA", strdup("A"));
    insertTree(treeLR, "BBB", strdup("B"));

    checkString("LR: Root should be BBB", "BBB", treeLR->root->code);
    checkInt("LR: Left balance is 0", 0, treeLR->root->left->balance);
    checkInt("LR: Right balance is 0", 0, treeLR->root->right->balance);
    deleteTree(&treeLR);

    AVL* treeRL = initTree();
    insertTree(treeRL, "AAA", strdup("A"));
    insertTree(treeRL, "CCC", strdup("C"));
    insertTree(treeRL, "BBB", strdup("B"));

    checkString("RL: Root should be BBB", "BBB", treeRL->root->code);
    checkString("RL: Left is AAA", "AAA", treeRL->root->left->code);
    checkString("RL: Right is CCC", "CCC", treeRL->root->right->code);
    deleteTree(&treeRL);
}

void testStructureDeletion()
{
    AVL* treeLeaf = initTree();
    insertTree(treeLeaf, "BBB", strdup("B"));
    insertTree(treeLeaf, "AAA", strdup("A"));
    deleteValue(treeLeaf, "AAA");

    checkSize("Delete leaf: size after", 1, size(treeLeaf));
    checkPtrNull("Delete leaf: left is NULL", treeLeaf->root->left);
    checkInt("Delete leaf: root balance reset", 0, treeLeaf->root->balance);
    deleteTree(&treeLeaf);

    AVL* treeTwoKids = initTree();
    insertTree(treeTwoKids, "BBB", strdup("B"));
    insertTree(treeTwoKids, "AAA", strdup("A"));
    insertTree(treeTwoKids, "DDD", strdup("D"));
    insertTree(treeTwoKids, "CCC", strdup("C"));

    deleteValue(treeTwoKids, "BBB");

    checkString("Delete 2 kids: CCC took place of BBB", "CCC", treeTwoKids->root->code);
    checkPtrNotNull("Delete 2 kids: AAA still exists", treeTwoKids->root->left);
    checkPtrNotNull("Delete 2 kids: DDD still exists", treeTwoKids->root->right);
    deleteTree(&treeTwoKids);

    AVL* treeRot = initTree();
    insertTree(treeRot, "BBB", strdup("B"));
    insertTree(treeRot, "AAA", strdup("A"));
    insertTree(treeRot, "DDD", strdup("D"));
    insertTree(treeRot, "EEE", strdup("E"));

    deleteValue(treeRot, "AAA");
    checkString("Delete rotation: DDD became root", "DDD", treeRot->root->code);
    checkString("Delete rotation: BBB is now left", "BBB", treeRot->root->left->code);
    checkInt("Delete rotation: balance is 0", 0, treeRot->root->balance);
    deleteTree(&treeRot);

    AVL* treeLast = initTree();
    insertTree(treeLast, "SVO", strdup("S"));
    deleteValue(treeLast, "SVO");

    checkSize("Delete last: size is 0", 0, size(treeLast));
    checkPtrNull("Delete last: root is NULL", treeLast->root);
    deleteTree(&treeLast);
}

int runTests()
{
    fprintf(stderr, "=== Starting AVL Tree Tests ===\n");

    testInitTree();
    testInsertTree();
    testAvlContains();
    testGetName();
    testDeleteValue();
    testDeleteTree();
    testSize();
    testStructureDeletion();
    testStructureInsertionRotations();

    fprintf(stderr, "=== Test Summary ===\n");
    fprintf(stderr, "Passed: %d\n", testsPassed);
    fprintf(stderr, "Failed: %d\n", testsFailed);

    return (testsFailed > 0) ? 1 : 0;
}