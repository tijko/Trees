#!/usr/bin/env python
# -*- coding: utf-8 -*-

import random

from pygame import *
from time import sleep


class Node(object):

    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None
        self.parent = None
        self.rect = None
        self.level = 0
        self.position = 0

    def coords(self):
        return (self.rect.center[0] - 10, self.rect.center[1] - 10)
 

class BST(object):

    def __init__(self):
        self.height = 0
        self.root = None

    def insert(self, value):
        self._insert(self.root, value)

    def _insert(self, node, value):
        if self.root is None:
            self.root = Node(value)
            self.root.level = 1
            self.root.position = 1
        elif node.value > value:
            if node.left is not None:
                self._insert(node.left, value)
            else:
                node.left = Node(value)
                node.left.parent = node
                node.left.level = node.level + 1
                node.left.position = (node.position * 2)
        elif node.value <= value:
            if node.right is not None:
                self._insert(node.right, value)
            else:
                node.right = Node(value)
                node.right.parent = node
                node.right.level = node.level + 1
                node.right.position = (node.position * 2) + 1

    def transplant(self, node_out, node_in):
        if self.root == node_out:
            self.root = node_in
        elif node_out.parent.left == node_out:
            node_out.parent.left = node_in
        else:
            node_out.parent.right = node_in
        if node_in is not None:
            node_in.parent = node_out.parent

    def delete(self, value):
        self._delete(self.root, value)

    def _delete(self, node, value):
        if node is None:
            print('Value: {} not in tree'.format(value))
        elif node.value > value:
            self._delete(node.left, value)
        elif node.value < value:
            self._delete(node.right, value)
        else:
            if node.left is None:
                self.transplant(node, node.right)
            elif node.right is None:
                self.transplant(node, node.left)
            else:
                node_in = min_tree(node.right)
                if node_in != node.right:
                    transplant(node_in, node_in.right)
                    node_in.right = node.right
                    node_in.right.parent = node_in
                self.transplant(node, node_in)
                node_in.left = node.left
                node_in.left.parent = node_in

    def min_tree(self, node):
        if node.left is None:
            return node
        return self.min_tree(node.left)

    def postorder(self):
        self._postorder(self.root)

    def _postorder(self, node):
        if node is not None:
            print(node.value)
            self._postorder(node.left)
            self._postorder(node.right)

    def inorder(self):
        self._inorder(self.root)

    def _inorder(self, node):
        if node is not None:
            self._inorder(node.left)
            print(node.value)
            self._inorder(node.right)

    def get_height(self):
        self._height(self.root)
        return self.height

    def _height(self, node):
        if node is not None:
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
        self.tree_surface.fill(Color('white'))
        self.tree_font = font.Font(font.get_default_font(), 14)
        self.size = self.tree_surface.get_size()[0]
        self.update()

    def draw_tree(self, tree):
        self.node_size = 15
        self.display_node(tree.root)
        self.update()

    def display_node(self, node):
        if node is None:
            return
        value_font = self.tree_font.render(str(node.value), True, Color('white'))
        if node.parent is None:
            x = int(self.size / 2)
        elif node.parent.left == node:
            width = int(self.size / 2**node.level)
            x = node.parent.rect.center[0] - width
        else:
            width = int(self.size / 2**node.level)
            x = node.parent.rect.center[0] + width
        y = ((node.level - 1) * 50) + 20
        node.rect = draw.circle(self.tree_surface, Color('black'), 
                                (x, y), self.node_size)
        if node.parent is not None:
            start = node.rect.midtop
            end = node.parent.rect.midbottom
            draw.line(self.tree_surface, Color('black'), start, end,  5)
        font_coords = (x - 5, y - 5)
        self.tree_surface.blit(value_font, font_coords)
        self.display_node(node.left)
        self.display_node(node.right)

    def update(self):
        display.update()

    def exit(self):
        display.quit()


def unique_values(span, length):
    if span < length:
        raise ValueError
    value_set = set()
    while len(value_set) < length:
        value_set.add(random.randint(1, span))
    value_list = list(value_set)
    random.shuffle(value_list)
    return value_list 

def unbalanced(length):
    return range(1, length)

if __name__ == "__main__":
    tree_display = Display((1000, 800))
    bst = BST()
    for value in unique_values(300, 200):
        bst.insert(value)
    tree_display.draw_tree(bst)
    while 1:
        tree_display.update()
    #sleep(30)
