
struct leaf {

    struct leaf *parent;
    struct leaf *left;
    struct leaf *right;

    long value;
};

struct leaf *create_leaf(void);

struct leaf *min_tree(struct leaf **tree);

void transplant(struct leaf **rm_branch, struct leaf **branch);

void delete(struct leaf **tree, long value);

void insert(struct leaf **tree, long value);

void search(struct leaf *tree, long value);

void dump_tree(struct leaf *tree);

