#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctime>
#include "Profiler.h"

#define MAX_SIZE 11

Profiler profiler("Dynamic Order Statistics");

int effSel = 0, effDel = 0;

struct Node {
    int key;
    int size;
    Node* left;
    Node* right;
};

Node* find_min(Node* root) {
    effDel++;
    while (root->left != NULL) {
        root = root->left;
        effDel++;
    }      
    return root;
}

Node* build_tree(int arr[], int l, int r) {
    if (l <= r) {
        Node* root = (Node*)malloc(sizeof(Node));
        root->key = arr[(l + r) / 2];
        root->size = 1;
        root->left = root->right = NULL;
        root->left = build_tree(arr, l, (l + r) / 2 - 1);
        root->right = build_tree(arr, (l + r) / 2 + 1, r);
        if (root->left != NULL)
            root->size += root->left->size;
        if (root->right != NULL)
            root->size += root->right->size;
        return root;
    }
    else
        return NULL;
}

Node* os_select(Node* n, int i) {
    effSel++;
    if (n != NULL) {
        int r;
        effSel++;
        if (n->left != NULL)
            r = n->left->size + 1;
        else
            r = 1;
        if (i == r)
            return n;
        else if (i < r) {
            return os_select(n->left, i);
        }
        else {
            return os_select(n->right, i - r);
        }
    }
    else
        return NULL;       
}

void os_delete(Node* root, Node* n) {
    effDel++;
    if (n == root) {
        effDel += 2;
        if (n->left == NULL && n->right != NULL) {
            n = n->right;
            return;
        }
        effDel += 2;
        if (n->right == NULL && n->left != NULL) {
            n = n->left;
            return;
        }
        effDel += 2;
        if (n->left == NULL && n->right == NULL) {
            free(n);
            n = NULL;
            return;
        }
        Node* rep = find_min(n->right);
        effDel++;
        int rkey = rep->key;
        os_delete(root, rep);
        n->key = rkey;
        return;
    }
    Node* parent = root;
    effDel += 2;
    while (parent->left != n && parent->right != n) {
        if (n->key < parent->key)
            parent = parent->left;
        else
            parent = parent->right;
        effDel += 2;
    }
    effDel++;
    if (n->left == NULL) {
        effDel++;
        if (n->right == NULL) {
            //no children
            effDel += 2;
            if (n == parent->left)
                parent->left = NULL;
            else
                parent->right = NULL;
            free(n);
            n = NULL;
        }
        else {
            //one child on the right
            effDel += 2;
            if (n == parent->left)
                parent->left = n->right;
            else
                parent->right = n->right;
            free(n);
            n = NULL;
        }
    }
    else {
        effDel++;
        if (n->right == NULL) {
            //one child on the left
            effDel += 2;
            if (n == parent->left)
                parent->left = n->left;
            else
                parent->right = n->left;
            free(n);
            n = NULL;
        }
        else {
            //two children, find replacement
            Node* rep = find_min(n->right);
            effDel++;
            int rkey = rep->key;
            os_delete(root, rep);
            n->key = rkey;
            return;
        }
    }
    //update size field
    Node* aux = root;
    while (aux != parent) {
        aux->size--;
        if (parent->key < aux->key)
            aux = aux->left;
        else
            aux = aux->right;
    }
    parent->size--;
}

void pretty_print(Node* root) {
    if (root != NULL) {
       /* printf("node:%d size:%d left:%d right:%d\n", root->key, root->size, 
            (root->left == NULL)? 0: root->left->key, 
            (root->right == NULL) ? 0 : root->right->key);*/
        pretty_print(root->left);
        for (int i = 0; i < root->size / 2 + 1; i++)
            printf("  ");
        printf("%d(%d)\n", root->key, root->size);
        pretty_print(root->right);
    }
}

void testCorrect(int arr[]) {
    //Build a tree of size MAX_SIZE, which should be a decently small number
    Node* pbt = (Node*)malloc(sizeof(Node));
    pbt = build_tree(arr, 0, MAX_SIZE-1);
    pretty_print(pbt);

    int sel = 0;
    Node* n = (Node*)malloc(sizeof(Node));
    for (int i = 0; i < 3; i++) {
        sel = 1 + rand() % (MAX_SIZE - i);
        n = os_select(pbt, sel);
        if (n != NULL) {
            printf("%d. smallest element: %d(%d)\n", sel, n->key, n->size);
            os_delete(pbt, n);
            printf("Node succesfully deleted.\n");
        }         
        else
            printf("Tree size=%d smaller than requested i=%d\n", pbt->size, sel);      
    } 
    pretty_print(pbt);
}

int main() {
    srand(time(NULL));
    int arr[MAX_SIZE];
    int i = 0;
    for (i = 0; i < MAX_SIZE; i++) {
        arr[i] = i + 1;
    }
    
    //to call this function, modify MAX_SIZE to a decently small number(11)
    testCorrect(arr);

   /* Node* pbt = (Node*)malloc(sizeof(Node));
    Node* node = (Node*)malloc(sizeof(Node));

    int n = 100;
    int size = 0, r = 0;
    for (n = 100; n < MAX_SIZE + 1; n += 100) {
        for (i = 0; i < 5; i++) {
            printf("n=%d i=%d\n", n, i);
            pbt = build_tree(arr, 0, n - 1);
            effSel = 0;
            effDel = 0;
            size = n;
            while (size > 0) {
                r = 1 + rand() % size;
                node = os_select(pbt, r);
                os_delete(pbt, node);
                size--;
            }
        }
        profiler.countOperation("Select", n, effSel / 5);
        profiler.countOperation("Delete", n, effDel / 5);
    }
    profiler.createGroup("Total operations", "Select", "Delete");
    profiler.showReport();*/
    return 0;
}