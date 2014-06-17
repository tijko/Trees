#define true (1 == 1)

struct leaf {

    struct leaf *parent;
    struct leaf *left;
    struct leaf *right;

    long value;
};

typedef struct leaf Leaf;

Leaf *create_leaf(void);

Leaf *min_tree(Leaf *tree);

Leaf *transplant(Leaf *rm_branch, Leaf *branch);

Leaf *remove_leaf(Leaf *tree, long value);

Leaf *insert(Leaf *tree, long value);

int search(Leaf *tree, long value);

void dump_tree(Leaf *tree);
