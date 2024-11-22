//
// Created by asus on 11/19/2024.
//

#ifndef FORESTTREE_H
#define FORESTTREE_H

#include <iostream>
#include <string>
#include <vector>
#include "TreeNode.h"

using namespace std;

class ForestTree {
private:
    vector<NodePtr> rootAccounts; // Vector of root nodes representing the forest tree

public:
    // Constructor and Destructor
    ForestTree();
    ~ForestTree();

    // Function to initialize an empty forest tree
    void initialize();

    // Function to build a chart of accounts from a file
    void buildFromFile(const string &filename);

    // Utility to print the entire forest tree (for debugging and verification)
    void printForestTree() const;

private:
    // Helper function to recursively print tree nodes
    void printTreeHelper(NodePtr node, int level) const;
    // Recursive helper to find a node by account number
    NodePtr findNodeByAccountNumber(NodePtr node, int accountNumber) const;


};

#endif // FORESTTREE_H

