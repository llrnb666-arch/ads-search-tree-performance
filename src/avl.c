#include <stdio.h>
#include <stdlib.h>
#define MAXN 21

typedef struct node {
    int val;
    int height;
    struct node *left;
    struct node *right;
} node;

int get_height(node *root) {

    if (root == NULL)
        return -1;

    return root->height;
}

int max(int a, int b) {

    return a > b ? a : b;
}

void update_height(node *root) {

    if (root == NULL)
        return;
    else
        root->height = max(get_height(root->left), get_height(root->right)) + 1;
}

int get_balanced_factor(node *root) {

    if (root == NULL)
        return 0;
    return get_height(root->left) - get_height(root->right);
}

node *create_node(int val) {

    node *new_node = malloc(sizeof(*new_node));
    if (new_node == NULL) {
        printf("Memory allocation fails\n");
        return 0;
    }

    new_node->height = 0;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->val = val;

    return new_node;
}

node *left_rotation(node *y) {

    node *x = y->right;

    y->right = x->left;
    x->left = y;

    update_height(y);
    update_height(x);

    return x;
}

node *right_rotation(node *y) {

    node *x = y->left;

    y->left = x->right;
    x->right = y;

    update_height(y);
    update_height(x);

    return x;
}

node *insert_node(node *root, int val) {

    if (root == NULL)
        return create_node(val);
    if (val < root->val)
        root->left = insert_node(root->left, val);
    else if (val > root->val)
        root->right = insert_node(root->right, val);
    else
        return root;

    update_height(root);

    int bf = get_balanced_factor(root);

    if (bf > 1 && val < root->left->val)
        return right_rotation(root);

    if (bf < -1 && val > root->right->val)
        return left_rotation(root);

    if (bf > 1 && val > root->left->val) {
        root->left = left_rotation(root->left);
        return right_rotation(root);
    }

    if (bf < -1 && val < root->right->val) {
        root->right = right_rotation(root->right);
        return left_rotation(root);
    }

    return root;
}

node *find_min(node *root) {

    if (root == NULL)
        return NULL;

    while (root->left)
        root = root->left;

    return root;
}

node *delete(node *root, int val) {

    if (root == NULL)
        return NULL;

    if (val < root->val)
        root->left = delete(root->left, val);
    else if (val > root->val)
        root->right = delete(root->right, val);

    else {
        if (root->left == NULL || root->right == NULL) {

            node *child;
            if (root->left)
                child = root->left;
            else
                child = root->right;

            free(root);
            return child;
        }

        else {

            node *successor;
            successor = find_min(root->right);

            root->val = successor->val;
            root->right = delete(root->right, successor->val);
        }
    }

    update_height(root);

    int bf = get_height(root->left) - get_height(root->right);
    if (bf > 1) {
        if (get_balanced_factor(root->left) < 0)
            root->left = left_rotation(root->left);
        return right_rotation(root);
    }
    if (bf < -1) {
        if (get_balanced_factor(root->right) > 0)
            root->right = right_rotation(root->right);
        return left_rotation(root);
    }

    return root;
}

void cut_down_tree(node *root) {

    if (root == NULL)
        return;

    cut_down_tree(root->left);
    cut_down_tree(root->right);
    free(root);
}

int main(void) {

    int N;
    scanf(" %d", &N);

    node *root = NULL;

    for (int i = 0; i < N; i++) {

        int val;
        scanf(" %d", &val);

        root = insert_node(root, val);
    }

    printf("%d\n", root->val);

    cut_down_tree(root);

    return 0;
}