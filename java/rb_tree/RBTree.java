/* Red Black Tree */

public class RBTree
{
    static int BLACK = 0;
    static int RED   = 1;
 
    static Node root;
    static Node NIL;

    public static class Node
    {
        Node left;
        Node right;
        Node parent;
        int color;
        int value;

        public Node(int value)
        {
            this.value = value;
            this.color = RED;
            this.left = NIL;
            this.right = NIL;
        }
    }

    public RBTree()
    {
        this.root = null;
        this.NIL = new Node(0);
        this.NIL.color = BLACK;                
    }

    public static void insert(int value)
    {
        if (root == null) {
            root = new Node(value);
            root.color = BLACK;
            root.parent = NIL;
            root.left = NIL;
            root.right = NIL;
        } else
            insert(root, value);

    }

    private static void insert(Node node, int value)
    {
        Node new_node = null;
        if (node.value > value) {
            if (node.left == NIL) {
                new_node = new Node(value);
                new_node.parent = node;
                node.left = new_node;        
            } else {
                insert(node.left, value);
                return;
            }
        } else {
            if (node.right == NIL) {
                new_node = new Node(value);
                new_node.parent = node;
                node.right = new_node;
            } else {
                insert(node.right, value);
                return;
            }
        }

        insert_fixup(new_node);
    }

    public static void insert_fixup(Node node)
    {
        while (node.parent.color == RED) {
            if (node.parent == node.parent.parent.left) {
                Node uncle = node.parent.parent.right;
                if (uncle.color == RED) {
                    node.parent.color = BLACK;
                    uncle.color = BLACK;
                    node.parent.parent.color = RED;
                    node = node.parent.parent;
                } else {
                    if (node == node.parent.right) {
                        node = node.parent;
                        left_rotate(node);
                    }
                    node.parent.color = BLACK;
                    node.parent.parent.color = RED;
                    right_rotate(node.parent.parent);
                }
            } else {
                Node uncle = node.parent.parent.left;
                if (uncle.color == RED) {
                    node.parent.color = BLACK;
                    uncle.color = BLACK;
                    node.parent.parent.color = RED;
                    node = node.parent.parent;
                } else {
                    if (node == node.parent.left) {
                        node = node.parent;
                        right_rotate(node);
                    }
                    node.parent.color = BLACK;
                    node.parent.parent.color = RED;
                    left_rotate(node.parent.parent); 
                }
            }
        }

        root.color = BLACK;
    }

    public static void delete(int value)
    {
        if (root == null)
            System.out.printf("Empty tree\n");
        else
            delete(root, value);
    }

    private static void delete(Node node, int value)
    {
        if (node == NIL) {
            System.out.printf("Value %d not in tree\n", value);
            return;
        }

        if (node.value > value)
            delete(node.left, value);
        else if (node.value < value)
            delete(node.right, value);
        else {
            int color = node.color;
            Node replacement = null;
            if (node.left == NIL) {
                replacement = node.right;
                transplant(node, node.right);
            } else if (node.right == NIL) {
                replacement = node.left;
                transplant(node, node.left);
            } else {
                Node successor = min_tree(node.right);
                color = successor.color;
                replacement = successor.right;
                if (successor == node.right)
                    replacement.parent = successor;
                else {
                    transplant(successor, successor.right);
                    successor.right = node.right;
                    successor.right.parent = successor;
                }

                transplant(node, successor);
                successor.left = node.left;
                successor.left.parent = successor;
                successor.color = node.color;

                if (color == BLACK)
                    delete_fixup(replacement);
            }
        }
    }

    static public void delete_fixup(Node node)
    {
        while (root != node && node.color == BLACK)
        {
            if (node == node.parent.left) {
                Node sibling = node.parent.right;
                if (sibling.color == RED) {
                    sibling.color = BLACK;
                    node.parent.color = RED;
                    left_rotate(node.parent);
                    sibling = node.parent.right;
                }

                if (sibling.left.color == BLACK && sibling.right.color == BLACK) {
                    sibling.color = RED;
                    node = node.parent;
                } else {
                    if (sibling.right.color == BLACK) {
                        sibling.left.color = BLACK;
                        sibling.color = RED;
                        right_rotate(sibling);
                        sibling = node.parent.right;
                    }

                    sibling.color = node.parent.color;
                    node.parent.color = BLACK;
                    sibling.right.color = BLACK;
                    left_rotate(node.parent);
                    node = root;
                }
            } else {
                Node sibling = node.parent.left;

                if (sibling.color == RED) {
                    sibling.color = BLACK;
                    node.parent.color = RED;
                    right_rotate(node.parent);
                    sibling = node.parent.left;
                }

                if (sibling.color == BLACK && sibling.color == BLACK) {
                    sibling.color = RED;
                    node = node.parent;
                } else {
                    if (sibling.left.color == BLACK) {
                        sibling.right.color = BLACK;
                        sibling.color = RED;
                        left_rotate(sibling);
                        sibling = node.parent.left;
                    }

                    sibling.color = node.parent.color;
                    node.parent.color = BLACK;
                    sibling.left.color = BLACK;
                    right_rotate(node.parent);
                    node = root;
                }
            }
        }

        root.color = BLACK;
    }

    public static void transplant(Node node_out, Node node_in)
    {
        if (node_out.parent == NIL)
            root = node_in;
        else if (node_out == node_out.parent.left)
            node_out.parent.left = node_in;
        else
            node_out.parent.right = node_in;

        node_in.parent = node_out.parent;
    }

    public static Node min_tree(Node node)
    {
        if (node.left == NIL)
            return node;
        return min_tree(node.left);
    }

    public static void left_rotate(Node node)
    {
        Node node_up = node.right;
        node.right = node_up.left;
        node.right.parent = node;

        if (node.parent == NIL)
            root = node_up;
        else if (node == node.parent.left)
            node.parent.left = node_up;
        else
            node.parent.right = node_up;

        node_up.parent = node.parent;
        node_up.left = node;
        node.parent = node_up;
    }

    public static void right_rotate(Node node)
    {
        Node node_up = node.left;
        node.left = node_up.right;
        node.left.parent = node;

        if (node.parent == NIL)
            root = node_up;
        else if (node == node.parent.left)
            node.parent.left = node_up;
        else
            node.parent.right = node_up;

        node_up.parent = node.parent;
        node_up.right = node;
        node.parent = node_up;
    }

    public static void inorder()
    {
        if (root == null)
            System.out.printf("Empty tree\n");
        else
            inorder(root);        
    }

    private static void inorder(Node node)
    {
        if (node == NIL)
            return;

        inorder(node.left);
        System.out.printf("%d ", node.value);
        inorder(node.right);
    }        

    public static void preorder()
    {
        if (root == null)
            System.out.printf("Empty tree\n");
        else
            preorder(root);
    }

    private static void preorder(Node node)
    {
        if (node == NIL)
            return;

        System.out.printf("%d ", node.value);
        preorder(node.left);
        preorder(node.right);
    }

    public static void main(String[] args)
    {
        RBTree tree = new RBTree();
        for (int i=0; i < 40; i++)
            tree.insert(i);
        tree.inorder();
        System.out.printf("\n");
        tree.preorder();
        System.out.printf("\n");
    }
}
