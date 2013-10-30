#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" 
An implementation of a complete binary search tree. The 'complete' part of the 
this tree comes where each node will always have two children, where one will 
either be a value and the other None or both will be None.
"""

from copy import deepcopy


class Leaf(object):

    def __init__(self, value):
        self.value = value
        self.parent = None
        self.left = None
        self.right = None

def insert(tree, leaf):
    parent = None
    _tree = tree
    while _tree:
        parent = _tree
        if leaf.value < _tree.value:
            _tree = _tree.left
        else:
            _tree = _tree.right
    leaf.parent = parent
    if parent and leaf.value < parent.value:
        parent.left = leaf
    elif parent and leaf.value >= parent.value:
        parent.right = leaf
    return tree              
 
def delete(tree):
    pass

def search(tree, value):
    if tree.value == value:
        print "Value <%d> is in tree!" % value
        return
    if not tree:
        print "Value <%d> not in tree!" % value
    elif tree.value < value:
        search(tree.right, value)
    else:   
        search(tree.left, value)
    
def tree_walk(tree):
    if tree:
        tree_walk(tree.left)
        print tree.value
        tree_walk(tree.right)

def tree_predecessor(tree):
    if tree.left:
        return tree_max(tree.left)
    parent = tree.parent
    while parent and tree == parent.left:
        tree = parent
        parent = parent.parent
    return parent.value

def tree_successor(tree):
    if tree.right:
        return tree_min(tree.right)
    parent = tree.parent
    while parent and tree == parent.right:
        tree = parent
        parent = parent.parent
    return parent.value

def tree_max(tree):
    while tree.right:
        tree = tree.right
    return tree.value

def tree_min(tree):
    while tree.left:
        tree = tree.left
    return tree.value

def return_selection():
    while True:
        try:
            option = int(raw_input('> '))
            return option 
        except ValueError:
            print "Enter a number"



if __name__ == '__main__':
    options = {1:insert, 2:delete, 3:search, 4:tree_walk}
    tree = None
    while True:
        print "Select option: [1=insert, 2=delete, 3=search, 4=tree_walk, 5=exit]"
        option = return_selection()
        tree_action = options.get(option)
        if tree_action:
            if option == 1:
                value = return_selection() 
                _leaf = Leaf(value)
                leaf = deepcopy(_leaf)
                tree = tree_action(tree, leaf)
            elif option == 3:
                value = return_selection()
                tree_action(tree, value)
            else:    
                tree_action(tree)
        else:
            break

