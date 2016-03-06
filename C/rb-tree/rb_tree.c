#include <stdio.h>
#include <unistd.h> // sleep
#include <stdlib.h>


enum Color {
    RED,
    BLACK
};

struct Node {
    int value;
    int level;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    enum Color color;
};

struct Tree {
    struct Node *root;
    struct Node *nil;
};

struct Tree *rb_tree_init(void);
struct Node *create_node(int value);
struct Node *create_nil(void);
void insert(struct Tree *tree, struct Node *node, int value);
void delete(struct Tree *tree, struct Node *node, int value);
void transplant(struct Tree *tree, struct Node *node_out, struct Node *node_in);
struct Node *min_tree(struct Tree *tree, struct Node *node);
void inorder(struct Tree *tree, struct Node *node);
void preorder(struct Tree *tree, struct Node *node);
void insert_fixup(struct Tree *tree, struct Node *node);
void delete_fixup(struct Tree *tree, struct Node *node);
void left_rotate(struct Tree *tree, struct Node *node);
void right_rotate(struct Tree *tree, struct Node *node);
void level_fixup(struct Tree *tree, struct Node *node);
void free_tree(struct Tree *tree);
void free_all_nodes(struct Tree *tree, struct Node *node);
void free_node(struct Node *node);


int main(int argc, char *argv[])
{
    struct Tree *tree = rb_tree_init();
    int values[10] = { 3, 9, 0, 1, 2, 5, 7, 6, 8, 4 };

    for (int i=0; i < 10; i++) { 
        insert(tree, tree->root, values[i]);
        preorder(tree, tree->root);
        printf("\n");
    //    sleep(2);
    }

    for (int i=0; i < 10; i++) {
        delete(tree, tree->root, i);
        preorder(tree, tree->root);
        printf("\n");
    //    sleep(2);
    }

    free_tree(tree);
    return 0;
}

struct Tree *rb_tree_init(void)
{
    struct Tree *tree = malloc(sizeof *tree);
    tree->root = NULL;
    tree->nil = malloc(sizeof *tree->nil);
    tree->nil->left = NULL;
    tree->nil->right = NULL;
    tree->nil->parent = NULL;
    tree->nil->color = BLACK;

    return tree;
}

struct Node *create_node(int value)
{
    struct Node *node = malloc(sizeof *node);

    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->value = value;
    node->color = RED;
    node->level = 0;

    return node;
} 

void insert(struct Tree *tree, struct Node *node, int value)
{
    if (!tree->root) {
        tree->root = create_node(value);
        tree->root->parent = tree->nil;
        tree->root->left = tree->nil;
        tree->root->right = tree->nil;
        tree->root->color = BLACK;
        tree->root->level = 1;
        return;
    }

    if (node->value > value) {
        if (node->left == tree->nil) {
            struct Node *new_node = create_node(value);
            new_node->parent = node;
            new_node->left = tree->nil;
            new_node->right = tree->nil;
            node->left = new_node;
            new_node->level = node->level + 1;
            insert_fixup(tree, new_node);            
            level_fixup(tree, tree->root);
        } else 
            insert(tree, node->left, value);
    } else {
        if (node->right == tree->nil) {
            struct Node *new_node = create_node(value);
            new_node->parent = node;
            new_node->left = tree->nil; 
            new_node->right = tree->nil;
            node->right = new_node;
            new_node->level = node->level + 1;
            insert_fixup(tree, new_node);            
            level_fixup(tree, tree->root);
        } else 
            insert(tree, node->right, value);
    }

}

void delete(struct Tree *tree, struct Node *node, int value)
{
    if (node == tree->nil) {
        printf("Value %d not in tree\n", value);
        return;
    }

    if (node->value > value) 
        delete(tree, node->left, value);
    else if (node->value < value) 
        delete(tree, node->right, value);
    else {
        int color = node->color;
        struct Node *replacement;
        if (node->left == tree->nil) {
            replacement = node->right;
            transplant(tree, node, node->right);
            free_node(node);
        } else if (node->right == tree->nil) {
            replacement = node->left;
            transplant(tree, node, node->left);
            free_node(node);
        } else {
            struct Node *successor = min_tree(tree, node->right);
            color = successor->color;
            replacement = successor->right;
            if (successor == node->right)
                replacement->parent = successor;
            else {
                transplant(tree, successor, successor->right);
                successor->right = node->right;
                successor->right->parent = successor;
            }

            transplant(tree, node, successor);
            successor->left = node->left;   
            successor->left->parent = successor;
            successor->color = node->color;
            free_node(node);
        }

        if (color == BLACK)
            delete_fixup(tree, replacement);
    }    

    level_fixup(tree, tree->root);
}

void insert_fixup(struct Tree *tree, struct Node *node)
{
    while (node->parent->color == RED) {
        if (node->parent == node->parent->parent->left) {
            struct Node *uncle = node->parent->parent->right;
            if (uncle->color == RED) {
                uncle->color = BLACK;
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    left_rotate(tree, node);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                right_rotate(tree, node->parent->parent);
            }
        } else { 
            struct Node *uncle = node->parent->parent->left;
            if (uncle->color == RED) {
                uncle->color = BLACK;
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    right_rotate(tree, node);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                left_rotate(tree, node->parent->parent);
            }
        }
    }

    tree->root->color = BLACK;
}

void delete_fixup(struct Tree *tree, struct Node *node)
{
    while (node != tree->root && node->color == BLACK) {
        if (node == node->parent->left) {
            struct Node *sibling = node->parent->right;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                left_rotate(tree, node->parent);
                sibling = node->parent->right;
            }

            if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                sibling->color = RED;
                node = node->parent;
            } else {
                if (sibling->right->color == BLACK) {
                    sibling->color = RED;
                    sibling->left->color = BLACK;
                    right_rotate(tree, sibling);
                    sibling = node->parent->right;
                }

                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->right->color = BLACK;
                left_rotate(tree, node->parent);
                node = tree->root;
            }
        } else {
            struct Node *sibling = node->parent->left;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                right_rotate(tree, node->parent);
                sibling = node->parent->left;
            }

            if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                sibling->color = RED;
                node = node->parent;
            } else {
                if (sibling->left->color == BLACK) {
                    sibling->color = RED;
                    sibling->right->color = BLACK;
                    left_rotate(tree, sibling);
                    sibling = node->parent->left;
                }

                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->left->color = BLACK;
                right_rotate(tree, node->parent);
                node = tree->root;
            }
        }
    }

    node->color = BLACK;
}

struct Node *min_tree(struct Tree *tree, struct Node *node)
{
    while (node->left != tree->nil)
        node = node->left;
    return node;
}

void transplant(struct Tree *tree, struct Node *node_out, struct Node *node_in)
{
    if (node_out == tree->root)
        tree->root = node_in;
    else if (node_out == node_out->parent->left)
        node_out->parent->left = node_in;
    else
        node_out->parent->right = node_in;
    node_in->parent = node_out->parent; 
}

void left_rotate(struct Tree *tree, struct Node *node)
{
    struct Node *node_up = node->right;
    node->right = node_up->left;
    if (node->right != tree->nil)
        node->right->parent = node;

    if (node->parent == tree->nil)
        tree->root = node_up;
    else if (node == node->parent->left)
        node->parent->left = node_up;
    else
        node->parent->right = node_up;

    node_up->parent = node->parent;
    node_up->left = node;
    node->parent = node_up;
}

void right_rotate(struct Tree *tree, struct Node *node)
{
    struct Node *node_up = node->left;
    node->left = node_up->right;
    if (node->left != tree->nil)
        node->left->parent = node;

    if (node->parent == tree->nil)
        tree->root = node_up;
    else if (node == node->parent->left)
        node->parent->left = node_up;
    else
        node->parent->right = node_up;

    node_up->parent = node->parent;
    node_up->right = node;
    node->parent = node_up;
}

void level_fixup(struct Tree *tree, struct Node *node)
{
    if (node == tree->nil)
        return;

    if (node->parent == tree->nil)
        node->level = 1;
    else
        node->level = node->parent->level + 1;

    level_fixup(tree, node->left);
    level_fixup(tree, node->right);
}

void inorder(struct Tree *tree, struct Node *node)
{
    if (node == tree->nil)
        return;
    inorder(tree, node->left);
    printf("%d ", node->value);
    inorder(tree, node->right);
}

void preorder(struct Tree *tree, struct Node *node)
{
    if (node == tree->nil)
        return;
    printf("%d level %d color %s\n", node->value, node->level,
                                   node->color == RED ? "red" : "black");
    preorder(tree, node->left);
    preorder(tree, node->right);
}

void free_tree(struct Tree *tree)
{
    if (tree->root && tree->root != tree->nil)
        free_all_nodes(tree, tree->root);
    free(tree->nil);
    free(tree);
}

void free_all_nodes(struct Tree *tree, struct Node *node)
{
    if (node->left != tree->nil)
        free_node(node->left);
    if (node->right != tree->nil)
        free_node(node->right);
    free(node);
}

void free_node(struct Node *node)
{
    free(node);
}
