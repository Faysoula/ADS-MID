//
// Created by asus on 11/19/2024.
//

#include "ForestTree.h"
#include <fstream>
#include <sstream>

// Constructor
ForestTree::ForestTree() {}

// Destructor
ForestTree::~ForestTree() {
    for (NodePtr root : rootAccounts) {
        delete root; // Delete root nodes and their subtrees
    }
    rootAccounts.clear();
}

// Function to initialize an empty forest tree
void ForestTree::initialize() {
    for (NodePtr root : rootAccounts) {
        delete root; // Clean up dynamically allocated nodes
    }
    rootAccounts.clear();
    cout << "Forest tree initialized successfully." << endl;
}

// Function to build a chart of accounts from a file
void ForestTree::buildFromFile(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        istringstream stream(line);
        int accountNumber, parentNumber;
        string description;
        stream >> accountNumber >> parentNumber;
        getline(stream, description);

        // Create a new account object
        Account newAccount(accountNumber, description, 0.0);

        if (parentNumber == -1) {
            // Create a root node
            NodePtr newNode = new TreeNode(newAccount);
            rootAccounts.push_back(newNode);
        } else {
            // Find the parent node recursively
            NodePtr parentNode = nullptr;
            for (NodePtr root : rootAccounts) {
                parentNode = findNodeByAccountNumber(root, parentNumber);
                if (parentNode != nullptr) {
                    break;
                }
            }

            if (parentNode) {
                parentNode->addchild(newAccount); // Add as a child to the parent node
            } else {
                cerr << "Parent account " << parentNumber << " not found. Cannot add subaccount " << accountNumber << "." << endl;
            }
        }
    }

    file.close();
    cout << "Chart of accounts built from file successfully." << endl;
}

// Recursive function to find a node by account number
NodePtr ForestTree::findNodeByAccountNumber(NodePtr node, int accountNumber) const {
    if (!node) {
        return nullptr;
    }

    if (node->getData().getAccountNumber() == accountNumber) {
        return node;
    }

    // Recur for the left child
    NodePtr foundNode = findNodeByAccountNumber(node->getLeftChild(), accountNumber);
    if (foundNode) {
        return foundNode;
    }

    // Recur for the right sibling
    return findNodeByAccountNumber(node->getRightSibling(), accountNumber);
}

// Utility function to print the entire forest tree
void ForestTree::printForestTree() const {
    for (NodePtr root : rootAccounts) {
        printTreeHelper(root, 0); // Print each root account and its subtree
    }
}

// Helper function to print tree nodes recursively
void ForestTree::printTreeHelper(NodePtr node, int level) const {
    if (!node) {
        return;
    }

    // Print indentation based on the level
    for (int i = 0; i < level; ++i) {
        cout << "  ";
    }

    // Print account details
    cout << node->getData().getAccountNumber() << " - " << node->getData().getDescription() << endl;

    // Recur for child nodes
    printTreeHelper(node->getLeftChild(), level + 1);

    // Recur for sibling nodes
    printTreeHelper(node->getRightSibling(), level);
}


