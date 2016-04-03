#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "kdtree.h"


struct Tree *init_tree(void)
{
    struct Tree *new_tree = malloc(sizeof *new_tree);

    if (new_tree == NULL) {
        perror("malloc");
        return NULL;
    }

    new_tree->root = NULL;

    return new_tree;
}

struct Node *init_node(int *point)
{
    struct Node *new_node = malloc(sizeof *new_node);
    new_node->point = point;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    new_node->dimension = 0;
    new_node->max_x = 0, new_node->min_x = 0;
    new_node->max_y = 0, new_node->min_y = 0;

    return new_node;
}

void init_root(struct Tree *tree, int *point)
{
    tree->root = init_node(point);
    tree->root->max_x = MAX;
    tree->root->max_y = MAX;
}

void free_tree(struct Tree *tree)
{
    free_nodes(tree->root);
    free(tree);
}

void free_nodes(struct Node *node)
{
    if (node == NULL)
        return;

    free_nodes(node->left);
    free_nodes(node->right);
    free_node(node);
}

void free_node(struct Node *node)
{
    free(node->point);
    free(node);
}

int **create_random_points(int number_of_points)
{
    int **points = malloc(sizeof *points * number_of_points);

    if (points == NULL) {
        perror("malloc");
        return NULL;
    }

    for (int i=0; i < number_of_points; i++) {
        points[i] = malloc(sizeof(int) * 2);
        points[i][0] = rand() % MAX;
        points[i][1] = rand() % MAX;
    }

    return points;
}

void insert_node(struct Node *node, struct Node *new_node)
{
    int dimension = node->dimension;

    if (node->point[dimension] > new_node->point[dimension]) {
        if (node->left == NULL) {
            node->left = new_node;
            new_node->parent = node;
            new_node->dimension ^= dimension;
            set_dimension(node, new_node);
        } else 
            insert_node(node->left, new_node);
    } else {
        if (node->right == NULL) {
            node->right = new_node;
            new_node->parent = node;
            new_node->dimension ^= dimension;
            set_dimension(node, new_node);
        } else 
            insert_node(node->right, new_node);
    }
}

void set_dimension(struct Node *node, struct Node *new_node)
{
    int dimension = node->dimension;

    if (!dimension) {
        if (new_node == node->left) {
            new_node->max_x = node->point[0];
            new_node->max_y = node->max_y;
            new_node->min_x = node->min_x;
            new_node->min_y = node->min_y;    
        } else {
            new_node->max_x = node->max_x;
            new_node->max_y = node->max_y;
            new_node->min_x = node->point[0];
            new_node->min_y = node->min_y;
        }
    } else {
        if (new_node == node->left) {
            new_node->max_x = node->max_x;
            new_node->max_y = node->point[1];
            new_node->min_x = node->min_x;
            new_node->min_y = node->min_y;
        } else {
            new_node->max_x = node->max_x;
            new_node->max_y = node->max_y;
            new_node->min_x = node->min_x;
            new_node->min_y = node->point[1];
        }
    }
}

void insert_point_list(struct Tree *tree, int **points, int number_of_points)
{
    int i = 0;
    if (tree->root == NULL) {
        init_root(tree, points[i++]);
    }

    for (; i < number_of_points; i++) {
        struct Node *new_node = init_node(points[i]);
        insert_node(tree->root, new_node);
    }
}

int main(int argc, char *argv[])
{
    struct Tree *tree = init_tree();

    if (tree == NULL)
        return 0;
    
    int number_of_points = 100;

    int **points = create_random_points(number_of_points);

#if 0    

    /* Debug points */

    for (int i=0; i < number_of_points; i++)
        printf("Point %d: (%d, %d)\n", i, points[i][0], points[i][1]);

#endif

    if (points == NULL)
        free(tree);

    insert_point_list(tree, points, number_of_points);


    return 0;
}
