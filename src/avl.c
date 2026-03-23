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

static Node* createNode()
{
    return calloc(1, sizeof(Node));
}

AVL* initTree(void)
{
    return calloc(1, sizeof(AVL));
}

static void cleanTree(Node* head)
{
    if (head == NULL) {
        return;
    }

    cleanTree(head->left);
    cleanTree(head->right);

    free(head->name);
    free(head);
}

void deleteTree(AVL** tree)
{
    AVL* t = *tree;
    if (tree == NULL || t == NULL) {
        return;
    }
    cleanTree(t->root);
    free(t);
    *tree = NULL;
}

size_t size(AVL* tree)
{
    return tree->size;
}

// return true if code1 < code2
static bool isLower(const iataCode code1, const iataCode code2)
{
    return strcmp(code1, code2) < 0;
}

static bool isEqual(const iataCode code1, const iataCode code2)
{
    return strcmp(code1, code2) == 0;
}

static Node* search(Node* node, const iataCode code)
{
    if (node == NULL || isEqual(code, node->code)) {
        return node;
    }

    if (isLower(node->code, code)) {
        return search(node->right, code);
    }

    return search(node->left, code);
}

bool avlContains(const AVL* tree, const iataCode code)
{
    if (tree == NULL) {
        return false;
    }
    if (search(tree->root, code) != NULL) {
        return true;
    }
    return false;
}

static Node* rotateLeft(Node* a, bool* change)
{
    Node* b = a->right;
    Node* c = b->left;
    b->left = a;
    a->right = c;

    if (b->balance == 0) {
        a->balance = 1;
        b->balance = -1;
        *change = false;
    } else {
        a->balance = 0;
        b->balance = 0;
        *change = true;
    }
    return b;
}

static Node* rotateRight(Node* a, bool* change)
{
    Node* b = a->left;
    Node* c = b->right;
    b->right = a;
    a->left = c;

    if (b->balance == 0) {
        a->balance = -1;
        b->balance = 1;
        *change = false;
    } else {
        a->balance = 0;
        b->balance = 0;
        *change = true;
    }
    return b;
}

static Node* bigRotateRight(Node* a)
{
    Node* q = a->left;
    Node* r = q->right;

    q->right = r->left;
    a->left = r->right;
    r->left = q;
    r->right = a;

    if (r->balance == -1) {
        a->balance = 1;
        q->balance = 0;
    } else if (r->balance == 1) {
        a->balance = 0;
        q->balance = -1;
    } else {
        a->balance = 0;
        q->balance = 0;
    }
    r->balance = 0;

    return r;
}

static Node* bigRotateLeft(Node* a)
{
    Node* p = a->right;
    Node* r = p->left;

    p->left = r->right;
    a->right = r->left;
    r->right = p;
    r->left = a;

    if (r->balance == 1) {
        a->balance = -1;
        p->balance = 0;
    } else if (r->balance == -1) {
        a->balance = 0;
        p->balance = 1;
    } else {
        a->balance = 0;
        p->balance = 0;
    }
    r->balance = 0;

    return r;
}

static Node* balance(Node* node, bool* grew)
{
    if (node->balance == 2) {
        *grew = false;
        if (node->right->balance >= 0) {
            return rotateLeft(node, grew);
        }
        return bigRotateLeft(node);
    }

    if (node->balance == -2) {
        *grew = false;
        if (node->left->balance <= 0) {
            return rotateRight(node, grew);
        }
        return bigRotateRight(node);
    }
    return node;
}

static Node* balanceDel(Node* node, bool* lower)
{
    if (node->balance == 2) {
        if (node->right->balance >= 0) {
            return rotateLeft(node, lower);
        } else {
            node = bigRotateLeft(node);
            *lower = true;
            return node;
        }
    }

    if (node->balance == -2) {
        if (node->left->balance <= 0) {
            return rotateRight(node, lower);
        } else {
            node = bigRotateRight(node);
            *lower = true;
            return node;
        }
    }

    if (node->balance == 0) {
        *lower = true;
    } else {
        *lower = false;
    }
    return node;
}

static Node* insert(Node* node, const iataCode code, char* name, bool* grew)
{
    if (node == NULL) {
        Node* newNode = createNode();
        if (newNode == NULL) {
            *grew = false;
            return NULL;
        }
        memcpy(newNode->code, code, 4);
        newNode->name = name;
        *grew = true;
        return newNode;
    }

    if (isLower(node->code, code)) {
        node->right = insert(node->right, code, name, grew);
        if (*grew) {
            node->balance++;
            if (node->balance == 0) {
                *grew = false;
            }
        }
    } else {
        node->left = insert(node->left, code, name, grew);
        if (*grew) {
            node->balance--;
            if (node->balance == 0) {
                *grew = false;
            }
        }
    }

    return balance(node, grew);
}

//-1 if tree is NULL 1 if code is contains
int insertTree(AVL* tree, const iataCode code, char* name)
{
    if (tree == NULL) {
        return -1;
    }
    if (avlContains(tree, code)) {
        return 1;
    }

    bool fl = false;

    Node* r = insert(tree->root, code, name, &fl);
    if (r == NULL) {
        return -1;
    }

    tree->root = r;
    tree->size++;
    return 0;
}

static Node* deleteNode(Node* node, const iataCode code, bool* lower)
{
    if (node == NULL) {
        *lower = false;
        return NULL;
    }
    if (isLower(node->code, code)) {
        node->right = deleteNode(node->right, code, lower);
        if (*lower) {
            node->balance--;
            if (node->balance == -1)
                *lower = false;
            else
                node = balanceDel(node, lower);
        }
    } else if (isLower(code, node->code)) {
        node->left = deleteNode(node->left, code, lower);
        if (*lower) {
            node->balance++;
            if (node->balance == 1)
                *lower = false;
            else
                node = balanceDel(node, lower);
        }
    } else {
        if ((node->right == NULL) && (node->left == NULL)) {
            free(node->name);
            free(node);
            *lower = true;

            return NULL;
        } else if ((node->left == NULL) && (node->right != NULL)) {
            Node* temp = node->right;
            free(node->name);
            free(node);
            *lower = true;

            return temp;
        } else if ((node->left != NULL) && (node->right == NULL)) {
            Node* temp = node->left;
            free(node->name);
            free(node);
            *lower = true;

            return temp;
        } else {

            Node* min = node->right;
            while (min->left) {
                min = min->left;
            }

            memcpy(node->code, min->code, 4);
            char* temp = node->name;
            node->name = min->name;
            min->name = temp;
            node->right = deleteNode(node->right, min->code, lower);

            if (*lower) {
                node->balance--;
                node = balanceDel(node, lower);
            }
        }
    }
    return node;
}

int deleteValue(AVL* tree, const iataCode code)
{
    if (tree == NULL || tree->root == NULL) {
        return -1;
    }

    if (!avlContains(tree, code)) {
        return 0;
    }
    bool fl = false;

    tree->root = deleteNode(tree->root, code, &fl);
    tree->size--;
    return 0;
}

static void printRecAir(const Node* node)
{
    if (node == NULL) {
        return;
    }
    printf("%s:%s\n", node->code, node->name);
    printRecAir(node->right);
    printRecAir(node->left);
}

void printAir(const AVL* tree)
{
    if (tree == NULL || tree->root == NULL) {
        printf("Tree is empty");
        return;
    }
    printRecAir(tree->root);
}

AVL* parseAirports(FILE* file)
{
    AVL* tree = initTree();
    if (tree == NULL) {
        return NULL;
    }

    char* line = NULL;
    size_t len = 0;

    while (getline(&line, &len, file) != -1) {
        line[strcspn(line, "\r\n")] = '\0';
        iataCode code;
        char* name = NULL;
        char* position = strchr(line, ':');
        if (position != NULL) {
            size_t codeLen = position - line;
            if (codeLen != 3) {
                printf("invalid iata code\n");
                continue;
            }
            strncpy(code, line, 3);
            code[3] = '\0';

            name = strdup(position + 1);
        } else {
            printf("invalid line\n");
            continue;
        }
        if (insertTree(tree, code, name) != 0) {
            free(name);
            free(line);
            printf("Insert err");
            deleteTree(&tree);
            return NULL;
        }
    }
    free(line);
    return tree;
}

static void printFile(FILE* file, const Node* node)
{
    if (node == NULL) {
        return;
    }
    fprintf(file, "%s:%s\n", node->code, node->name);
    printFile(file, node->left);
    printFile(file, node->right);
}
int printInFile(FILE* file, const AVL* tree)
{
    if (tree == NULL || tree->root == NULL) {
        return -1;
    }

    printFile(file, tree->root);
    return 0;
}

int getName(const AVL* tree, const iataCode code, char** result)
{
    Node* node = search(tree->root, code);
    if (node == NULL)
        return -1;
    *result = node->name;
    return 0;
}
