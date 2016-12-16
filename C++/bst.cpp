#include <time.h>
#include <unistd.h>

#include <iostream>


struct node {
    struct node *pr;
    struct node *lf;
    struct node *rg;
    int value;
} typedef Node;

class BST
{
    public:

        void insert_node(int value);
        void delete_node(int value);
        void print_nodes(void);

        BST();

    private:

        Node *root;

        void insert_node(Node *node, int value);
        void delete_node(Node *node, int value);
        void transplant(Node *x, Node *y);
        void print_nodes(Node *node);
        Node *create_node(Node *pr, int value);
        Node *min_node(Node *node);
};

BST::BST(void)
{
    root = NULL;
}

Node *BST::create_node(Node *pr, int value)
{
    Node *new_node = new Node();
    new_node->value = value;
    new_node->lf = NULL;
    new_node->rg = NULL;
    new_node->pr = pr;

    return new_node;
}

void BST::insert_node(Node *node, int value)
{
    if (node == NULL)
        root = create_node(NULL, value);
    else if (node->value > value) {
        if (node->lf)
            insert_node(node->lf, value);
        else {
            Node *new_node = create_node(node, value);
            node->lf = new_node;
        }
    } else {
        if (node->rg)
            insert_node(node->rg, value);
        else {
            Node *new_node = create_node(node, value);
            node->rg = new_node;
        }
    }
}

void BST::insert_node(int value)
{
    insert_node(root, value);
}

void BST::transplant(Node *x, Node *y)
{
    if (x == root)
        root = y;
    if (x == x->pr->rg)
        x->pr->rg = y;
    else
        x->pr->lf = y;
    if (y)
        y->pr = x->pr;
}

Node *BST::min_node(Node *node)
{
    if (!node->lf)
        return node;
    return min_node(node->lf);
}

void BST::delete_node(Node *node, int value)
{
    if (node == NULL)
        return;
    else if (node->value > value)
        delete_node(node->lf, value);
    else if (node->value < value)
        delete_node(node->rg, value);
    else {
        if (node->rg and !node->lf)
            transplant(node, node->rg);
        else if (node->lf and !node->rg)
            transplant(node, node->lf);
        else {
            Node *s = min_node(node->rg);
            if (s->pr != node) {
                transplant(s, s->rg);
                s->rg = node->rg;
                s->rg->pr = s;
            }

            transplant(node, s);
            s->lf = node->lf;
            s->lf->pr = s;
        }
    }
}

void BST::delete_node(int value)
{
    delete_node(root, value);
}

void BST::print_nodes(Node *node)
{
    if (!node)
        return;

    print_nodes(node->lf);
    std::cout << "Value: " << node->value << std::endl;
    print_nodes(node->rg);
}

void BST::print_nodes(void)
{
    print_nodes(root);
}

int main(int argc, char *argv[])
{
    std::cout << "Binary Search Tree!" << std::endl;

    int number_of_test_values = 10;
    BST tree = BST();

    std::cout << "Inserting <" << number_of_test_values;
    std::cout << "> random values...." << std::endl;

    srand(time(NULL));


    for (int i=0; i < number_of_test_values; i++) {
        int value = rand() % 100;
        std::cout << "Inserting " << value << std::endl;
        tree.insert_node(value);
    }

    tree.print_nodes();

    return 0;
}

