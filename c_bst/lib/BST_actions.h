#include <stdio.h>


struct leaf {

    struct leaf *parent;
    struct leaf *left;
    struct leaf *right;

    int value;
};

struct leaf *create_leaf(void);

struct leaf *min_tree(struct leaf **tree);

void transplant(struct leaf **rm_branch, struct leaf **branch);

void delete(struct leaf **tree, int value);

void insert(struct leaf **tree, int value);

void search(struct leaf *tree, int value);

void dump_tree(struct leaf *tree);

