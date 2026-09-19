#include <stdio.h>
#include <stdlib.h>
#include "splay.h"

SplayNode *splay_create_node(int val) {

    SplayNode *new_node = malloc(sizeof(*new_node));
    if (new_node == NULL) {
        printf("Memory allocation fails\n");
        return NULL;
    }

    new_node->val = val;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

SplayNode *splay_left_rotation(SplayNode *y) {

    SplayNode *x = y->right;

    y->right = x->left;
    x->left = y;

    return x;
}

SplayNode *splay_right_rotation(SplayNode *y) {

    SplayNode *x = y->left;

    y->left = x->right;
    x->right = y;

    return x;
}

SplayNode *splay(SplayNode *root, int val) {

    if (root == NULL || val == root->val)
        return root;

    // val is on the left side of the root
    if (val < root->val) {

        // left subtree doesn't exist
        if (root->left == NULL)
            return root;

        // zig-zig
        if (val < root->left->val) {
            root->left->left = splay(root->left->left, val);
            root = splay_right_rotation(root);
        }

        // zig-zag
        else if (val > root->left->val) {
            root->left->right = splay(root->left->right, val);

            // left rotation relies on existed right child
            if (root->left->right)
                root->left = splay_left_rotation(root->left);
        }

        // zig or the second rotation of zig-zig or zig-zag
        if (root->left == NULL)
            return root;

        return splay_right_rotation(root);
    }

    // val is on the right side of the root
    if (val > root->val) {

        // right subtree doesn't exist
        if (root->right == NULL)
            return root;

        // zig-zig
        if (val > root->right->val) {
            root->right->right = splay(root->right->right, val);
            root = splay_left_rotation(root);
        }

        // zig-zag
        else if (val < root->right->val) {
            root->right->left = splay(root->right->left, val);
            if (root->right->left)
                root->right = splay_right_rotation(root->right);
        }

        // zig
        if (root->right == NULL)
            return root;

        return splay_left_rotation(root);
    }

    return root;
}

SplayNode *splay_insert(SplayNode *root, int val) {

    // if the tree is empty, create a new node
    if (root == NULL)
        return splay_create_node(val);

    root = splay(root, val);

    // if the val is already in the tree
    if (root->val == val)
        return root;

    SplayNode *new_node = splay_create_node(val);

    if (val < root->val) {
        new_node->left = root->left;
        new_node->right = root;
        root->left = NULL;
    } else {
        new_node->right = root->right;
        new_node->left = root;
        root->right = NULL;
    }

    return new_node;
}

SplayNode *splay_delete(SplayNode *root, int val) {

    if (root == NULL)
        return root;

    root = splay(root, val);

    // val doesn't exist
    if (root->val != val)
        return root;

    // val exists
    SplayNode *old_node = root;

    if (root->left == NULL) {
        root = root->right;
    } else {
        SplayNode *right_sub = root->right;

        root = splay(root->left, val);
        root->right = right_sub;
    }

    free(old_node);
    return root;
}

SplayNode *splay_search(SplayNode *root, int val, int *found) {

    root = splay(root, val);

    *found = root != NULL && root->val == val;
    return root;
}

void splay_destroy(SplayNode *root) {

    if (root == NULL)
        return;

    splay_destroy(root->left);
    splay_destroy(root->right);
    free(root);
}
