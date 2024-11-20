//
// Created by User on 11/19/2024.
//
#include "TreeNode.h"
#include <iostream>
#include <string>

using namespace std;

TreeNode::TreeNode() : leftChild(NULL), rightSibling(NULL) {}

TreeNode::TreeNode(const Account &acc) : leftChild(NULL), rightSibling(NULL) {
    account = new Account(acc);
}

TreeNode::TreeNode(const TreeNode &other) {
    copyForm(other);
}

TreeNode::~TreeNode() {
    clean();
}

TreeNode &TreeNode::operator=(const TreeNode &other) {
    if (this != &other) {
        clean();
        copyForm(other);
    }
    return *this;
}

//hello im faysal i shall now explain these to you
/*
 *
 *   A
    /|\
   B C D
   A's leftChild points to B,
   B's rightSibling points to C,
   C's rightSibling points to D

   so it looks like this in a tree,

    A
    |
    B -> C -> D

   */
//then we need this
bool TreeNode::isLeaf() const {
    return leftChild == NULL;
}

bool TreeNode::hasSibling() const {
    return rightSibling != NULL;
}

void TreeNode::addchild(const Account &acc) {
    NodePtr child = new TreeNode(acc);

    if (leftChild == NULL) {
        leftChild = child;
    } else {
        NodePtr temp = leftChild;
        while (temp->rightSibling != NULL) {
            temp = temp->rightSibling;
        }
        temp->rightSibling = child;
    }
}

void TreeNode::addSibling(const Account &acc) {
    NodePtr sibling = new TreeNode(acc);

    if (rightSibling == NULL) {
        rightSibling = sibling;
    } else {
        NodePtr temp = rightSibling;
        while (temp->rightSibling != NULL) {
            temp = temp->rightSibling;
        }
        temp->rightSibling = sibling;
    }
}

void TreeNode::updateBalance(double ammount) {
    account->updateBalance(ammount);// maya this is the function you need to implement first
}

bool TreeNode::isParent(int accNum) const {
    return false;
}




