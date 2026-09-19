#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

int avl_get_height(AVLNode *root) {

    if (root == NULL)
        return -1;

    return root->height;
}

static int avl_max(int a, int b) {

    return a > b ? a : b;
}

void avl_update_height(AVLNode *root) {

    if (root == NULL)
        return;
    else
        root->height =
            avl_max(avl_get_height(root->left), avl_get_height(root->right)) + 1;
}

int avl_get_balance_factor(AVLNode *root) {

    if (root == NULL)
        return 0;
    return avl_get_height(root->left) - avl_get_height(root->right);
}

AVLNode *avl_create_node(int val) {

    AVLNode *new_node = malloc(sizeof(*new_node));
    if (new_node == NULL) {
        printf("Memory allocation fails\n");
        return NULL;
    }

    new_node->height = 0;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->val = val;

    return new_node;
}

AVLNode *avl_left_rotation(AVLNode *y) {

    AVLNode *x = y->right;

    y->right = x->left;
    x->left = y;

    avl_update_height(y);
    avl_update_height(x);

    return x;
}

AVLNode *avl_right_rotation(AVLNode *y) {

    AVLNode *x = y->left;

    y->left = x->right;
    x->right = y;

    avl_update_height(y);
    avl_update_height(x);

    return x;
}

AVLNode *avl_insert(AVLNode *root, int val) {

    if (root == NULL)
        return avl_create_node(val);
    if (val < root->val)
        root->left = avl_insert(root->left, val);
    else if (val > root->val)
        root->right = avl_insert(root->right, val);
    else
        return root;

    avl_update_height(root);

    int bf = avl_get_balance_factor(root);

    if (bf > 1 && val < root->left->val)
        return avl_right_rotation(root);

    if (bf < -1 && val > root->right->val)
        return avl_left_rotation(root);

    if (bf > 1 && val > root->left->val) {
        root->left = avl_left_rotation(root->left);
        return avl_right_rotation(root);
    }

    if (bf < -1 && val < root->right->val) {
        root->right = avl_right_rotation(root->right);
        return avl_left_rotation(root);
    }

    return root;
}

AVLNode *avl_find_min(AVLNode *root) {

    if (root == NULL)
        return NULL;

    while (root->left)
        root = root->left;

    return root;
}

AVLNode *avl_delete(AVLNode *root, int val) {

    if (root == NULL)
        return NULL;

    if (val < root->val)
        root->left = avl_delete(root->left, val);
    else if (val > root->val)
        root->right = avl_delete(root->right, val);

    else {
        if (root->left == NULL || root->right == NULL) {

            AVLNode *child;
            if (root->left)
                child = root->left;
            else
                child = root->right;

            free(root);
            return child;
        }

        else {

            AVLNode *successor;
            successor = avl_find_min(root->right);

            root->val = successor->val;
            root->right = avl_delete(root->right, successor->val);
        }
    }

    avl_update_height(root);

    int bf = avl_get_height(root->left) - avl_get_height(root->right);
    if (bf > 1) {
        if (avl_get_balance_factor(root->left) < 0)
            root->left = avl_left_rotation(root->left);
        return avl_right_rotation(root);
    }
    if (bf < -1) {
        if (avl_get_balance_factor(root->right) > 0)
            root->right = avl_right_rotation(root->right);
        return avl_left_rotation(root);
    }

    return root;
}

void avl_destroy(AVLNode *root) {

    if (root == NULL)
        return;

    avl_destroy(root->left);
    avl_destroy(root->right);
    free(root);
}
