#!/usr/bin/env python
# -*- coding: utf-8 -*-

from pygame import *
from math import sqrt, inf
from random import randint

from time import sleep #debug


class Node(object):

    def __init__(self, value):
        self.value = value
        self.d = 0
        self.level = 1
        self.left = None
        self.right = None
        self.parent = None
        # set max/min levels on insertion
        self.max_x = self.max_y = 0
        self.min_x = self.min_y = 0


DIMENSION = 0x1

class KdTree(object):

    def __init__(self):
        self.root = None
        self.close = inf
        self.close_coord = None

    def insert(self, value):
        self._insert(self.root, value)

    def _insert(self, node, value):
        if node is None:
            self.root = Node(value)
            self.root.max_y = 650
            self.root.min_y = 0
            self.root.max_x = 700
            self.root.min_x = 100
        else:
            d = node.d
            if node.value[d] <= value[d]:
                if node.right is None:
                    self.create_node(node, 'right', value)
                else:
                    self._insert(node.right, value)
            else:
                if node.left is None:
                    self.create_node(node, 'left', value)
                else:
                    self._insert(node.left, value)

    def create_node(self, node, branch, value):
        new_node = Node(value)
        new_node.d = node.d ^ DIMENSION
        new_node.parent = node
        new_node.level = node.level + 1
        if branch == 'left':
            if node.d == 0:
                new_node.max_x = node.value[0] 
                new_node.max_y = node.max_y
            else:
                new_node.max_x = node.max_x
                new_node.max_y = node.value[1]
            new_node.min_x = node.min_x
            new_node.min_y = node.min_y
        else:
            new_node.max_x = node.max_x
            new_node.max_y = node.max_y
            if node.d == 0:
                new_node.min_x = node.value[0]
                new_node.min_y = node.min_y
            else:
                new_node.min_x = node.min_x
                new_node.min_y = node.value[1]
        setattr(node, branch, new_node)
             
dist = lambda x1, y1, x2, y2: sqrt((x1 - x2)**2 + (y1 - y2)**2)

def load_treats(fname):
    with open(fname) as f:
        treat_data = f.readlines()
        treats = [tuple(map(float, coords.split())) for coords in treat_data[1:]]
    treats.insert(0, int(treat_data[0]))
    return treats

def print_kdtree(node):
    if node is not None:
        print(node.value, node.d, node.level)
        print_kdtree(node.left)
        print_kdtree(node.right)
        
def setup():
    display.init()
    return display.set_mode((700, 750))

def draw_axis(screen, coords):
    x = draw.line(screen, Color('black'), (0, 650), (700, 650), 5)
    y = draw.line(screen, Color('black'), (100, 0), (100, 750),  5)
    display.update()

def draw_point(screen, coords):
    draw.circle(screen, Color('violet'), coords, 5)
    display.update()

def create_points(n):
    x = (101, 699)
    y = (1, 649)
    return [(randint(*x), randint(*y)) for _ in range(n)]

def find_neighbor(tree, node, target):
    if node is None:
        return
    node_dist = dist(*node.value, *target)
    if node_dist < tree.close:
        tree.close = node_dist
        tree.close_coord = node.value
    if node == tree.root:
        find_neighbor(tree, node.left, target)
        find_neighbor(tree, node.right, target)
    else:
        if target[node.d] < node.value[node.d]:
            find_neighbor(tree, node.left, target)
        elif target[node.d] > node.value[node.d]:
            find_neighbor(tree, node.right, target)
        else:
            find_neighbor(tree, node.left, target)
            find_neighbor(tree, node.right, target)

def in_tree(node, value):
    if node is None: return False
    if node.value == value: return True
    if node.value[node.d] < value[node.d]:
        return in_tree(node.left, value)
    else:
        return in_tree(node.right, value)

def find_closest(screen, tree, target):
    find_neighbor(tree, tree.root, target)
    draw.circle(screen, Color('orange'), tree.close_coord, 5)
    display.update()

def draw_tree(screen, node):
    if node is None:
        return
    parent = node.parent
    if parent is None:
        boundary = (0, 0)
        draw.circle(screen, Color('red'), node.value, 5)
        # vertical line
        draw.line(screen, Color('red'), (node.value[0], 0), (node.value[0], 650), 3)
    else:
        boundary = parent.value
        d = node.d
        if d == 0:
            # top to bottom
            if node == parent.left:
                # parent is max 
                start = (node.value[0], node.min_y) 
                end = (node.value[0], boundary[1])
            else:
                start = (node.value[0], node.max_y)
                end = (node.value[0], boundary[1])
                # parent is min
            draw.circle(screen, Color('red'), node.value, 5)
            draw.line(screen, Color('red'), start, end, 3)
        else:
            # left to right
            if node == parent.left:
                # parent is max
                start = (node.min_x, node.value[1])
                end = (boundary[0], node.value[1])
            else:
                start = (boundary[0], node.value[1])
                end = (node.max_x, node.value[1])
                # parent is min
            draw.circle(screen, Color('blue'), node.value, 5)
            draw.line(screen, Color('blue'), start, end, 3)
    display.update()
    print('Coords: {} Max: {} Min: {}'.format(node.value, 
         (node.max_x, node.max_y), (node.min_x, node.min_y)))
    draw_tree(screen, node.left)
    draw_tree(screen, node.right)

def build_tree(treats):
    tree = KdTree()
    for treat in treats:
        tree.insert(treat)
    return tree
        
if __name__ == "__main__":
    RUNNING = True
    screen = setup()
    screen.fill(Color('white'))
    display.update()
    draw_axis(screen, (40, 40))
    tree = KdTree()

    treats = create_points(100)
    tree = build_tree(treats)
    
    draw_tree(screen, tree.root)
    target = create_points(1)[0]
    draw_point(screen, target)
    find_closest(screen, tree, target)
    while RUNNING:
        for ev in event.get():
            if ev.type == KEYDOWN and ev.key == K_q:
                RUNNING = False
        display.update()
        sleep(1)
    quit()
