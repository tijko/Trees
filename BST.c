#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


struct leaf {

    struct leaf *parent;
    struct leaf *left;
    struct leaf *right;

    int value;
};

struct leaf *create_leaf(void) {

    struct leaf *fresh_leaf = malloc ( sizeof *fresh_leaf );
    fresh_leaf->parent = NULL;
    fresh_leaf->left = NULL;
    fresh_leaf->right = NULL;
    return fresh_leaf;
}

void delete(struct leaf **tree, int value) {
}

void insert(struct leaf **tree, int value) {

    if ((*tree)->value < value) {
        if (!((*tree)->right)) {

            struct leaf *new_leaf = create_leaf();
            new_leaf->parent = *tree;
            new_leaf->value = value;

            (*tree)->right = new_leaf;
        }
        else {
            insert(&(*tree)->right, value);
        }
    }

    else if ((*tree)->value >= value) {
        if (!((*tree)->left)) {

            struct leaf *new_leaf = create_leaf();
            new_leaf->parent = *tree;
            new_leaf->value = value;

            (*tree)->left = new_leaf;
        }

        else {
            insert(&(*tree)->left, value);
        }
    }           
}

void search(struct leaf *tree, int value) {

    if (tree && tree->value != value) {
        if (tree->value < value) {
            search(tree->right, value);
        }
        else {
            search(tree->left, value);
        }
    }
    else if (tree && tree->value == value) {
        printf ("Value %d is in tree!\n", tree->value);
    }
    else {
        printf ("Value %d is not in tree!\n", tree->value);
    }
}

void dump_tree(struct leaf *tree) {

    if (tree) {

        dump_tree(tree->left);
        printf ("Leaf: %d\n", tree->value);
        dump_tree(tree->right);
    }
}

struct leaf **min_tree(struct leaf **tree) {

    if ((*tree)->left) {
        min_tree(&(*tree)->left);
    }
    else {
        return tree;
    }
}

int tree_choice(void) {

    int choice;
    int value;

    struct leaf *tree = NULL;

    while ( true ) {
        printf ("\nPick any of the following choices: [1] insert - [2] delete - [3] search - [4] print tree - [5] exit\n");
        printf ("Enter choice: ");
        scanf ("%d", &choice);
        
        switch (choice) {

            case (1):
                printf ("Enter value: ");
                scanf ("%d", &value);
                if (!(tree)) {
                    tree = create_leaf(); 
                    tree->value = value;
                }
                else {
                    insert(&tree, value);
                }
                break;
            
            case (2):
                printf ("Enter value: ");
                scanf ("%d", &value);
                delete(&tree, value);
                break;

            case (3):
                printf ("Enter value: ");
                scanf ("%d", &value);
                search(tree, value);
                break;

            case (4):
                dump_tree(tree); 
                break;

            case (5):
                return 0;
        } 
    }                 
}

int main(void) {

    printf ("\n -*- Binary Search Tree -*- \n");
    tree_choice();

    return 0;
}
