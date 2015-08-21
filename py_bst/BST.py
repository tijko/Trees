#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" 
An implementation of a complete binary search tree. The 'complete' part of the 
this tree comes where each node will always have two children, where one will 
either be a value and the other None or both will be None.
"""


class Leaf(object):

    def __init__(self, value):
        self.value = value
        self.parent = None
        self.left = None
        self.right = None

class Tree(object):

    def __init__(self):
        self.root = None


def get_value():
    while True:
        try:
            value = int(raw_input('Enter value > '))
            return value
        except ValueError:
            print "Enter a number"

def create_leaf():
    value = get_value()
    return Leaf(value)

def insert(tree, leaf):
    while tree:
        if leaf.value < tree.value:
            if not tree.left:
                tree.left = leaf
                leaf.parent = tree
                return
            tree = tree.left
        else:
            if not tree.right:
                tree.right = leaf
                leaf.parent = tree
                return
            tree = tree.right
 
def delete(tree, leaf, value):
    leaf = match_leaf(leaf, value)
    if not leaf:
        print "Value <%d> is not in tree!" % value
        return
    if not leaf.left:
        transplant(tree, leaf, leaf.right)
    elif not leaf.right:
        transplant(tree, leaf, leaf.left)
    else:
        _branch = tree_min(leaf.right)
        if _branch.parent != leaf:  #check if branch is child of deleted leaf
            transplant(tree, _branch, _branch.right) #trans branch.rg ;) 
            _branch.right = leaf.right 
            _branch.right.parent = _branch
        transplant(tree, leaf, _branch)
        _branch.left = leaf.left
        _branch.left.parent = _branch

def transplant(tree, leaf, branch):
    if not leaf.parent:
        tree.root = branch
    elif leaf == leaf.parent.left:
        leaf.parent.left = branch
    else:
        leaf.parent.right = branch
    if branch:
        branch.parent = leaf.parent

def match_leaf(branch, value):
    if not branch:
        return branch
    if branch.value > value:
        return match_leaf(branch.left, value)
    elif branch.value < value:
        return match_leaf(branch.right, value)
    return branch
 
def search(tree, value):
    if not tree:
        print "Value <%d> not in tree!" % value
        return
    elif tree.value == value:
        print "Value <%d> is in tree!" % value
        return
    while tree:
        if tree.value == value:
            print "Value <%d> is in tree!" % value
            return
        elif tree.value < value:
            tree = tree.right
        else:
            tree = tree.left
    print "Value <%d> is not in tree!" % value
    return    
    
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


if __name__ == '__main__':
    tree = Tree() 
    while True:
        print "Select option: [1=insert, 2=delete, 3=search, 4=tree_walk, 5=exit]"
        option = raw_input('Enter number of option > ')
        if option == '1':
            if not tree.root:
                tree.root = create_leaf()
            else:
                leaf = create_leaf()
                insert(tree.root, leaf)     
        elif option == '2':
            value = get_value()
            delete(tree, tree.root, value) 
        elif option == '3':
            value = get_value()
            search(tree.root, value)
        elif option == '4':
            tree_walk(tree.root) 
        else:
            break
