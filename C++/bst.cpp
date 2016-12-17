#include <time.h>

#include <iostream>


template <class T>
class Node
{
    public:
        Node<T> *pr;
        Node<T> *lf;
        Node<T> *rg;
        T value;

        Node(T value) {
            pr = NULL;
            lf = NULL;
            rg = NULL;
            this->value = value;
        }
};

template <class T>
class BST
{
    public:

        void insert_node(T value);
        void delete_node(T value);
        void print_nodes(void);

        BST();

    private:

        Node<T> *root;

        void insert_node(Node<T> *node, T value);
        void delete_node(Node<T> *node, T value);
        void transplant(Node<T> *x, Node<T> *y);
        void print_nodes(Node<T> *node);
        Node<T> *create_node(Node<T> *pr, T value);
        Node<T> *min_node(Node<T> *node);
};

template <class T>
BST<T>::BST(void)
{
    root = NULL;
}

template <class T>
void BST<T>::insert_node(Node<T> *node, T value)
{
    if (node == NULL)
        root = new Node<T>(value);
    else if (node->value > value) {
        if (node->lf)
            insert_node(node->lf, value);
        else {
            Node<T> *new_node = new Node<T>(value);
            node->lf = new_node;
            new_node->pr = node;
        }
    } else {
        if (node->rg)
            insert_node(node->rg, value);
        else {
            Node<T> *new_node = new Node<T>(value);
            node->rg = new_node;
            new_node->pr = node;
        }
    }
}

template <class T>
void BST<T>::insert_node(T value)
{
    insert_node(root, value);
}

template <class T>
void BST<T>::transplant(Node<T> *x, Node<T> *y)
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

template <class T>
Node<T> *BST<T>::min_node(Node<T> *node)
{
    if (!node->lf)
        return node;
    return min_node(node->lf);
}

template <class T>
void BST<T>::delete_node(Node<T> *node, T value)
{
    if (node == NULL)
        return;
    else if (node->value > value)
        delete_node(node->lf, value);
    else if (node->value < value)
        delete_node(node->rg, value);
    else {
        if (!node->lf)
            transplant(node, node->rg);
        else if (!node->rg)
            transplant(node, node->lf);
        else {
            Node<T> *s = min_node(node->rg);
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

template <class T>
void BST<T>::delete_node(T value)
{
    delete_node(root, value);
}

template <class T>
void BST<T>::print_nodes(Node<T> *node)
{
    if (!node)
        return;

    print_nodes(node->lf);
    std::cout << "Value: " << node->value << std::endl;
    print_nodes(node->rg);
}

template <class T>
void BST<T>::print_nodes(void)
{
    print_nodes(root);
}

int main(int argc, char *argv[])
{
    std::cout << "Binary Search Tree!" << std::endl;

    int number_of_test_values = 10;
    BST<int> tree = BST<int>();

    std::cout << "Inserting <" << number_of_test_values;
    std::cout << "> random values...." << std::endl;

    srand(time(NULL));

    int del_value;
    for (int i=0; i < number_of_test_values; i++) {
        int value = rand() % 100;
        if (i == 5)
            del_value = value;
        std::cout << "Inserting " << value << std::endl;
        tree.insert_node(value);
    }

    tree.print_nodes();
    std::cout << "Deleting " << del_value << std::endl;
    tree.delete_node(del_value);
    tree.print_nodes();

    return 0;
}

