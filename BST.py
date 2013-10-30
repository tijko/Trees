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
    else:
        tree = leaf
    return tree              
 
def delete(tree, leaf):
    if not leaf.left:
       tree = transplant(tree, leaf, leaf.right)
    elif not leaf.right:
       tree = transplant(tree, leaf, leaf.left)
    else:
        _branch = tree_min(leaf.right)
        if not _branch.parent:
            tree = transplant(tree, _branch, _branch.right)
            _branch.right = leaf.right
            _branch.right.parent = _branch
        tree = transplant(tree, leaf, _branch)
        _branch.left = leaf.left
        _branch.left.parent = _branch
    return tree

def transplant(tree, leaf, branch):
    _tree = tree
    if not leaf.parent:
        _tree = branch
    elif leaf == leaf.parent.left:
        leaf.parent.left = branch
    else:
        leaf.parent.right = branch
    if branch:
        branch.parent = leaf.parent
    return tree

def search(tree, leaf):
    if not tree:
        print "Value <%d> not in tree!" % leaf.value
    elif tree.value == leaf.value:
        print "Value <%d> is in tree!" % leaf.value
        return
    elif tree.value < leaf.value:
        search(tree.right, leaf)
    else:   
        search(tree.left, leaf)
    
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
    return tree

def tree_min(tree):
    while tree.left:
        tree = tree.left
    return tree

def return_selection():
    leaf = None
    while True:
        try:
            option = int(raw_input('Enter number of option > '))
            if option == 5:
                return option, leaf
            elif option != 4:
                value = int(raw_input('Enter value > '))
                _leaf = Leaf(value)
                leaf = deepcopy(_leaf)
            break
        except ValueError:
            print "Enter a number"
    return option, leaf


if __name__ == '__main__':
    tree = None
    while True:
        print "Select option: [1=insert, 2=delete, 3=search, 4=tree_walk, 5=exit]"
        option, leaf = return_selection()
        if option == 1:
            tree = insert(tree, leaf)
        elif option == 2:
            tree = delete(tree, leaf)
        elif option == 3:
            search(tree, leaf)
        elif option == 4:    
            tree_walk(tree)
        else:
            break

