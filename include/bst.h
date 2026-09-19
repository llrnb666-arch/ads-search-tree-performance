#ifndef BST_H
#define BST_H

typedef struct BSTNode {
    int val;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

BSTNode *bst_create_node(int val);
BSTNode *bst_insert(BSTNode *root, int val);
BSTNode *bst_search(BSTNode *root, int val);
BSTNode *bst_find_min(BSTNode *root);
BSTNode *bst_delete(BSTNode *root, int val);
void bst_destroy(BSTNode *root);

#endif
