#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

BSTNode *bst_create_node(int val) {

    BSTNode *new_node = malloc(sizeof(*new_node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    new_node->val = val;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

BSTNode *bst_insert(BSTNode *root, int val) {

    if (root == NULL)
        return bst_create_node(val);

    if (val < root->val)
        root->left = bst_insert(root->left, val);
    else if (val > root->val)
        root->right = bst_insert(root->right, val);

    return root;
}

BSTNode *bst_search(BSTNode *root, int val) {

    if (root == NULL || root->val == val)
        return root;

    if (val < root->val)
        return bst_search(root->left, val);

    return bst_search(root->right, val);
}

BSTNode *bst_find_min(BSTNode *root) {

    if (root == NULL)
        return NULL;

    while (root->left != NULL)
        root = root->left;

    return root;
}

BSTNode *bst_delete(BSTNode *root, int val) {

    if (root == NULL)
        return NULL;

    if (val < root->val) {
        root->left = bst_delete(root->left, val);
    } else if (val > root->val) {
        root->right = bst_delete(root->right, val);
    } else {
        if (root->left == NULL) {
            BSTNode *right_child = root->right;
            free(root);
            return right_child;
        }

        if (root->right == NULL) {
            BSTNode *left_child = root->left;
            free(root);
            return left_child;
        }

        BSTNode *successor = bst_find_min(root->right);
        root->val = successor->val;
        root->right = bst_delete(root->right, successor->val);
    }

    return root;
}

void bst_destroy(BSTNode *root) {

    if (root == NULL)
        return;

    bst_destroy(root->left);
    bst_destroy(root->right);
    free(root);
}
