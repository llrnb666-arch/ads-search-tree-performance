#ifndef TREES_H
#define TREES_H

#include <stddef.h>

typedef struct {
    unsigned long long comparisons;
    unsigned long long node_visits;
    unsigned long long rotations;
} TreeStats;

typedef struct BstNode BstNode;
typedef struct AvlNode AvlNode;
typedef struct SplayNode SplayNode;

BstNode *bst_insert(BstNode *root, int key, TreeStats *stats);
BstNode *bst_delete(BstNode *root, int key, TreeStats *stats);
int bst_contains(const BstNode *root, int key);
int bst_height(const BstNode *root);
size_t bst_size(const BstNode *root);
int bst_validate(const BstNode *root);
void bst_destroy(BstNode *root);

AvlNode *avl_insert(AvlNode *root, int key, TreeStats *stats);
AvlNode *avl_delete(AvlNode *root, int key, TreeStats *stats);
int avl_contains(const AvlNode *root, int key);
int avl_height(const AvlNode *root);
size_t avl_size(const AvlNode *root);
int avl_validate(const AvlNode *root);
void avl_destroy(AvlNode *root);

SplayNode *splay_insert(SplayNode *root, int key, TreeStats *stats);
SplayNode *splay_delete(SplayNode *root, int key, TreeStats *stats);
SplayNode *splay_access(SplayNode *root, int key, int *found, TreeStats *stats);
int splay_contains_plain(const SplayNode *root, int key);
int splay_height(const SplayNode *root);
size_t splay_size(const SplayNode *root);
int splay_validate(const SplayNode *root);
void splay_destroy(SplayNode *root);

#endif

