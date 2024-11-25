//
// Created by Faysal on 11/19/2024.
//

/**
 * @file TreeNode.cpp
 * @brief Implements the `TreeNode` class, which represents a single node in the forest tree structure.
 *
 * This file defines the `TreeNode` class, including methods for managing the data of a node, accessing child nodes,
 * and handling operations like adding or removing children. Each node in the tree contains an `Account` object
 * and pointers to its children, facilitating the hierarchical structure of the chart of accounts.
 */

#include "TreeNode.h"
#include <iostream>
#include <string>

using namespace std;
/**
 * @class TreeNode
 * @brief Represents a node in a hierarchical tree structure for managing accounts.
 *
 * Each TreeNode contains an Account object, pointers to its left child and right sibling,
 * and provides functionalities for managing its child nodes, sibling nodes, and balances.
 */

/**
 * @brief Default constructor.
 *
 * Initializes a TreeNode with null pointers for the account, left child, and right sibling.
 */
TreeNode::TreeNode() : account(NULL), leftChild(NULL), rightSibling(NULL) {}
/**
 * @brief Parameterized constructor.
 *
 * Initializes a TreeNode with the given account and null pointers for its children.
 *
 * @param acc The account to store in this TreeNode.
 */
TreeNode::TreeNode(const Account &acc) : leftChild(NULL), rightSibling(NULL) {
    account = new Account(acc);
}
/**
 * @brief Copy constructor.
 *
 * Creates a deep copy of another TreeNode, including its account and child nodes.
 *
 * @param other The TreeNode to copy from.
 */
TreeNode::TreeNode(const TreeNode &other) {
    copyForm(other);
}
/**
 * @brief Destructor.
 *
 * Cleans up dynamically allocated memory for the account, child, and sibling nodes.
 */
TreeNode::~TreeNode() {
    delete account;
    clean();
}
/**
 * @brief Gets the left child of this TreeNode.
 *
 * @return Pointer to the left child node.
 */
//gets
//Account TreeNode::getData() const {
//    return *account;
//}

NodePtr TreeNode::getLeftChild() const {
    return leftChild;
}
/**
 * @brief Gets the right sibling of this TreeNode.
 *
 * @return Pointer to the right sibling node.
 */
NodePtr TreeNode::getRightSibling() const {
    return rightSibling;
}
/**
 * @brief Sets the account data for this TreeNode.
 *
 * Deletes the existing account (if any) and sets the new account.
 *
 * @param acc The account to associate with this TreeNode.
 */
//sets
void TreeNode::setData(const Account &acc) {
    if (account != NULL) {
        delete account;
    }
    account = new Account(acc);
}
/**
 * @brief Sets the left child for this TreeNode.
 *
 * @param left Pointer to the node to set as the left child.
 */
void TreeNode::setLeftChild(NodePtr left) {
    leftChild = left;
}
/**
 * @brief Sets the right sibling for this TreeNode.
 *
 * @param right Pointer to the node to set as the right sibling.
 */
void TreeNode::setRightSibling(NodePtr right) {
    rightSibling = right;
}
/**
 * @brief Assignment operator.
 *
 * Performs a deep copy of another TreeNode, cleaning up the current data beforehand.
 *
 * @param other The TreeNode to assign from.
 * @return Reference to the updated TreeNode.
 */
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
/**
 * @brief Checks if this TreeNode is a leaf node (has no children).
 *
 * @return True if the node has no children, false otherwise.
 */
bool TreeNode::isLeaf() const {
    return leftChild == NULL;
}
/**
 * @brief Checks if this TreeNode has a sibling.
 *
 * @return True if the node has a right sibling, false otherwise.
 */
bool TreeNode::hasSibling() const {
    return rightSibling != NULL;
}
/**
 * @brief Finds a node with a specific account number in the tree.
 *
 * Performs a depth-first search for the node with the specified account number.
 *
 * @param root Pointer to the root of the tree to search.
 * @param accNum The account number to search for.
 * @return Pointer to the found node, or nullptr if not found.
 */
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

/**
 * @brief Adds a child node with the specified account to this TreeNode.
 *
 * Maintains sibling order based on account numbers.
 *
 * @param acc The account to associate with the new child node.
 */
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
/**
 * @brief Adds a sibling node with the specified account to this TreeNode.
 *
 * Maintains sibling order based on account numbers.
 *
 * @param acc The account to associate with the new sibling node.
 */
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
/**
 * @brief Adds a new account node to the tree.
 *
 * This method adds a new account node to the tree while maintaining the parent-child
 * and sibling relationships. If the account already exists, it does nothing.
 *
 * @param root Pointer to the root node of the tree.
 * @param newAcc The account to be added to the tree.
 * @return True if the account was successfully added, false otherwise.
 */
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
/**
 * @brief Updates the balances for the current account and its parent nodes.
 *
 * Applies a transaction to the current account and propagates the balance changes
 * to all parent accounts in the tree, starting from the current account and moving upward.
 *
 * @param root Pointer to the root node of the tree.
 * @param t The transaction to apply to the current account.
 * @throws runtime_error If the current account is null.
 */
void TreeNode::updateBalance(NodePtr root, Transaction &t) {
    if (!account) {
        throw runtime_error("Null account pointer");
    }

    // Get parent nodes from the main root of this account's tree
    vector<NodePtr> parents = getParentNodes(root);

    // Update the current account's balance first
    account->updateBalance(t);


    // Update all parent balances from bottom to top
    for (NodePtr parent: parents) {
        if (parent && parent->account) {
            parent->account->updateBalance(t);
        }
    }
}
/**
 * @brief Retrieves all parent nodes of the current account in the tree.
 *
 * This method calculates and returns a list of parent nodes for the current account,
 * working its way up the tree hierarchy.
 *
 * @param root Pointer to the root node of the tree.
 * @return A vector of pointers to the parent nodes.
 */
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
/**
 * @brief Updates the balances of all parent accounts based on a transaction.
 *
 * This method takes a list of parent nodes and updates their balances to reflect
 * the changes introduced by the specified transaction.
 *
 * @param parents A vector of pointers to the parent nodes.
 * @param t The transaction to apply to the parent accounts.
 */
void TreeNode::updateParentBalances(const vector<NodePtr> &parents, const Transaction &t) {
    for (NodePtr parent: parents) {
        parent->account->updateBalance(t);
    }
}
/**
 * @brief Validates the parent-child relationship between two accounts.
 *
 * Checks whether a child account number is a valid extension of a parent account number,
 * ensuring that the child number starts with the parent's number and is exactly one digit longer.
 *
 * @param parentNum The account number of the parent.
 * @param childNum The account number of the child.
 * @return True if the child is valid, false otherwise.
 */
bool TreeNode::isValidChild(int parentNum, int childNum) const {
    string parent = to_string(parentNum);
    string child = to_string(childNum);

    if (child.length() <= parent.length()) {
        return false;
    }

    return child.substr(0, parent.length()) == parent;
}
/**
 * @brief Gets the level of the current account in the tree.
 *
 * This method calculates the depth (or level) of the current account node in the tree
 * relative to the root node.
 *
 * @param root Pointer to the root node of the tree.
 * @return The level of the current account node, or -1 if the account is not found.
 */
int TreeNode::getLevel(NodePtr root) const {
    if (root == NULL) {
        return -1;
    }
    if (root->account->getAccountNumber() == account->getAccountNumber()) {
        return 0;
    }

    return getLevelHelper(root->leftChild, 1);
}
/**
 * @brief Helper method for calculating the level of the current node.
 *
 * This recursive method determines the depth of a specific node relative to
 * the root node by traversing the tree structure.
 *
 * @param node Pointer to the node currently being inspected.
 * @param currentLevel The current level being tracked in the recursive calls.
 * @return The level of the current node, or -1 if the node is not found.
 */
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
/**
 * @brief Prints the details of the current account.
 *
 * Displays the account number, description, and balance for the account stored
 * in this TreeNode.
 */
void TreeNode::print() const {
    std::cout << "Account: " << account->getAccountNumber()
              << " - " << account->getDescription()
              << " (Balance: " << account->getBalance() << ")\n";
}
/**
 * @brief Copies data from another TreeNode into this one.
 *
 * Performs a deep copy of the account, left child, and right sibling of another
 * TreeNode into the current TreeNode.
 *
 * @param other The TreeNode to copy from.
 */
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

/**
 * @brief Cleans up the child and sibling nodes of this TreeNode.
 *
 * Deletes the left child and right sibling subtrees of this TreeNode, freeing
 * dynamically allocated memory.
 */
void TreeNode::clean() {
    // Delete child subtree
    delete leftChild;
    leftChild = nullptr;

    // Delete sibling subtree
    delete rightSibling;
    rightSibling = nullptr;
}

