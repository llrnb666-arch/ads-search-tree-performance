#include "trees.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum { INSERT, DELETE } OpKind;
typedef struct { OpKind kind; int key; } Operation;
typedef enum { BST, AVL, SPLAY } TreeKind;

static double elapsed(clock_t a,clock_t b){return(double)(b-a)/CLOCKS_PER_SEC*1000.0;}
static void run(TreeKind kind,const char*name,const char*workload,const Operation*ops,size_t count){
    void*root=NULL;TreeStats s={0};clock_t begin=clock();
    for(size_t i=0;i<count;i++){
        if(kind==BST)root=ops[i].kind==INSERT?(void*)bst_insert(root,ops[i].key,&s):(void*)bst_delete(root,ops[i].key,&s);
        else if(kind==AVL)root=ops[i].kind==INSERT?(void*)avl_insert(root,ops[i].key,&s):(void*)avl_delete(root,ops[i].key,&s);
        else root=ops[i].kind==INSERT?(void*)splay_insert(root,ops[i].key,&s):(void*)splay_delete(root,ops[i].key,&s);
    }
    clock_t end=clock();int height=kind==BST?bst_height(root):kind==AVL?avl_height(root):splay_height(root);size_t size=kind==BST?bst_size(root):kind==AVL?avl_size(root):splay_size(root);int ok=kind==BST?bst_validate(root):kind==AVL?avl_validate(root):splay_validate(root);
    printf("%-15s,%-6s,%zu,%zu,%d,%llu,%llu,%llu,%.3f,%s\n",workload,name,count,size,height,s.comparisons,s.node_visits,s.rotations,elapsed(begin,end),ok?"yes":"NO");
    if(kind==BST)bst_destroy(root);else if(kind==AVL)avl_destroy(root);else splay_destroy(root);
}
static void all(const char*w,const Operation*o,size_t n){run(BST,"BST",w,o,n);run(AVL,"AVL",w,o,n);run(SPLAY,"Splay",w,o,n);}
static unsigned rng=20260915u;static unsigned next_rand(void){rng=1664525u*rng+1013904223u;return rng;}
int main(int argc,char**argv){
    int n=argc>1?atoi(argv[1]):2000;if(n<10){fprintf(stderr,"n must be at least 10\n");return 2;}
    Operation*ops=(Operation*)malloc((size_t)n*2*sizeof(*ops));if(!ops)return 1;
    puts("workload,tree,operations,final_size,height,comparisons,node_visits,rotations,time_ms,valid");
    for(int i=0;i<n;i++){ops[i]=(Operation){INSERT,i};ops[n+i]=(Operation){DELETE,i*2};}all("sorted",ops,(size_t)n+n/2);
    for(int i=0;i<n;i++)ops[i]=(Operation){INSERT,i};for(int i=n-1;i>0;i--){int j=(int)(next_rand()%(unsigned)(i+1));Operation t=ops[i];ops[i]=ops[j];ops[j]=t;}for(int i=0;i<n;i++)ops[n+i]=(Operation){DELETE,(int)(next_rand()%(unsigned)n)};all("random",ops,(size_t)n*2);
    for(int i=0;i<n;i++)ops[i]=(Operation){INSERT,i};for(int i=0;i<n;i++){int hot=n/2+(int)(next_rand()%21u)-10;ops[n+i]=(Operation){i%2?INSERT:DELETE,hot};}all("locality",ops,(size_t)n*2);
    free(ops);return 0;
}

