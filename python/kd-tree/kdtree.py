#!/usr/bin/env python
# -*- coding: utf-8 -*-

from pygame import *
from math import sqrt, inf
from random import randint

from time import sleep


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
MAX_X = (101, 699)
MAX_Y = (1, 649)

class KdTree(object):

    def __init__(self):
        self.root = None
        self.close = inf
        self.close_coord = None
        self.dist_to_point = (lambda x1, y1, x2, y2: 
                              sqrt((x1 - x2)**2 + (y1 - y2)**2))

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

    def construct_tree_from_list(self, nodes):
        for node in nodes:
            self.insert(node)

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

    def find_closest(self, target):
        self.find_neighbor(self.root, target)
        draw_point(self.close_coord, p_color='orange')

    def in_tree(self, value):
        return self._in_tree(self.root, value)

    def _in_tree(self, node, value):
        if node is None: return False
        if node.value == value: return True
        if node.value[node.d] < value[node.d]:
            return in_tree(node.left, value)
        else:
            return in_tree(node.right, value)
                 
    def find_neighbor(self, node, target):
        if node is None: return
        if self.close < self.dist_to_rect(node, target):
            return
        node_dist = self.dist_to_point(*node.value, *target)
        if node_dist < self.close:
            self.close = node_dist
            self.close_coord = node.value
        if node == self.root:
            self.find_neighbor(node.left, target)
            self.find_neighbor(node.right, target)
        else:
            if target[node.d] < node.value[node.d]:
                self.find_neighbor(node.left, target)
                self.find_neighbor(node.right, target)
            else:
                self.find_neighbor(node.right, target)
                self.find_neighbor(node.left, target)

    def dist_to_rect(self, node, target):
        x = y = 0
        if target[0] < node.min_x:
            x = target[0] - node.min_x
        elif target[0] > node.max_x:
            x = target[0] - node.max_x
        if target[1] < node.min_y:
            y = target[1] - node.min_y
        elif target[1] > node.max_y:
            y = target[1] - node.max_y 
        return sqrt(x * x + y * y)

    @property
    def print_kdtree(self):
        self._print(self.root)

    def _print(self, node):
        if node is not None:
            print(node.value, node.d, node.level)
            self._print(node.left)
            self._print(node.right)


def load_points(fname):
    with open(fname) as f:
        treat_data = f.readlines()
        points = [tuple(map(float, coords.split())) for 
                            coords in treat_data[1:]]
    points.insert(0, int(treat_data[0]))
    return points
        
def display_setup():
    display.init()
    return display.set_mode((700, 750))

def draw_grid_axis(coords):
    x = draw.line(screen, Color('black'), (0, 650), (700, 650), 5)
    y = draw.line(screen, Color('black'), (100, 0), (100, 750),  5)
    display.update()

def draw_point(coords, p_color='violet'):
    draw.circle(screen, Color(p_color), coords, 5)
    display.update()

def create_random_points(n):
    return [(randint(*MAX_X), randint(*MAX_Y)) for _ in range(n)]

def display_points(node):
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
    display_points(node.left)
    display_points(node.right)

if __name__ == "__main__":
    RUNNING = True
    screen = display_setup()
    screen.fill(Color('white'))
    display.update()
    draw_grid_axis((40, 40))
    tree = KdTree()
    points = create_random_points(100)
    tree.construct_tree_from_list(points)
    tree.print_kdtree
        
    display_points(tree.root)
    target = create_random_points(1)[0]
    draw_point(target)
    tree.find_closest(target)
    while RUNNING:
        for ev in event.get():
            if ev.type == KEYDOWN and ev.key == K_q:
                RUNNING = False
        display.update()
        sleep(1)
    quit()
