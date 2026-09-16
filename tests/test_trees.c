#include "trees.h"
#include <assert.h>
#include <stdio.h>

int main(void){
    TreeStats s={0};BstNode*b=NULL;AvlNode*a=NULL;SplayNode*p=NULL;
    int keys[]={30,20,10,25,40,50,22,35,45};size_t n=sizeof(keys)/sizeof(keys[0]);
    for(size_t i=0;i<n;i++){b=bst_insert(b,keys[i],&s);a=avl_insert(a,keys[i],&s);p=splay_insert(p,keys[i],&s);assert(bst_validate(b));assert(avl_validate(a));assert(splay_validate(p));}
    assert(bst_size(b)==n&&avl_size(a)==n&&splay_size(p)==n);
    int del[]={10,30,50,999,22};for(size_t i=0;i<sizeof(del)/sizeof(del[0]);i++){b=bst_delete(b,del[i],&s);a=avl_delete(a,del[i],&s);p=splay_delete(p,del[i],&s);assert(bst_validate(b));assert(avl_validate(a));assert(splay_validate(p));}
    for(int k=0;k<60;k++)assert(bst_contains(b,k)==avl_contains(a,k)&&avl_contains(a,k)==splay_contains_plain(p,k));
    bst_destroy(b);avl_destroy(a);splay_destroy(p);

    /* Differential stress test: all implementations must represent the same set. */
    b=NULL;a=NULL;p=NULL;int present[257]={0};unsigned state=1234567u;
    for(int step=0;step<10000;step++){
        state=state*1664525u+1013904223u;int k=(int)(state%257u);
        state=state*1664525u+1013904223u;int insert=(int)(state&1u);
        if(insert){b=bst_insert(b,k,&s);a=avl_insert(a,k,&s);p=splay_insert(p,k,&s);present[k]=1;}
        else{b=bst_delete(b,k,&s);a=avl_delete(a,k,&s);p=splay_delete(p,k,&s);present[k]=0;}
        assert(bst_validate(b)&&avl_validate(a)&&splay_validate(p));
        if(step%97==0)for(int x=0;x<257;x++){
            assert(bst_contains(b,x)==present[x]);
            assert(avl_contains(a,x)==present[x]);
            assert(splay_contains_plain(p,x)==present[x]);
        }
    }
    bst_destroy(b);avl_destroy(a);splay_destroy(p);puts("all tree tests passed");return 0;
}
