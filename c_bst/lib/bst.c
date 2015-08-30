#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bst.h"


Leaf *create_leaf(void) 
{
    Leaf *fresh_leaf = malloc(sizeof *fresh_leaf);

    assert(fresh_leaf != NULL);

    fresh_leaf->parent = NULL;
    fresh_leaf->left = NULL;
    fresh_leaf->right = NULL;
    return fresh_leaf;
}

Leaf *minimum(Leaf *branch) 
{
    if (!branch->left) 
        return branch;

    return minimum(branch->left);
}

Leaf *retrieve_branch(Leaf *branch, long value)
{
    if (!branch)
        return NULL;
    else if (branch->value == value)
        return branch;
    else if (branch->value > value)
        return retrieve_branch(branch->left, value);
    return retrieve_branch(branch->right, value);
}

void transplant(Tree *tree, Leaf *rm_branch, Leaf *branch) 
{
    if (!rm_branch->parent) { 
        branch->parent = NULL;
        tree->root = branch;
        return;
    }

    if (rm_branch == rm_branch->parent->left)
        rm_branch->parent->left = branch;
    else 
        rm_branch->parent->right = branch;

    if (branch)
        branch->parent = rm_branch->parent;
}

void remove_leaf(Tree *tree, long value) 
{
    Leaf *branch = retrieve_branch(tree->root, value);

    if (!branch) 
        printf("%ld is NOT in tree!\n", value);
    else {
        if (!branch->left) 
            transplant(tree, branch, branch->right);
        else if (!branch->right) 
            transplant(tree, branch, branch->left);
        else {
            Leaf *splice = minimum(branch->right);
            if (splice->parent->value != branch->value) {
                transplant(tree, splice, splice->right);
                splice->right = branch->right;
                splice->right->parent = splice;
            }
            transplant(tree, branch, splice);
            splice->left = branch->left;
            splice->left->parent = splice;
        }
    }
}

void insert(Leaf *tree, long value) 
{
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
}

int search(Leaf *tree, long value) 
{
    if (!(retrieve_branch(tree, value)));
        return 0;
    return 0;
}

void dump_tree(Leaf *tree) 
{
    if (tree) {
        if (tree->left && tree->right) 
            printf("Node: %ld - Left: %ld Right: %ld\n", 
                    tree->value, tree->left->value, tree->right->value);
        else if (tree->left && !tree->right) 
            printf("Node: %ld - Left: %ld Right: <Null>\n", 
                    tree->value, tree->left->value);
        else if (tree->right && !tree->left) 
            printf("Node: %ld - Left: <Null> Right: %ld\n", 
                    tree->value, tree->right->value);
        else if (!tree->right && !tree->left && !tree->parent)
            printf("Node: %ld\n", tree->value);
        dump_tree(tree->left);
        dump_tree(tree->right);
    }
}

void free_tree(Leaf *tree)
{
    if (tree) {
        free_tree(tree->left);
        free_tree(tree->right);
        free(tree);
    }
}
