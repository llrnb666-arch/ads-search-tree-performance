# Search Tree Performance Research Project

This is a starter workspace for comparing unbalanced binary search trees, AVL trees, and splay trees. The student will write the implementation; this initial repository intentionally contains no completed tree code or experimental results.

## Required experiments

For input sizes from 1,000 to 10,000, test distinct integers in three ways:

1. Insert in increasing order and delete in increasing order.
2. Insert in increasing order and delete in reverse order.
3. Insert in random order and delete in random order.

Measure the three structures and plot running time against input size.

## Directory structure

```text
include/       Header files
src/           BST, AVL, splay, and benchmark source files
tests/         Correctness tests
data/inputs/   Reproducible input sequences
data/          Runtime tables and CSV output
plots/         Runtime plots
report/        Final report and documentation
```

## Suggested order

1. Unbalanced BST
2. AVL tree
3. Splay tree
4. Correctness tests
5. Performance program
6. Tables and plots
7. Analysis and final report
