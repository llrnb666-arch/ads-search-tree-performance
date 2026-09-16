#include "trees.h"
#include <limits.h>
#include <stdlib.h>

struct AvlNode { int key, height; struct AvlNode *left, *right; };
static int h(const AvlNode *n) { return n ? n->height : 0; }
static int max2(int a,int b) { return a>b?a:b; }
static void update(AvlNode *n) { n->height=1+max2(h(n->left),h(n->right)); }
static AvlNode *new_node(int k) { AvlNode *n=(AvlNode*)malloc(sizeof(*n)); if(!n) exit(EXIT_FAILURE); n->key=k;n->height=1;n->left=n->right=NULL;return n; }
static AvlNode *right_rotate(AvlNode *y, TreeStats *s) { AvlNode *x=y->left,*t=x->right;x->right=y;y->left=t;update(y);update(x);if(s)s->rotations++;return x; }
static AvlNode *left_rotate(AvlNode *x, TreeStats *s) { AvlNode *y=x->right,*t=y->left;y->left=x;x->right=t;update(x);update(y);if(s)s->rotations++;return y; }
static AvlNode *balance(AvlNode *n, TreeStats *s) {
    if(!n)return NULL; update(n); int b=h(n->left)-h(n->right);
    if(b>1) { if(h(n->left->left)<h(n->left->right)) n->left=left_rotate(n->left,s); return right_rotate(n,s); }
    if(b<-1) { if(h(n->right->right)<h(n->right->left)) n->right=right_rotate(n->right,s); return left_rotate(n,s); }
    return n;
}
AvlNode *avl_insert(AvlNode *r,int k,TreeStats*s){if(!r)return new_node(k);if(s){s->node_visits++;s->comparisons++;}if(k<r->key)r->left=avl_insert(r->left,k,s);else{if(s)s->comparisons++;if(k>r->key)r->right=avl_insert(r->right,k,s);else return r;}return balance(r,s);}
static AvlNode *min_node(AvlNode *r,TreeStats*s){while(r->left){if(s)s->node_visits++;r=r->left;}if(s)s->node_visits++;return r;}
AvlNode *avl_delete(AvlNode*r,int k,TreeStats*s){if(!r)return NULL;if(s){s->node_visits++;s->comparisons++;}if(k<r->key)r->left=avl_delete(r->left,k,s);else{if(s)s->comparisons++;if(k>r->key)r->right=avl_delete(r->right,k,s);else{if(!r->left||!r->right){AvlNode*t=r->left?r->left:r->right;free(r);return t;}AvlNode*t=min_node(r->right,s);r->key=t->key;r->right=avl_delete(r->right,t->key,s);}}return balance(r,s);}
int avl_contains(const AvlNode*r,int k){while(r){if(k==r->key)return 1;r=k<r->key?r->left:r->right;}return 0;}
int avl_height(const AvlNode*r){return h(r);} size_t avl_size(const AvlNode*r){return r?1+avl_size(r->left)+avl_size(r->right):0;}
static int valid(const AvlNode*r,long long lo,long long hi,int*out){if(!r){*out=0;return 1;}int l,q;if((long long)r->key<=lo||(long long)r->key>=hi||!valid(r->left,lo,r->key,&l)||!valid(r->right,r->key,hi,&q))return 0;*out=1+max2(l,q);return r->height==*out&&l-q>=-1&&l-q<=1;}
int avl_validate(const AvlNode*r){int x;return valid(r,(long long)INT_MIN-1,(long long)INT_MAX+1,&x);} void avl_destroy(AvlNode*r){if(r){avl_destroy(r->left);avl_destroy(r->right);free(r);}}

