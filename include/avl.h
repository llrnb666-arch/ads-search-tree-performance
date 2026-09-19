#ifndef AVL_H
#define AVL_H

typedef struct AVLNode {
    int val;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

int avl_get_height(AVLNode *root);
void avl_update_height(AVLNode *root);
int avl_get_balance_factor(AVLNode *root);
AVLNode *avl_create_node(int val);
AVLNode *avl_left_rotation(AVLNode *root);
AVLNode *avl_right_rotation(AVLNode *root);
AVLNode *avl_insert(AVLNode *root, int val);
AVLNode *avl_find_min(AVLNode *root);
AVLNode *avl_delete(AVLNode *root, int val);
void avl_destroy(AVLNode *root);

#endif
