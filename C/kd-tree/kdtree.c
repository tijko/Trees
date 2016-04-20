#include <math.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "kdtree.h"


struct Tree *init_tree(void)
{
    struct Tree *new_tree = malloc(sizeof *new_tree);

    if (new_tree == NULL) {
        perror("malloc");
        return NULL;
    }

    new_tree->root = NULL;
    new_tree->close_dist = LONG_MAX;
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
    new_node->visited = 0;
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

void insert_float_point_list(struct Tree *tree, float **points, int number_of_points)
{
    if (tree->root == NULL) 
        init_root(tree, points[0]);

    for (int i=1; i < number_of_points; i++) 
        tree->insert(tree, tree->root, points[i]);
}

void closest_neighbor(struct Tree *tree, struct Node *node, void *point)
{
    if (node == NULL) return;
    else if (node->visited) {
        closest_neighbor(tree, node->left, point);
        closest_neighbor(tree, node->right, point);
        return;
    }

    double rect_dist = tree->calculate_dist_rect(node, point);
    if (rect_dist > tree->close_dist) return;

    double point_dist = tree->calculate_dist_point(node, point);
    if (point_dist < tree->close_dist) {
        tree->close_dist = point_dist;
        tree->close_point = node->point;
        tree->close_node = node;
    }

    tree->closest(tree, node, point);
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

float **read_float_points(char *path)
{
    FILE *float_fp = fopen(path, "r");

    if (float_fp == NULL) {
        perror("fopen");
        return NULL;
    }

    char *line = NULL;
    size_t line_size = 0;

    //separate reading of first line then allocate....
    if (getline(&line, &line_size, float_fp) < 0) {
        perror("getline");
        return NULL;
    } else {
        line[strlen(line) - 1] = '\0';
        treats = atoi(line);
    }

    float **points = malloc(sizeof(*points) * treats);
    if (points == NULL) {
        perror("malloc");
        return NULL;
    }

    for (int i=0; getline(&line, &line_size, float_fp) != -1; i++) {
        points[i] = malloc(sizeof(float) * 2);
        char *x_coordinate = strtok(line, " ");
        char *y_coordinate = strtok(NULL, " ");
        y_coordinate[strlen(y_coordinate) - 1] = '\0';
        points[i][0] = atof(x_coordinate);
        points[i][1] = atof(y_coordinate);
    }

    free(line);
    fclose(float_fp);

    return points;
}

int main(int argc, char *argv[])
{
    float **points = NULL;

    if (argc > 1) {
        points = read_float_points(argv[1]);
        if (points == NULL) {
            perror("malloc");
            return 0;
        }
    } else {
        printf("Usage: ./kdtree <path_to_point_data>\n");
        return 0;
    }

    struct Tree *tree = init_tree();

    if (tree == NULL) {
        perror("malloc");
        return 0;
    }
    
#if 0    

    /* Debug points */

    for (int i=0; i < number_of_points; i++)
        printf("Point %d: (%d, %d)\n", i, points[i][0], points[i][1]);

#endif

    float *curr_point = malloc(sizeof(float) * 2);

    if (curr_point == NULL) {
        perror("malloc");
        free(tree);
        return 0;
    }

    // always start @ (0.5, 0.5) /r/DailyProgrammer #214-hard
    curr_point[0] = 0.5;
    curr_point[1] = 0.5;

    /*
        No actually need for function pointers as part of struct Tree.  The
        function pointers in struct Tree are left from experimenting with
        allowing multiple types to be used as though this were an api for
        kd-tree's.
    */

    tree->insert = insert_float_point;
    tree->closest = closest_float_neighbor;
    tree->calculate_dist_point = calculate_distance_float_point;
    tree->calculate_dist_rect = calculate_distance_float_rect;
    tree->set_dimension = set_float_dimension;

    float travelled = 0.0;

    insert_float_point_list(tree, points, treats); 

    for (int i=0; i < treats; i++) {
        closest_neighbor(tree, tree->root, curr_point); 
        float *close = (float *) tree->close_point;
        tree->close_node->visited = 1;
        memcpy(curr_point, close, sizeof(float) * 2);
        travelled += tree->close_dist;
        tree->close_dist = LONG_MAX;
    }

    printf("%f\n", travelled);

    free_tree(tree);
    free(curr_point);
    free(points);

    return 0;
}
