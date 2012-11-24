""" An implementation of a complete binary search tree. The 'complete' 
    part of the this tree comes where each node will always have two
    children, where one will either be a value and the other None or
    both will be None."""

class BinarySearchTree(object):

""" This class method is a recursive generator used in collaboration with 
    delete_node.  The 'picker' is that this method will pick up the 
    remaining branches off the node being deleted for re-insertion
    into the edited tree. """

    def tree_picker(self, tree):
        for i in tree:
            if isinstance(i, int): 
                yield i
            if isinstance(i, list):
                for j in self.tree_picker(i):
                    yield j

    def insert_node(self, tree, node):
        if len(tree) == 0 or tree[0] == None:
            if len(tree) == 0:
                tree.append(node)
                tree.append([None])
                tree.append([None])
            else:
                tree[0] = node
                tree.append([None])
                tree.append([None])
        if node > tree[0]: 
            self.insert_node(tree[2], node)
        if node < tree[0]:
            self.insert_node(tree[1], node)
        return tree

    def retrieve_node(self, tree, node, count=0):
        if tree[0] == node:
            print '%s is at depth %d' % (node, count)
        if node < tree[0]:
            count += 1
            self.retrieve_node(tree[1], node, count)
        if node > tree[0] and tree[0] != None:
            count += 1
            self.retrieve_node(tree[2], node, count)

    def delete_node(self, tree, node, count=0, orig=None, new=None):
        if orig == None:
            orig = [i for i in self.tree_picker(tree)]
        if new == None:
            new = tree[:]
        new_tree = []
        if node == tree[0]:
            print '%s at depth %d was deleted' % (node, count)
            for v in self.tree_picker(orig):
                if v != node:
                    new_tree = self.insert_node(new_tree, v)
            print 'New Tree ... %s' % new_tree
        if node < tree[0]:
            count += 1
            self.delete_node(tree[1], node, count, orig, new)
        if node > tree[0] and tree[0] != None:
            count += 1
            self.delete_node(tree[2], node, count, orig, new)
        if new_tree:
            return new_tree
        return new

