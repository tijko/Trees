#ifndef TREE_H
#define TREE_H

#define true (1 == 1)

struct leaf {

    struct leaf *parent;
    struct leaf *left;
    struct leaf *right;

    long value;
};

typedef struct leaf Leaf;

struct tree {
    Leaf *root;
};

typedef struct tree Tree;

Leaf *create_leaf(void);

Leaf *minimum(Leaf *branch);

Leaf *retrieve_branch(Leaf *branch, long value);

void transplant(Tree *tree, Leaf *rm_branch, Leaf *branch);

void remove_leaf(Tree *tree, long value);

void insert(Leaf *tree, long value);

int search(Leaf *tree, long value);

void dump_tree(Leaf *tree);

void free_tree(Leaf *tree);

#endif
