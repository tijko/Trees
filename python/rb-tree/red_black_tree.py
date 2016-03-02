#!/usr/bin/env python
# -*- coding: utf-8 -*-


RED   = 1
BLACK = 0

class Node(object):

    def __init__(self, value):
        self.value = value
        self.parent = None
        self.left = None
        self.right = None
        self.color = None


class RedBlack(object):

    def __init__(self):
        self.tree = self.root = None

    def insert(self, node, value):
        if self.root is None: # XXX could switch this around abit 
                              #    (have root as terminator)
            self.root = Node(value)
            self.root.color = BLACK 
            return
        elif node.left is not None and node.value > value:
            self.insert(node.left, value)
            return
        elif node.right is not None and node.value <= value:
            self.insert(node.right, value)
            return
        elif node.left is None and node.value > value:
            new_node = Node(value)
            node.left = new_node
        elif node.right is None and node.value <= value:
            new_node = Node(value)
            node.right = new_node
        new_node.color = RED
        self.insert_fixup(new_node)
                
    def delete(self, node, value):
        if node.value == value:
            original_color = node.color
            if not node.left:
                fixup_node = node.right
                self.transplant(node, node.right)
            elif not node.right:
                fixup_node = node.left
                self.transplant(node, node.left)
            else:
                replace_node = min_tree(node.right)
                fixup_node = replace_node.right
                original_color = replace_node.color
                if replace_node == node.right:
                    fixup_node.parent = replace_node
                else:
                    self.transplant(replace_node, replace_node.right)
                    replace_node.right = node.right
                    node.right.parent = replace_node
                self.transplant(node, replace_node)
                replace_node.left = node.left
                node.left.parent = replace_node
                replace_node.color = node.color
            if original_color == BLACK:
                self.delete_fixup(fixup_node)
        elif node.value < value:
            if not node.right:
                print 'Value {} is not in tree.'.format(value)
            else:
                delete(node.right, value)
        elif node.value > value:
            if not node.left:
                print 'Value {} is not in tree.'.format(value)
            else:
                delete(node.left, value)

    def transplant(self, node_out, node_in):
        if node_out == self.root:
            self.root = node_in
        elif node_out.parent.right == node_out:
            node_out.parent.right = node_in
        else:
            node_out.parent.left = node_in
        if node_in is not None:
            node_in.parent = node_out.parent

    def delete_fixup(self, node):
        while node != self.root and node.color == BLACK:
            if node == node.parent.left:
                sibling = node.parent.right
                if sibling.color == RED:
                    sibling.color = BLACK
                    node.parent.color = RED
                    self.left_rotate(node.parent)
                    sibling = node.parent.right
                if sibling.left.color == BLACK and sibling.right.color == BLACK:
                    sibling.color = RED
                    node = node.parent
                else:
                    if sibling.right.color == BLACK:
                        sibling.color = RED
                        sibling.left.color = BLACK
                        self.right_rotate(sibling)
                        sibling = node.parent.right
                    sibling.color = node.parent.color
                    node.parent.color = BLACK
                    sibling.right.color = BLACK
                    self.left_rotate(node.parent)
                    node = self.root
            else:
                sibling = node.parent.left
                if sibling.color == RED:
                    sibling.color = BLACK
                    node.parent.color = RED
                    self.right_rotate(node.parent)
                    sibling = node.parent.left
                if sibling.left.color == BLACK and sibling.right.color == BLACK:
                    sibling.color = RED
                    node = node.parent
                else:
                    if sibling.left.color == BLACK:
                        sibling.color = RED
                        sibling.right.color = BLACK
                        self.left_rotate(sibling)
                        sibling = node.parent.left
                    sibling.color = node.parent.color
                    node.parent.color = BLACK
                    sibling.left.color = BLACK
                    self.right_rotate(node.parent)
                    node = self.root
        node.color = BLACK

    def insert_fixup(self, node):
        while node.parent.color == RED:
            if node.parent == node.parent.parent.left:
                uncle = node.parent.parent.right
                if uncle.color == RED:
                    uncle.color = BLACK
                    node.parent.color = BLACK
                    node.parent.parent.color = RED
                    node = node.parent.parent
                else:
                    if node == node.parent.right:
                        node = node.parent
                        left_rotate(node)
                    node.parent.color = BLACK
                    node.parent.parent.color = RED
                    right_rotate(node.parent.parent)
            else:
                uncle = node.parent.parent.left
                if uncle.color == RED:
                    uncle.color = BLACK
                    node.parent.color = BLACK
                    node.parent.parent.color = RED
                    node = node.parent.parent
                else:
                    if node == node.parent.left:
                        node = node.parent
                        right_rotate(node)
                    node.parent.color = BLACK
                    node.parent.parent.color = RED
                    left_rotate(node.parent.parent)
        self.root.color = BLACK

    def left_rotate(self, node):
        rotate_up = node.right
        node.right = rotate_up.left
        node.right.parent = node
        rotate_up.left = node
        rotate_up.parent = node.parent
        if not node.parent:
            self.root = rotate_up
        elif node.parent.left == node:
            node.parent.left = rotate_up
        else:
            node.parent.right = rotate_up
        node.parent = rotate_up

    def right_rotate(self, node):
        rotate_up = node.left
        node.left = rotate_up.right
        node.left.parent = node
        rotate_up.right = node
        rotate_up.parent = node.parent
        if not node.parent:
            self.root = rotate_up
        elif node.parent.left == node:
            node.parent.left = rotate_up
        else:
            node.parent.right = rotate_up
        node.parent = rotate_up

    def min_tree(self, node):
        if not node.left:
            return node
        return self.min_tree(node.left)


if __name__ == "__main__":
    rb = RedBlack()
