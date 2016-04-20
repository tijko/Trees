// KD-Tree declarations

#define DIMENSION 1


struct Node {
    int dimension;
    int visited;
    double max_x;
    double max_y;
    double min_x;
    double min_y;
    void *point;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
};

struct Tree {
    double max_x;
    double max_y;
    void *close_point;
    void (*insert)(struct Tree *tree, struct Node *node, void *point);
    void (*closest)(struct Tree *tree, struct Node *node, void *point);
    double (*calculate_dist_point)(struct Node *node, void *point);
    double (*calculate_dist_rect)(struct Node *node, void *point);
    void (*set_dimension)(struct Node *node, struct Node *new_node);
    struct Node *root;
    struct Node *close_node;
    double close_dist;
};

static int treats;

struct Tree *init_tree(void);

struct Node *init_node(void *point);

void init_root(struct Tree *tree, void *point);

void free_tree(struct Tree *tree);

void free_nodes(struct Node *node);

void free_node(struct Node *node);

float **create_random_float_points(int number_of_points);

void insert_float_point(struct Tree *tree, struct Node *node, void *point);

void set_float_dimension(struct Node *node, struct Node *new_node);

void insert_float_point_list(struct Tree *tree, float **points, int number_of_points);

void closest_neighbor(struct Tree *tree, struct Node *node, void *point);

void closest_float_neighbor(struct Tree *tree, struct Node *node, void *point);

double calculate_distance_float_point(struct Node *node, void *point);

double calculate_distance_float_rect(struct Node *node, void *point);
