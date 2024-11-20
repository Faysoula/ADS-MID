//
// Created by User on 11/19/2024.
//
#include "TreeNode.h"
#include <iostream>
#include <string>

using namespace std;

TreeNode::TreeNode() : account(NULL), leftChild(NULL), rightSibling(NULL) {}

TreeNode::TreeNode(const Account &acc) : leftChild(NULL), rightSibling(NULL) {
    account = new Account(acc);
}

TreeNode::TreeNode(const TreeNode &other) {
    copyForm(other);
}

TreeNode::~TreeNode() {
    delete account;
    clean();
}

//gets
Account TreeNode::getData() const {
    return *account;
}

NodePtr TreeNode::getLeftChild() const {
    return leftChild;
}

NodePtr TreeNode::getRightSibling() const {
    return rightSibling;
}

//sets
void TreeNode::setData(const Account &acc) {
    if (account != NULL) {
        delete account;
    }
    account = new Account(acc);
}

void TreeNode::setLeftChild(NodePtr left) {
    leftChild = left;
}

void TreeNode::setRightSibling(NodePtr right) {
    rightSibling = right;
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

void TreeNode::updateBalance(const Transaction &t) {
    if (account == nullptr) {
        throw std::runtime_error("Null account pointer ");
    }
    account->updateBalance(t);
}

bool TreeNode::checkParent(int parentNum, int childNum) const {
    string parent = to_string(parentNum);
    string child = to_string(childNum);

    if (child.length() <= parent.length()) {
        return false;
    }

    return child.substr(0, parent.length()) == parent;
}

bool TreeNode::isParent(int accNum) const {
    return checkParent(account->getAccountNumber(), accNum);
}

//HII FAYSAL AGAIN so
/*
       1000
      /    \
   1100   1200
   /  \      \
 1110 1120   1210

 this will turn to

1000            // Level 0
|
1100->1200       // Level 1
|      |
1110->1120-> 1210 // Level 2

 */


int TreeNode::getLevel(NodePtr root) const {
    if (root == NULL) {
        return -1;
    }
    if (root->account->getAccountNumber() == account->getAccountNumber()) {
        return 0;
    }

    getLevelHelper(root->leftChild, 1);

}

int TreeNode::getLevelHelper(NodePtr node, int currentLevel) const {
    if (node == NULL) {
        return -1;
    }
    if (node->account->getAccountNumber() == account->getAccountNumber()) {
        return currentLevel;
    }

    NodePtr sibling = node->rightSibling;
    while (sibling != nullptr) {
        if (sibling->account->getAccountNumber() == account->getAccountNumber()) {
            return currentLevel;
        }
        sibling = sibling->rightSibling;
    }

    NodePtr child = node->leftChild;
    while (child != nullptr) {
        int result = getLevelHelper(child, currentLevel + 1);
        if (result != -1) {
            return result;
        }
        child = child->rightSibling;
    }

    return -1;
}

void TreeNode::print() const {
    std::cout << "Account: " << account->getAccountNumber()
              << " - " << account->getDescription()
              << " (Balance: " << account->getBalance() << ")\n";
}

void TreeNode::copyForm(const TreeNode &other) {
    if (account != NULL) {
        delete account;
    }
    if (leftChild != NULL) {
        delete leftChild;
    }
    if (rightSibling != NULL) {
        delete rightSibling;
    }

    // Deep copy of account
    account = (other.account != NULL) ? new Account(*other.account) : NULL;

    // Deep copy of child nodes
    leftChild = (other.leftChild != nullptr) ? new TreeNode(*other.leftChild) : NULL;

    // Deep copy of sibling nodes
    rightSibling = (other.rightSibling != nullptr) ? new TreeNode(*other.rightSibling) : NULL;
}


void TreeNode::clean() {
    // Delete child subtree
    delete leftChild;
    leftChild = nullptr;

    // Delete sibling subtree
    delete rightSibling;
    rightSibling = nullptr;
}

