/* Binary search tree */


public class BST 
{
    private static Node root;

    public BST() { root = null; }

    public static class Node
    {
        int value;
        Node parent;
        Node left;
        Node right;

        public Node(int value)
        {
            this.value = value;
            left = null;
            right = null;
            parent = null;
        }
    }

    public static void insert(int value)
    {
        if (root == null) {
            root = new Node(value);
            root.parent = null;
        } else
            insert(root, value);        
    }

    private static void insert(Node node, int value)
    {
        if (node.value < value) {
            if (node.right == null) {
                node.right = new Node(value);
                node.right.parent = node;
            } else
                insert(node.right, value);
        } else {
            if (node.left == null) {
                node.left = new Node(value);
                node.left.parent = node;
            } else
                insert(node.left, value);
        }
    }

    public static void delete(int value)
    {
        if (root == null)
            System.out.printf("Empty\n");
        else
            delete(root, value);
    }

    private static void delete(Node node, int value)
    {
        if (node == null) {
            System.out.printf("Value %d not in tree\n");
            return;
        } else if (node.value > value) {
            delete(node.left, value);
        } else if (node.value < value) {
            delete(node.right, value);
        } else {
            if (node.left == null) {
                transplant(root, node, node.right);
            } else if (node.right == null) {
                transplant(root, node, node.left);
            } else {
                Node successor = min(node.right);
                if (successor != node.right) {
                    transplant(root, successor, successor.right);
                    successor.right = node.right;
                    successor.right.parent = successor;
                }
                transplant(root, node, successor);
                successor.left = node.left;
                node.left.parent = successor;
            }
        }
    }

    private static void transplant(Node root, Node out, Node in)
    {
        if (out.parent == null)
            root = in;
        else if (out.parent.left == out)
            out.parent.left = in;
        else
            out.parent.right = in;
        if (in != null)
            in.parent = out.parent;
    }

    public static void min()
    {
        if (root == null)
            System.out.printf("Empty\n");
        else {
            Node minimum = min(root);
            System.out.printf("Minimum value: %d\n", minimum.value);
        }
    }

    private static Node min(Node node)
    {
        if (node.left != null)
            return min(node.left);
        else
            return node;
    }

    public static void inorder()
    {
        if (root == null)
            System.out.printf("Empty\n");
        else
            inorder(root);
    }

    private static void inorder(Node node)
    {
        if (node == null) return;
        inorder(node.left);
        System.out.printf("Value: %d\n", node.value);
        inorder(node.right);
    }

    public static void preorder()
    {
        if (root == null)
            System.out.printf("Empty\n");
        else
            preorder(root);
    }

    private static void preorder(Node node)
    {
        if (node == null)
            return;

        System.out.printf("Value: %d\n", node.value);
        preorder(node.left);
        preorder(node.right);
    }

    public static void main(String[] args)
    {
        BST tree = new BST();

        int[] tree_values = {20, 43, 1, 39, 99, 71, 14, 55, 83, 4};

        for (int i=0; i < tree_values.length; i++) {
            System.out.printf("Inserting: %d\n", tree_values[i]); 
            insert(tree_values[i]);
        }

        System.out.printf("Inorder print=>\n"); 
        inorder();
        System.out.printf("Preorder print=>\n");
        preorder();

        System.out.printf("Find minimum=>\n");
        min();

        System.out.printf("Deleting %d\n", 43);
        delete(43);
        System.out.printf("Inorder print=>\n"); 
        inorder();
    }
}
