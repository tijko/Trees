#include <math.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "kdtree.h"


// separate initializations for different types
struct Tree *init_tree(void)
{
    struct Tree *new_tree = malloc(sizeof *new_tree);

    if (new_tree == NULL) {
        perror("malloc");
        return NULL;
    }

    new_tree->root = NULL;
    new_tree->close_dist = LONG_MAX;
    /*
    new_tree->max_x = LONG_MAX;
    new_tree->max_y = LONG_MAX;
    */
    new_tree->max_x = 1.0;
    new_tree->max_y = 1.0;
    new_tree->close_point = NULL;

    return new_tree;
}

struct Node *init_node(void *point)
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

void init_root(struct Tree *tree, void *point)
{
    tree->root = init_node(point);
    tree->root->max_x = tree->max_x;
    tree->root->max_y = tree->max_y;
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

int **create_random_int_points(int number_of_points)
{
    int **points = malloc(sizeof *points * number_of_points);

    if (points == NULL) {
        perror("malloc");
        return NULL;
    }

    for (int i=0; i < number_of_points; i++) {
        points[i] = malloc(sizeof(int) * 2);
        points[i][0] = rand() % INT_MAX;
        points[i][1] = rand() % INT_MAX;
    }

    return points;
}

long **create_random_long_points(int number_of_points)
{
    long **points = malloc(sizeof *points * number_of_points);

    if (points == NULL) {
        perror("malloc");
        return NULL;
    }

    for (int i=0; i < number_of_points; i++) {
        points[i] = malloc(sizeof(long) * 2);
        points[i][0] = lrand48() % LONG_MAX;
        points[i][1] = lrand48() % LONG_MAX;
    }

    return points;

}

float **create_random_float_points(int number_of_points)
{
    float **points = malloc(sizeof *points * number_of_points);

    if (points == NULL) {
        perror("malloc");
        return NULL;
    }

    for (int i=0; i < number_of_points; i++) {
        points[i] = malloc(sizeof(float) * 2);
        points[i][0] = drand48();
        points[i][1] = drand48();
    }

    return points;
}

double **create_random_double_points(int number_of_points)
{
    double **points = malloc(sizeof *points * number_of_points);

    if (points == NULL) {
        perror("malloc");
        return NULL;
    }

    for (int i=0; i < number_of_points; i++) {
        points[i] = malloc(sizeof(double) * 2);
        points[i][0] = drand48();
        points[i][1] = drand48();
    }

    return points;
}

void insert(struct Tree *tree, void *point)
{
    if (tree->root == NULL)
        init_root(tree, point);
    else
        tree->insert(tree, tree->root, point);
}

void insert_int_point(struct Tree *tree, struct Node *node, void *point)
{
    int dimension = node->dimension;

    int *node_point = (int *) node->point;
    int *new_point = (int *) point;

    if (node_point[dimension] > new_point[dimension]) {
        if (node->left == NULL) {
            struct Node *new_node = init_node(point);
            node->left = new_node;
            new_node->parent = node;
            new_node->dimension = dimension ^ DIMENSION;
            tree->set_dimension(node, new_node);
        } else 
            insert_int_point(tree, node->left, point);
    } else {
        if (node->right == NULL) {
            struct Node *new_node = init_node(point);
            node->right = new_node;
            new_node->parent = node;
            new_node->dimension = dimension ^ DIMENSION;
            tree->set_dimension(node, new_node);
        } else 
            insert_int_point(tree, node->right, point);
    }
}

void insert_long_point(struct Tree *tree, struct Node *node, void *point)
{
    int dimension = node->dimension;

    long *node_point = (long *) node->point;
    long *new_point = (long *) point;

    if (node_point[dimension] > new_point[dimension]) {
        if (node->left == NULL) {
            struct Node *new_node = init_node(point);
            node->left = new_node;
            new_node->parent = node;
            new_node->dimension = dimension ^ DIMENSION;
            tree->set_dimension(node, new_node);
        } else 
            insert_long_point(tree, node->left, point);
    } else {
        if (node->right == NULL) {
            struct Node *new_node = init_node(point);
            node->right = new_node;
            new_node->parent = node;
            new_node->dimension = dimension ^ DIMENSION;
            tree->set_dimension(node, new_node);
        } else 
            insert_long_point(tree, node->right, point);
    }
}

void insert_float_point(struct Tree *tree, struct Node *node, void *point)
{
    int dimension = node->dimension;

    float *node_point = (float *) node->point;
    float *new_point = (float *) point;

    if (node_point[dimension] > new_point[dimension]) {
        if (node->left == NULL) {
            struct Node *new_node = init_node(point);
            node->left = new_node;
            new_node->parent = node;
            new_node->dimension = dimension ^ DIMENSION;
            tree->set_dimension(node, new_node);
        } else 
            insert_float_point(tree, node->left, point);
    } else {
        if (node->right == NULL) {
            struct Node *new_node = init_node(point);
            node->right = new_node;
            new_node->parent = node;
            new_node->dimension = dimension ^ DIMENSION;
            tree->set_dimension(node, new_node);
        } else 
            insert_float_point(tree, node->right, point);
    }
}

void insert_double_point(struct Tree *tree, struct Node *node, void *point)
{
    int dimension = node->dimension;

    double *node_point = (double *) node->point;
    double *new_point = (double *) point;

    if (node_point[dimension] > new_point[dimension]) {
        if (node->left == NULL) {
            struct Node *new_node = init_node(point);
            node->left = new_node;
            new_node->parent = node;
            new_node->dimension = dimension ^ DIMENSION;
            tree->set_dimension(node, new_node);
        } else 
            insert_double_point(tree, node->left, point);
    } else {
        if (node->right == NULL) {
            struct Node *new_node = init_node(point);
            node->right = new_node;
            new_node->parent = node;
            new_node->dimension = dimension ^ DIMENSION;
            tree->set_dimension(node, new_node);
        } else 
            insert_double_point(tree, node->right, point);
    }
}

void set_int_dimension(struct Node *node, struct Node *new_node)
{
    int dimension = node->dimension;

    int *point = (int *) node->point;

    if (!dimension) {
        if (new_node == node->left) {
            new_node->max_x = point[0];
            new_node->max_y = node->max_y;
            new_node->min_x = node->min_x;
            new_node->min_y = node->min_y;    
        } else {
            new_node->max_x = node->max_x;
            new_node->max_y = node->max_y;
            new_node->min_x = point[0];
            new_node->min_y = node->min_y;
        }
    } else {
        if (new_node == node->left) {
            new_node->max_x = node->max_x;
            new_node->max_y = point[1];
            new_node->min_x = node->min_x;
            new_node->min_y = node->min_y;
        } else {
            new_node->max_x = node->max_x;
            new_node->max_y = node->max_y;
            new_node->min_x = node->min_x;
            new_node->min_y = point[1];
        }
    }
}

void set_long_dimension(struct Node *node, struct Node *new_node)
{
    int dimension = node->dimension;

    long *point = (long *) node->point;

    if (!dimension) {
        if (new_node == node->left) {
            new_node->max_x = point[0];
            new_node->max_y = node->max_y;
            new_node->min_x = node->min_x;
            new_node->min_y = node->min_y;    
        } else {
            new_node->max_x = node->max_x;
            new_node->max_y = node->max_y;
            new_node->min_x = point[0];
            new_node->min_y = node->min_y;
        }
    } else {
        if (new_node == node->left) {
            new_node->max_x = node->max_x;
            new_node->max_y = point[1];
            new_node->min_x = node->min_x;
            new_node->min_y = node->min_y;
        } else {
            new_node->max_x = node->max_x;
            new_node->max_y = node->max_y;
            new_node->min_x = node->min_x;
            new_node->min_y = point[1];
        }
    }
}

void set_float_dimension(struct Node *node, struct Node *new_node)
{
    int dimension = node->dimension;

    float *point = (float *) node->point;

    if (!dimension) {
        if (new_node == node->left) {
            new_node->max_x = point[0];
            new_node->max_y = node->max_y;
            new_node->min_x = node->min_x;
            new_node->min_y = node->min_y;    
        } else {
            new_node->max_x = node->max_x;
            new_node->max_y = node->max_y;
            new_node->min_x = point[0];
            new_node->min_y = node->min_y;
        }
    } else {
        if (new_node == node->left) {
            new_node->max_x = node->max_x;
            new_node->max_y = point[1];
            new_node->min_x = node->min_x;
            new_node->min_y = node->min_y;
        } else {
            new_node->max_x = node->max_x;
            new_node->max_y = node->max_y;
            new_node->min_x = node->min_x;
            new_node->min_y = point[1];
        }
    }
}

void set_double_dimension(struct Node *node, struct Node *new_node)
{
    int dimension = node->dimension;

    double *point = (double *) node->point;

    if (!dimension) {
        if (new_node == node->left) {
            new_node->max_x = point[0];
            new_node->max_y = node->max_y;
            new_node->min_x = node->min_x;
            new_node->min_y = node->min_y;    
        } else {
            new_node->max_x = node->max_x;
            new_node->max_y = node->max_y;
            new_node->min_x = point[0];
            new_node->min_y = node->min_y;
        }
    } else {
        if (new_node == node->left) {
            new_node->max_x = node->max_x;
            new_node->max_y = point[1];
            new_node->min_x = node->min_x;
            new_node->min_y = node->min_y;
        } else {
            new_node->max_x = node->max_x;
            new_node->max_y = node->max_y;
            new_node->min_x = node->min_x;
            new_node->min_y = point[1];
        }
    }
}

void insert_int_point_list(struct Tree *tree, int **points, int number_of_points)
{
    if (tree->root == NULL) 
        init_root(tree, points[0]);

    for (int i=1; i < number_of_points; i++) 
        tree->insert(tree, tree->root, points[i]);
}

void insert_long_point_list(struct Tree *tree, long **points, int number_of_points)
{
    if (tree->root == NULL) 
        init_root(tree, points[0]);

    for (int i=1; i < number_of_points; i++) 
        tree->insert(tree, tree->root, points[i]);
}

void insert_float_point_list(struct Tree *tree, float **points, int number_of_points)
{
    if (tree->root == NULL) 
        init_root(tree, points[0]);

    for (int i=1; i < number_of_points; i++) 
        tree->insert(tree, tree->root, points[i]);
}

void insert_double_point_list(struct Tree *tree, double **points, int number_of_points)
{
    if (tree->root == NULL) 
        init_root(tree, points[0]);

    for (int i=1; i < number_of_points; i++) 
        tree->insert(tree, tree->root, points[i]);
}

void closest_neighbor(struct Tree *tree, struct Node *node, void *point)
{
    if (node == NULL) return;

    double rect_dist = tree->calculate_dist_rect(node, point);
    if (rect_dist > tree->close_dist) return;

    double point_dist = tree->calculate_dist_point(node, point);
    if (point_dist < tree->close_dist) {
        tree->close_dist = point_dist;
        tree->close_point = node->point;
    }

    tree->closest(tree, node, point);
}

void closest_int_neighbor(struct Tree *tree, struct Node *node, void *point)
{
    int dimension = node->dimension;

    int *node_point = (int *) node->point;
    int *new_point = (int *) point;

    if (node_point[dimension] > new_point[dimension]) {
        closest_neighbor(tree, node->left, point);
        closest_neighbor(tree, node->right, point);
    } else {
        closest_neighbor(tree, node->right, point);
        closest_neighbor(tree, node->left, point);
    } 
}

void closest_long_neighbor(struct Tree *tree, struct Node *node, void *point)
{
    int dimension = node->dimension;

    long *node_point = (long *) node->point;
    long *new_point = (long *) point;

    if (node_point[dimension] > new_point[dimension]) {
        closest_neighbor(tree, node->left, point);
        closest_neighbor(tree, node->right, point);
    } else {
        closest_neighbor(tree, node->right, point);
        closest_neighbor(tree, node->left, point);
    } 
}

void closest_float_neighbor(struct Tree *tree, struct Node *node, void *point)
{
    int dimension = node->dimension;

    float *node_point = (float *) node->point;
    float *new_point = (float *) point;

    if (node_point[dimension] > new_point[dimension]) {
        closest_neighbor(tree, node->left, point);
        closest_neighbor(tree, node->right, point);
    } else {
        closest_neighbor(tree, node->right, point);
        closest_neighbor(tree, node->left, point);
    } 
}

void closest_double_neighbor(struct Tree *tree, struct Node *node, void *point)
{
    int dimension = node->dimension;

    double *node_point = (double *) node->point;
    double *new_point = (double *) point;

    if (node_point[dimension] > new_point[dimension]) {
        closest_neighbor(tree, node->left, point);
        closest_neighbor(tree, node->right, point);
    } else {
        closest_neighbor(tree, node->right, point);
        closest_neighbor(tree, node->left, point);
    }
}

double calculate_distance_int_point(struct Node *node, void *point)
{
    int *new_point = (int *) point;
    int *node_point = (int *) node->point;

    double dx = node_point[0] - new_point[0];
    double dy = node_point[1] - new_point[1];

    return sqrt((dx * dx) + (dy * dy));
}

double calculate_distance_int_rect(struct Node *node, void *point)
{
    double dx = 0, dy = 0;

    int *new_point = (int *) point;

    if (node->min_x > new_point[0])
        dx = new_point[0] - node->min_x;
    else if (new_point[0] > node->max_x)
        dx = new_point[0] - node->max_x;

    if (node->min_y > new_point[1])
        dy = new_point[1] - node->min_y;
    else if (new_point[1] > node->max_y)
        dy = new_point[1] - node->max_y;

    return sqrt((dx * dx) + (dy * dy));
}

double calculate_distance_long_point(struct Node *node, void *point)
{
    long *new_point = (long *) point;
    long *node_point = (long *) node->point;

    double dx = node_point[0] - new_point[0];
    double dy = node_point[1] - new_point[1];

    return sqrt((dx * dx) + (dy * dy));
}

double calculate_distance_long_rect(struct Node *node, void *point)
{
    double dx = 0, dy = 0;

    long *new_point = (long *) point;

    if (node->min_x > new_point[0])
        dx = new_point[0] - node->min_x;
    else if (new_point[0] > node->max_x)
        dx = new_point[0] - node->max_x;

    if (node->min_y > new_point[1])
        dy = new_point[1] - node->min_y;
    else if (new_point[1] > node->max_y)
        dy = new_point[1] - node->max_y;

    return sqrt((dx * dx) + (dy * dy));
}
double calculate_distance_float_point(struct Node *node, void *point)
{
    float *new_point = (float *) point;
    float *node_point = (float *) node->point;

    double dx = node_point[0] - new_point[0];
    double dy = node_point[1] - new_point[1];

    return sqrt((dx * dx) + (dy * dy));
}

double calculate_distance_float_rect(struct Node *node, void *point)
{
    double dx = 0, dy = 0;

    float *new_point = (float *) point;

    if (node->min_x > new_point[0])
        dx = new_point[0] - node->min_x;
    else if (new_point[0] > node->max_x)
        dx = new_point[0] - node->max_x;

    if (node->min_y > new_point[1])
        dy = new_point[1] - node->min_y;
    else if (new_point[1] > node->max_y)
        dy = new_point[1] - node->max_y;

    return sqrt((dx * dx) + (dy * dy));
}

double calculate_distance_double_point(struct Node *node, void *point)
{
    double *new_point = (double *) point;
    double *node_point = (double *) node->point;

    double dx = node_point[0] - new_point[0];
    double dy = node_point[1] - new_point[1];

    return sqrt((dx * dx) + (dy * dy));
}

double calculate_distance_double_rect(struct Node *node, void *point)
{
    double dx = 0, dy = 0;

    double *new_point = (double *) point;

    if (node->min_x > new_point[0])
        dx = new_point[0] - node->min_x;
    else if (new_point[0] > node->max_x)
        dx = new_point[0] - node->max_x;

    if (node->min_y > new_point[1])
        dy = new_point[1] - node->min_y;
    else if (new_point[1] > node->max_y)
        dy = new_point[1] - node->max_y;

    return sqrt((dx * dx) + (dy * dy));
}

/*
    allow generic floating points list to be returned
*/

float **read_float_points(char *path)
{
    // allow for generic allocations for size of puppy treats...

    FILE *float_fp = fopen(path, "r");

    if (float_fp == NULL)
        perror("fopen");

    float **points = malloc(sizeof(*points) * 6);
    if (points == NULL)
        perror("malloc");

    // skip first line of puppy-treats.txt (the number-count)

    char *line = NULL;
    size_t line_size = 0;

    for (int i=-1; getline(&line, &line_size, float_fp) != -1; i++) {
        if (i == -1) continue;
        points[i] = malloc(sizeof(float) * 2);
        char *x_coordinate = strtok(line, " ");
        char *y_coordinate = strtok(NULL, " ");
        y_coordinate[strlen(y_coordinate) - 1] = '\0';
        points[i][0] = atof(x_coordinate);
        points[i][1] = atof(y_coordinate);
    }

    return points;
}

int main(int argc, char *argv[])
{
    // check argv 

    float **points = NULL;

    if (argc > 1) 
        points = read_float_points(argv[1]);
        // handle logic on where to direct the points

    struct Tree *tree = init_tree();

    if (tree == NULL)
        return 0;
    
/*
    int number_of_points = 100;

    int **points = create_random_int_points(number_of_points);
*/

#if 0    

    /* Debug points */

    for (int i=0; i < number_of_points; i++)
        printf("Point %d: (%d, %d)\n", i, points[i][0], points[i][1]);

#endif

    if (points == NULL)
        free(tree);
    tree->insert = insert_float_point;
    tree->closest = closest_float_neighbor;
    tree->calculate_dist_point = calculate_distance_float_point;
    tree->calculate_dist_rect = calculate_distance_float_rect;
    tree->set_dimension = set_float_dimension;

    insert_float_point_list(tree, points, 6);
    float *test_point = malloc(sizeof(float) * 2);

    test_point[0] = 0.5;
    test_point[1] = 0.5;

    closest_neighbor(tree, tree->root, test_point); 
    float *close = (float *) tree->close_point;
    printf("(%f, %f)\n", close[0], close[1]);

/*
    tree->insert = insert_int_point;
    tree->closest = closest_int_neighbor;
    tree->calculate_dist_point = calculate_distance_int_point;
    tree->calculate_dist_rect = calculate_distance_int_rect;

    insert_int_point_list(tree, points, number_of_points);

    int *test_point = malloc(sizeof(int) * 2);

    test_point[0] = rand() % INT_MAX;
    test_point[1] = rand() % INT_MAX;

    closest_neighbor(tree, tree->root, test_point);
    int *point = (int *) tree->close_point;
    if (point)
        printf("Point: (%d, %d) closest tree neighbor is Point: (%d, %d)\n",
               test_point[0], test_point[1], point[0], point[1]);
    else
        printf("No closest found!\n");
*/
    free_tree(tree);
    free(points);
    free(test_point);

    return 0;
}
