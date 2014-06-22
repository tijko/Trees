#include <stdio.h>
#include <stdlib.h>
#include "sng_ptr.h"


Leaf *create_leaf(void) {

    Leaf *fresh_leaf = malloc(sizeof *fresh_leaf);
    fresh_leaf->parent = NULL;
    fresh_leaf->left = NULL;
    fresh_leaf->right = NULL;
    return fresh_leaf;
}

Leaf *min_tree(Leaf *tree) {
    if (!tree->left) {
        return tree;
    }
    return min_tree(tree->left);
}

Leaf *transplant(Leaf *tree, Leaf *rm_branch, Leaf *branch) {
    if (!rm_branch->parent) {
        branch->parent = NULL;
        tree = branch;
        return tree;
    } else if (rm_branch == rm_branch->parent->left) {
        rm_branch->parent->left = branch;
    } else {
        rm_branch->parent->right = branch;
    }
    if (branch)
        branch->parent = rm_branch->parent;
    return tree;
}

Leaf *remove_leaf(Leaf *tree, long value) {
    Leaf *branch = tree;
    while (branch) {
        if (branch->value == value) {
            break;
        } else if (branch->value >= value) {
            branch = branch->left;
        } else {
            branch = branch->right;
        }
    }
    if (!branch) {
        printf("%ld is NOT in tree!\n", value);
        return tree;
    } else {
        if (!branch->left) {
            transplant(tree, branch, branch->right);
        } else if (!branch->right) {
            transplant(tree, branch, branch->left);
        } else {
            Leaf *splice = min_tree(branch->right);
            if (splice->parent != branch) {
                transplant(tree, splice, splice->right);
                splice->right = branch->right;
                splice->right->parent = splice;
            }
            tree = transplant(tree, branch, splice);
            splice->left = branch->left;
            splice->left->parent = splice;
        }
    }
    return tree;
}

Leaf *insert(Leaf *tree, long value) {
    Leaf *fresh_leaf = create_leaf();
    fresh_leaf->value = value;
    Leaf *branch = tree;
    while (branch) {
        if (!branch->left && branch->value >= value) {
            branch->left = fresh_leaf;
            fresh_leaf->parent = branch;
            break;
        } else if (!branch->right && branch->value < value) {
            branch->right = fresh_leaf;
            fresh_leaf->parent = branch;
            break;
        } else if (branch->right && branch->value < value) {
            branch = branch->right;
        } else {
            branch = branch->left;
        }
    }
    return tree;
}

int search(Leaf *tree, long value) {
    Leaf *branch = tree;
    while (branch) {
        if (branch->value == value) {
            return 1;
        } else if (branch->value >= value) {
            branch = branch->left;
        } else {
            branch = branch->right;
        }
    }
    return 0;
}

void dump_tree(Leaf *tree) {
    if (tree) {
        if (tree->left && tree->right) {
            printf("Node: %ld - Left: %ld Right: %ld\n", tree->value, tree->left->value, tree->right->value);
        } else if (tree->left && !tree->right) {
            printf("Node: %ld - Left: %ld Right: <Null>\n", tree->value, tree->left->value);
        } else if (tree->right && !tree->left) {
            printf("Node: %ld - Left: <Null> Right: %ld\n", tree->value, tree->right->value);
        } else if (!tree->right && !tree->left && !tree->parent){
            printf("Node: %ld\n", tree->value);
        }
        dump_tree(tree->left);
        dump_tree(tree->right);
    }
}
