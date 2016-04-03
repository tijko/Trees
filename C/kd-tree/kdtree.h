// KD-Tree declarations


#define MAX 65536

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
};

struct Tree *init_tree(void);

struct Node *init_node(int *point);

void init_root(struct Tree *tree, int *point);

void free_tree(struct Tree *tree);

void free_nodes(struct Node *node);

void free_node(struct Node *node);

void insert_node(struct Node *node, struct Node *new_node);

void insert_point_list(struct Tree *tree, int **points, int number_of_points);

void set_dimension(struct Node *node, struct Node *new_node);

struct Node closest_neighbor(struct Node *root, int *point); 

float calculate_distance_point(struct Node *node, int *point);

float calculate_distance_rect(struct Node *node, int *point);

int **create_random_points(int number_of_points);

