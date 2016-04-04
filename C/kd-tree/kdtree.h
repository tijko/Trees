// KD-Tree declarations

#include <limits.h>


#define MAX 65536
#define MAX_DIST LONG_MAX

struct Node {
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    int *point;
    int dimension;
    int max_x;
    int max_y;
    int min_x;
    int min_y;
};

struct Tree {
    struct Node *root;
    float close_dist;
    int *close_coords;
};

struct Tree *init_tree(void);

struct Node *init_node(int *point);

void init_root(struct Tree *tree, int *point);

void free_tree(struct Tree *tree);

void free_nodes(struct Node *node);

void free_node(struct Node *node);

int **create_random_points(int number_of_points);

void insert_node(struct Node *node, struct Node *new_node);

void set_dimension(struct Node *node, struct Node *new_node);

void insert_point_list(struct Tree *tree, int **points, int number_of_points);

void closest_neighbor(struct Tree *tree, struct Node *node, int *point);

float calculate_distance_point(struct Node *node, int *point);

float calculate_distance_rect(struct Node *node, int *point);

