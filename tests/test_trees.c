#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include "avl.h"
#include "bst.h"
#include "splay.h"

static int validate_bst(const BSTNode *root, long long low, long long high) {
    if (root == NULL)
        return 1;

    return root->val > low && root->val < high &&
           validate_bst(root->left, low, root->val) &&
           validate_bst(root->right, root->val, high);
}

static int validate_avl(const AVLNode *root, long long low, long long high,
                        int *height) {
    if (root == NULL) {
        *height = -1;
        return 1;
    }

    int left_height;
    int right_height;

    if (root->val <= low || root->val >= high ||
        !validate_avl(root->left, low, root->val, &left_height) ||
        !validate_avl(root->right, root->val, high, &right_height))
        return 0;

    int expected = (left_height > right_height ? left_height : right_height) + 1;
    int difference = left_height - right_height;
    *height = expected;

    return root->height == expected && difference >= -1 && difference <= 1;
}

static int validate_splay(const SplayNode *root, long long low, long long high) {
    if (root == NULL)
        return 1;

    return root->val > low && root->val < high &&
           validate_splay(root->left, low, root->val) &&
           validate_splay(root->right, root->val, high);
}

int main(void) {
    BSTNode *bst = NULL;
    AVLNode *avl = NULL;
    SplayNode *splay_root = NULL;
    int values[] = {50, 20, 80, 10, 30, 70, 90, 25, 40, 60, 75, 85, 100};
    int count = (int)(sizeof(values) / sizeof(values[0]));

    for (int i = 0; i < count; ++i) {
        bst = bst_insert(bst, values[i]);
        avl = avl_insert(avl, values[i]);
        splay_root = splay_insert(splay_root, values[i]);

        int avl_height;
        assert(validate_bst(bst, (long long)INT_MIN - 1, (long long)INT_MAX + 1));
        assert(validate_avl(avl, (long long)INT_MIN - 1,
                            (long long)INT_MAX + 1, &avl_height));
        assert(validate_splay(splay_root, (long long)INT_MIN - 1,
                              (long long)INT_MAX + 1));
        assert(splay_root->val == values[i]);
    }

    for (int i = 0; i < count; ++i) {
        assert(bst_search(bst, values[i]) != NULL);

        int found = 0;
        splay_root = splay_search(splay_root, values[i], &found);
        assert(found && splay_root->val == values[i]);
        assert(validate_splay(splay_root, (long long)INT_MIN - 1,
                              (long long)INT_MAX + 1));
    }

    int deletion_order[] = {50, 10, 80, 25, 100, 30, 70,
                            20, 40, 60, 75, 85, 90};

    for (int i = 0; i < count; ++i) {
        int val = deletion_order[i];
        bst = bst_delete(bst, val);
        avl = avl_delete(avl, val);
        splay_root = splay_delete(splay_root, val);

        int avl_height;
        assert(validate_bst(bst, (long long)INT_MIN - 1, (long long)INT_MAX + 1));
        assert(validate_avl(avl, (long long)INT_MIN - 1,
                            (long long)INT_MAX + 1, &avl_height));
        assert(validate_splay(splay_root, (long long)INT_MIN - 1,
                              (long long)INT_MAX + 1));
        assert(bst_search(bst, val) == NULL);
    }

    assert(bst == NULL && avl == NULL && splay_root == NULL);

    bst_destroy(bst);
    avl_destroy(avl);
    splay_destroy(splay_root);

    puts("All tree tests passed.");
    return 0;
}
