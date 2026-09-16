#include "trees.h"
#include <limits.h>
#include <stdlib.h>

struct SplayNode { int key; struct SplayNode *left,*right; };
static SplayNode*new_node(int k){SplayNode*n=(SplayNode*)malloc(sizeof(*n));if(!n)exit(EXIT_FAILURE);n->key=k;n->left=n->right=NULL;return n;}
static SplayNode*rot_r(SplayNode*y,TreeStats*s){SplayNode*x=y->left;y->left=x->right;x->right=y;if(s)s->rotations++;return x;}
static SplayNode*rot_l(SplayNode*x,TreeStats*s){SplayNode*y=x->right;x->right=y->left;y->left=x;if(s)s->rotations++;return y;}
static SplayNode*splay(SplayNode*r,int k,TreeStats*s){
    if(!r)return NULL;if(s){s->node_visits++;s->comparisons++;}if(k==r->key)return r;
    if(s)s->comparisons++;
    if(k<r->key){if(!r->left)return r;if(s){s->node_visits++;s->comparisons++;}if(k<r->left->key){r->left->left=splay(r->left->left,k,s);r=rot_r(r,s);}else{if(s)s->comparisons++;if(k>r->left->key){r->left->right=splay(r->left->right,k,s);if(r->left->right)r->left=rot_l(r->left,s);}}return r->left?rot_r(r,s):r;}
    if(!r->right)return r;if(s){s->node_visits++;s->comparisons++;}if(k>r->right->key){r->right->right=splay(r->right->right,k,s);r=rot_l(r,s);}else{if(s)s->comparisons++;if(k<r->right->key){r->right->left=splay(r->right->left,k,s);if(r->right->left)r->right=rot_r(r->right,s);}}return r->right?rot_l(r,s):r;
}
SplayNode*splay_access(SplayNode*r,int k,int*found,TreeStats*s){r=splay(r,k,s);if(found)*found=r&&r->key==k;return r;}
SplayNode*splay_insert(SplayNode*r,int k,TreeStats*s){if(!r)return new_node(k);r=splay(r,k,s);if(s)s->comparisons++;if(r->key==k)return r;SplayNode*n=new_node(k);if(k<r->key){n->right=r;n->left=r->left;r->left=NULL;}else{n->left=r;n->right=r->right;r->right=NULL;}return n;}
SplayNode*splay_delete(SplayNode*r,int k,TreeStats*s){if(!r)return NULL;r=splay(r,k,s);if(s)s->comparisons++;if(r->key!=k)return r;SplayNode*old=r;if(!r->left)r=r->right;else{SplayNode*right=r->right;r=splay(r->left,k,s);r->right=right;}free(old);return r;}
int splay_contains_plain(const SplayNode*r,int k){while(r){if(k==r->key)return 1;r=k<r->key?r->left:r->right;}return 0;}
int splay_height(const SplayNode*r){if(!r)return 0;int l=splay_height(r->left),q=splay_height(r->right);return 1+(l>q?l:q);}size_t splay_size(const SplayNode*r){return r?1+splay_size(r->left)+splay_size(r->right):0;}
static int valid(const SplayNode*r,long long lo,long long hi){return!r||((long long)r->key>lo&&(long long)r->key<hi&&valid(r->left,lo,r->key)&&valid(r->right,r->key,hi));}int splay_validate(const SplayNode*r){return valid(r,(long long)INT_MIN-1,(long long)INT_MAX+1);}void splay_destroy(SplayNode*r){if(r){splay_destroy(r->left);splay_destroy(r->right);free(r);}}

