#include "trees.h"
#include <limits.h>
#include <stdlib.h>

struct BstNode { int key; struct BstNode *left, *right; };

static BstNode *new_node(int key) {
    BstNode *n = (BstNode *)malloc(sizeof(*n));
    if (!n) exit(EXIT_FAILURE);
    n->key = key; n->left = n->right = NULL;
    return n;
}

BstNode *bst_insert(BstNode *root, int key, TreeStats *s) {
    if (!root) return new_node(key);
    if (s) { s->node_visits++; s->comparisons++; }
    if (key < root->key) root->left = bst_insert(root->left, key, s);
    else {
        if (s) s->comparisons++;
        if (key > root->key) root->right = bst_insert(root->right, key, s);
    }
    return root;
}

static BstNode *detach_min(BstNode *root, BstNode **minimum, TreeStats *s) {
    if (s) s->node_visits++;
    if (!root->left) { *minimum = root; return root->right; }
    root->left = detach_min(root->left, minimum, s);
    return root;
}

BstNode *bst_delete(BstNode *root, int key, TreeStats *s) {
    if (!root) return NULL;
    if (s) { s->node_visits++; s->comparisons++; }
    if (key < root->key) root->left = bst_delete(root->left, key, s);
    else {
        if (s) s->comparisons++;
        if (key > root->key) root->right = bst_delete(root->right, key, s);
        else {
            if (!root->left) { BstNode *r = root->right; free(root); return r; }
            if (!root->right) { BstNode *l = root->left; free(root); return l; }
            BstNode *m = NULL;
            root->right = detach_min(root->right, &m, s);
            root->key = m->key;
            free(m);
        }
    }
    return root;
}

int bst_contains(const BstNode *r, int k) { while (r) { if (k == r->key) return 1; r = k < r->key ? r->left : r->right; } return 0; }
int bst_height(const BstNode *r) { if (!r) return 0; int l=bst_height(r->left), q=bst_height(r->right); return 1+(l>q?l:q); }
size_t bst_size(const BstNode *r) { return r ? 1+bst_size(r->left)+bst_size(r->right) : 0; }
static int valid(const BstNode *r, long long lo, long long hi) { return !r || ((long long)r->key>lo && (long long)r->key<hi && valid(r->left,lo,r->key) && valid(r->right,r->key,hi)); }
int bst_validate(const BstNode *r) { return valid(r, (long long)INT_MIN-1, (long long)INT_MAX+1); }
void bst_destroy(BstNode *r) { if (r) { bst_destroy(r->left); bst_destroy(r->right); free(r); } }

