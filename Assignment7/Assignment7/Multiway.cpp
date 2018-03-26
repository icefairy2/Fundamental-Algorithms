#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define LENGTH 9

/* Additional memory: each multiway tree node stores the number of its children
 * Multiway tree node: stores the key, number of its children, and a vector of children nodes
 * Binary tree node: stores the key, pointers to child and brother
 */

struct MWTnode {
    int key;
    int nrChildren;
    MWTnode* children[20];
};

struct BTnode {
    int key;
    BTnode* child;
    BTnode* brother;
};

MWTnode* transformT1(int arr[]) {
    MWTnode** nodesArr = (MWTnode**)malloc(LENGTH * sizeof(MWTnode));
    int i;
    for (i = 0; i < LENGTH; i++) {
        MWTnode* node = (MWTnode*)malloc(sizeof(MWTnode));
        node->key = i + 1;
        node->nrChildren = 0;
        nodesArr[i] = node;
    }
    MWTnode* root;
    MWTnode* aux;
    for (i = 0; i < LENGTH; i++) {
        if (arr[i] == -1) {
            root = nodesArr[i];
            continue;
        }
        aux = nodesArr[arr[i] - 1];
        aux->children[aux->nrChildren] = nodesArr[i];
        aux->nrChildren++;
    }
    for (i = 0; i < LENGTH; i++) {
        nodesArr[i]->children[nodesArr[i]->nrChildren] = NULL;
    }
    return root;
}

MWTnode* nextBrother(BTnode* child, MWTnode* parent) {
    if (parent == NULL)
        return NULL;
    if (parent->nrChildren == 0)
        return NULL;
    int k = child->key;
    int i;
    for (i = 0; i < parent->nrChildren; i++) {
        if (parent->children[i]->key == k) {
            break;
        }
    }
    return parent->children[i + 1];
}

BTnode* createChain(MWTnode* parent, int nr) {
    if (parent == NULL || parent->children[nr] == NULL)
        return NULL;
    BTnode* child = (BTnode*)malloc(sizeof(BTnode));
    child->key = parent->children[nr]->key;
    child->brother = createChain(parent, nr + 1);
    return child;
}

void transformT2(BTnode* root, MWTnode* parent, MWTnode* corresp) {
    if (root == NULL)
        return;
    root->child = createChain(corresp, 0);
    transformT2(root->child, corresp, corresp->children[0]);
    MWTnode* brother = nextBrother(root, parent);
    if (root->brother != NULL)
      transformT2(root->brother, parent, brother);
}

void pretty_printMWT(MWTnode* root, int level) {
    if (root != NULL) {       
        for (int i = 0; i < level; i++)
            printf("  ");
        printf("%d\n", root->key);
        int k = 0;
        while (root->children[k] != NULL) {
            pretty_printMWT(root->children[k], level+2);
            k++;
        }
    }
}

void pretty_printBT(BTnode* root, int level) {
    if (root != NULL) {
        for (int i = 0; i < level; i++)
            printf("  ");
        printf("%d\n", root->key);
        int k = 0;
        pretty_printBT(root->child, level + 2);
        pretty_printBT(root->brother, level + 2);
        k++;
    }
}

int main() {
    int R1[] = { 2, 7, 5, 2, 7, 7, -1, 5, 2 };
    int i;
    
    printf("Array:\n");
    for (i = 0; i < LENGTH; i++) {
        printf(" %d ", R1[i]);
    }
    printf("\n");
    for (i = 0; i < LENGTH; i++) {
        printf("(%d)", i);
    }
    printf("\n");
   
    MWTnode* rootMWT = transformT1(R1);

    printf("Multiway tree:\n");
    pretty_printMWT(rootMWT, 0);

    BTnode* rootBT = (BTnode*)malloc(sizeof(BTnode));
    rootBT->key = rootMWT->key;
    rootBT->brother = NULL;
    transformT2(rootBT, NULL, rootMWT);

    printf("Binary tree:\n");
    pretty_printBT(rootBT, 0);

    _getch();
    return 0;
}