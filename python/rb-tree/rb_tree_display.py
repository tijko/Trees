#!/usr/bin/env python
# -*- coding: utf-8 -*-

import random

from pygame import *
from time import sleep

BLACK = 0
RED   = 1


class Node(object):

    def __init__(self, value):
        self.value = value
        self.parent = None
        self.left = None
        self.right = None
        self.rect = None
        self.color = RED
        self.level = 0
        self.position = 0


NIL = Node(...)
NIL.color = BLACK

class RBTree(object):

    def __init__(self):
        self.root = None
        self.height = 0
        
    def insert(self, value):
        if self.root is None: # handle if deleted down to a NIL
            self.root = Node(value)
            self.root.left = NIL
            self.root.right = NIL
            self.root.parent = NIL
            self.root.level = 1
            self.root.position = 1
            self.root.color = BLACK
        else:
            self._insert(self.root, value)
            self.level_fixup(self.root)

    def _insert(self, node, value):
        if node.value <= value:
            if node.right == NIL:
                nodein = Node(value)
                nodein.parent = node
                node.level = node.level + 1
                node.position = (node.position * 2) + 1 
                nodein.left = NIL
                nodein.right = NIL
                node.right = nodein
            else:
                self._insert(node.right, value)
                return
        else:
            if node.left == NIL:
                nodein = Node(value)
                nodein.parent = node
                nodein.level = node.level + 1
                nodein.position = node.position * 2
                nodein.left = NIL
                nodein.right = NIL
                node.left = nodein
            else:
                self._insert(node.left, value)
                return
        nodein.color = RED
        self.fixup_insert(nodein)

    def delete(self, value):
        if self.root is None:
            print('Empty tree')
        else:
            self._delete(self.root, value)
            self.level_fixup(self.root)

    def _delete(self, node, value):
        if node == NIL:
            print('Value {} is not in tree'.format(value))
            return
        elif node.value > value:
            self._delete(node.left, value)        
            return
        elif node.value < value:
            self._delete(node.right, value)
            return
        original_color = node.color
        if node.left == NIL:
            replace = node.right
            self.transplant(node, node.right)
        elif node.right == NIL:
            replace = node.left
            self.transplant(node, node.left)
        else:
            successor = self.min_tree(node.right)
            original_color = successor.color
            replace = successor.right
            if successor == node.right:
                replace.parent = successor
            else:
                self.transplant(successor, successor.right)
                successor.right = node.right
                successor.right.parent = successor
            self.transplant(node, successor)
            successor.left = node.left
            successor.left.parent = successor
            successor.color = node.color
        if original_color == BLACK:
            self.fixup_delete(replace)            

    def transplant(self, nodeout, nodein):
        if nodeout.parent == NIL:
            self.root = nodein
        elif nodeout.parent.left == nodeout:
            nodeout.parent.left = nodein
        else:
            nodeout.parent.right = nodein
        nodein.parent = nodeout.parent

    def fixup_insert(self, node):
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
                        self.left_rotate(node)
                    node.parent.color = BLACK
                    node.parent.parent.color = RED
                    self.right_rotate(node.parent.parent)
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
                        self.right_rotate(node)
                    node.parent.color = BLACK
                    node.parent.parent.color = RED
                    self.left_rotate(node.parent.parent)
        self.root.color = BLACK

    def fixup_delete(self, node):
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
                    sibling = node.parent.color
                    node.parent.color = BLACK
                    sibling.left.color = BLACK
                    self.right_rotate(node.parent)
                    node = self.root
        node.color = BLACK

    def level_fixup(self, node):
        if node == NIL:
            return
        if node == self.root:
            node.level = 1
        else:
            node.level = node.parent.level + 1
        self.level_fixup(node.left)
        self.level_fixup(node.right)

    def left_rotate(self, node):
        nodeup = node.right
        node.right = nodeup.left
        if node.right != NIL:
            node.right.parent = node
        if node.parent == NIL:
            self.root = nodeup
        elif node.parent.left == node:
            node.parent.left = nodeup
        else:
            node.parent.right = nodeup
        nodeup.parent = node.parent
        nodeup.left = node
        node.parent = nodeup

    def right_rotate(self, node):
        nodeup = node.left
        node.left = nodeup.right
        if node.left != NIL:
            node.left.parent = node
        if node.parent == NIL:
            self.root = nodeup
        elif node.parent.left == node:
            node.parent.left = nodeup
        else:
            node.parent.right = nodeup
        nodeup.parent = node.parent
        nodeup.right = node
        node.parent = nodeup

    def min_tree(self, node):
        if node.left == NIL:
            return node
        return self.min_tree(node.left)

    def inorder(self):
        if self.root is None:
            print('Empty tree')
        else:
            self._inorder(self.root)

    def _inorder(self, node):
        if node == NIL:
            return
        self._inorder(node.left)
        print('{}'.format(node.value))
        self._inorder(node.right)

    def preorder(self):
        if self.root is None:
            print('Empty tree')
        else:
            self._preorder(self.root)

    def _preorder(self, node):
        if node == NIL:
            return
        print('{} '.format(node.value))
        self._preorder(node.left)
        self._preorder(node.right)

    def get_height(self):
        self._height()
        return self.height

    def _height(self, node):
        if node is None:
            return
        if node.level > self.height:
            self.height = node.level
        self._height(node.left)
        self._height(node.right)


class Display(object):

    def __init__(self, coords=None):
        if coords is not None:
            self.tree_surface = display.set_mode(coords)
        else:
            self.tree_surface = display.set_mode()
        font.init()
        self.tree_font = font.Font(font.get_default_font(), 14)
        self.tree_surface.fill(Color('white'))
        self.size = self.tree_surface.get_size()[0]
        self.update()

    def display_tree(self, tree):
        self.node_size = 15
        self.tree_surface.fill(Color('white'))
        self.update()
        self.display_node(tree.root)
        self.update()

    def display_node(self, node):
        if node == NIL:
            return
        if node.parent == NIL:
            x = int(self.size / 2)
        elif node.parent.left == node:
            width = int(self.size / 2**node.level)
            x = node.parent.rect.center[0] - width
        else:
            width = int(self.size / 2**node.level)
            x = node.parent.rect.center[0] + width
        y = ((node.level - 1) * 50) + 20
        if node.color == RED:
            value_font = self.tree_font.render(str(node.value), 
                                               True, Color('black'))
            node.rect = draw.circle(self.tree_surface, Color('red'), 
                                    (x, y), self.node_size)
        else:
            value_font = self.tree_font.render(str(node.value), 
                                               True, Color('white'))
            node.rect = draw.circle(self.tree_surface, Color('black'), 
                                    (x, y), self.node_size)
        if node.parent != NIL:
            start = node.rect.midtop
            end = node.parent.rect.midbottom
            draw.line(self.tree_surface, Color('black'), start, end, 5)
        font_coords = (node.rect.center[0] - 5, node.rect.center[1] - 5)
        self.tree_surface.blit(value_font, font_coords)
        self.nil_node(node)
        self.display_node(node.left)
        self.display_node(node.right)
 
    def nil_node(self, node):
        y = (node.level * 50) + 20
        width = int(self.size / 2**(node.level + 1))
        centerx = node.rect.center[0] 
        font_value = self.tree_font.render('Nil', True, Color('white'))
        if node.left == NIL:
            x = centerx - width
            nil = draw.circle(self.tree_surface, Color('black'),
                              (x, y), self.node_size)
            start = nil.midtop 
            end = node.rect.midbottom
            draw.line(self.tree_surface, Color('black'), start, end, 5)
            font_coords = (x - 5, y - 5)
            self.tree_surface.blit(font_value, font_coords)
        if node.right == NIL:
            x = centerx + width
            nil = draw.circle(self.tree_surface, Color('black'),
                              (x, y), self.node_size)
            start = nil.midtop
            end = node.rect.midbottom
            draw.line(self.tree_surface, Color('black'), start, end, 5)
            font_coords = (x - 5, y - 5)
            self.tree_surface.blit(font_value, font_coords)

    def update(self):
        display.update()

    def exit(self):
        display.quit()


def unique_list(span, length):
    if span < length:
        raise ValueError
    value_set = set()
    while len(value_set) < length:
        value_set.add(random.randint(1, span))
    value_list = list(value_set)
    random.shuffle(value_list)
    return value_list

def unbalanced(length):
    return range(length)


if __name__ == "__main__":
    rbt = RBTree()
    tree_display = Display((1000, 800))
    values = [3, 9, 0, 1, 2, 5, 7, 6, 8, 4]
    for value in values:
        rbt.insert(value)
        tree_display.display_tree(rbt)
        sleep(20)
    for value in range(10):
        rbt.delete(value)
        tree_display.display_tree(rbt)
        sleep(20)
