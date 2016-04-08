// red-black tree

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
    unsigned int color : 1;
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

