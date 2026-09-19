#ifndef SPLAY_H
#define SPLAY_H

typedef struct SplayNode {
    int val;
    struct SplayNode *left;
    struct SplayNode *right;
} SplayNode;

SplayNode *splay_create_node(int val);
SplayNode *splay_left_rotation(SplayNode *root);
SplayNode *splay_right_rotation(SplayNode *root);
SplayNode *splay(SplayNode *root, int val);
SplayNode *splay_insert(SplayNode *root, int val);
SplayNode *splay_delete(SplayNode *root, int val);
SplayNode *splay_search(SplayNode *root, int val, int *found);
void splay_destroy(SplayNode *root);

#endif
