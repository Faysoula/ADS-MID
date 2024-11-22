//
// Created by Faysal on 11/19/2024.
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
//Account TreeNode::getData() const {
//    return *account;
//}

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

NodePtr TreeNode::findNode(NodePtr root, int accNum) {
    if (!root) {
        return nullptr;
    }

    // Check current node
    if (root->account && root->account->getAccountNumber() == accNum) {
        return root;
    }

    // Check children first (depth-first search)
    NodePtr found = nullptr;
    if (root->leftChild) {
        found = findNode(root->leftChild, accNum);
        if (found) return found;
    }

    // Then check siblings
    if (root->rightSibling) {
        found = findNode(root->rightSibling, accNum);
        if (found) return found;
    }

    return nullptr;
}


void TreeNode::addChild(const Account &acc) {
    NodePtr newChild = new TreeNode(acc);

    if (leftChild == NULL) {
        leftChild = newChild;
        return;
    }

    // Find proper position among siblings
    if (leftChild->account->getAccountNumber() > acc.getAccountNumber()) {
        // Insert at beginning
        newChild->rightSibling = leftChild;
        leftChild = newChild;
        return;
    }

    // Find insertion point
    NodePtr current = leftChild;
    while (current->rightSibling &&
           current->rightSibling->account->getAccountNumber() < acc.getAccountNumber()) {
        current = current->rightSibling;
    }

    // Insert after current
    newChild->rightSibling = current->rightSibling;
    current->rightSibling = newChild;
}

void TreeNode::addSibling(const Account &acc) {
    NodePtr newSibling = new TreeNode(acc);

    if (rightSibling == NULL) {
        rightSibling = newSibling;
        return;
    }

    // Find proper position
    if (rightSibling->account->getAccountNumber() > acc.getAccountNumber()) {
        // Insert at beginning
        newSibling->rightSibling = rightSibling;
        rightSibling = newSibling;
        return;
    }

    // Find insertion point
    NodePtr current = rightSibling;
    while (current->rightSibling &&
           current->rightSibling->account->getAccountNumber() < acc.getAccountNumber()) {
        current = current->rightSibling;
    }

    // Insert after current
    newSibling->rightSibling = current->rightSibling;
    current->rightSibling = newSibling;
}

bool TreeNode::addAccountNode(NodePtr root, const Account &newAcc) {
    int newAccNum = newAcc.getAccountNumber();

    // Check if account already exists
    if (findNode(root, newAccNum) != nullptr) {
        return false;  // Account number already exists
    }

    // If this is the first node
    if (!account) {
        account = new Account(newAcc);
        return true;
    }

    // Find the proper parent for this account
    string newAccStr = to_string(newAccNum);
    string parentStr = newAccStr.substr(0, newAccStr.length() - 1);

    if (parentStr.empty()) {
        return false;  // Let ForestTree handle root nodes
    }

    // Validate that child is only one digit longer than parent
    int parentNum = stoi(parentStr);
    if (to_string(parentNum).length() + 1 != newAccStr.length()) {
        return false;  // Invalid child-parent relationship
    }

    NodePtr parentNode = findNode(root, parentNum);
    if (!parentNode) {
        return false;  // Parent doesn't exist
    }

    try {
        // If parent has no children, add as first child
        if (!parentNode->leftChild) {
            parentNode->addChild(newAcc);
        } else {
            // Find the right sibling position among existing children
            NodePtr lastChild = parentNode->leftChild;

            // If new account should be first child
            if (lastChild->account->getAccountNumber() > newAccNum) {
                parentNode->addChild(newAcc);
                return true;
            }

            // Find the appropriate sibling to add after
            while (lastChild->rightSibling &&
                   lastChild->rightSibling->account->getAccountNumber() < newAccNum) {
                lastChild = lastChild->rightSibling;
            }
            lastChild->addSibling(newAcc);
        }
        return true;
    } catch (const invalid_argument &e) {
        return false;
    }
}

void TreeNode::updateBalance(NodePtr root, Transaction &t) {
    if (!account) {
        throw runtime_error("Null account pointer");
    }

    // Get parent nodes from the main root of this account's tree
    vector<NodePtr> parents = getParentNodes(root);

    // Update the current account's balance first
    if (t.getDebitCredit() == 'D') {
        account->setBalance(account->getBalance() + t.getAmount());
    } else if (t.getDebitCredit() == 'C') {
        account->setBalance(account->getBalance() - t.getAmount());
    }

    // Update all parent balances from bottom to top
    for (NodePtr parent: parents) {
        if (parent && parent->account) {
            if (t.getDebitCredit() == 'D') {
                parent->account->setBalance(parent->account->getBalance() + t.getAmount());
            } else if (t.getDebitCredit() == 'C') {
                parent->account->setBalance(parent->account->getBalance() - t.getAmount());
            }
        }
    }
}

vector<NodePtr> TreeNode::getParentNodes(NodePtr root) {
    vector<NodePtr> parents;
    string childNum = to_string(account->getAccountNumber());

    while (childNum.length() > 1) {
        // Remove last digit to get parent number
        childNum = childNum.substr(0, childNum.length() - 1);
        int parentNum = stoi(childNum);

        // First try to find in current root's tree
        NodePtr parent = findNode(root, parentNum);
        if (parent) {
            parents.insert(parents.begin(), parent); // Insert at beginning to maintain order
        }
    }

    return parents;
}

void TreeNode::updateParentBalances(const vector<NodePtr> &parents, const Transaction &t) {
    for (NodePtr parent: parents) {
        parent->account->updateBalance(t);
    }
}

bool TreeNode::isValidChild(int parentNum, int childNum) const {
    string parent = to_string(parentNum);
    string child = to_string(childNum);

    if (child.length() <= parent.length()) {
        return false;
    }

    return child.substr(0, parent.length()) == parent;
}

int TreeNode::getLevel(NodePtr root) const {
    if (root == NULL) {
        return -1;
    }
    if (root->account->getAccountNumber() == account->getAccountNumber()) {
        return 0;
    }

    return getLevelHelper(root->leftChild, 1);
}

//when your adding a trans ez il balance bas kif
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
    leftChild = (other.leftChild != NULL) ? new TreeNode(*other.leftChild) : NULL;

    // Deep copy of sibling nodes
    rightSibling = (other.rightSibling != NULL) ? new TreeNode(*other.rightSibling) : NULL;
}


void TreeNode::clean() {
    // Delete child subtree
    delete leftChild;
    leftChild = nullptr;

    // Delete sibling subtree
    delete rightSibling;
    rightSibling = nullptr;
}

