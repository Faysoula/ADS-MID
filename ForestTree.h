//
// Created by asus on 11/19/2024.
//

#ifndef FORESTTREE_H
#define FORESTTREE_H

#include <iostream>
#include <string>
#include <vector>
#include "TreeNode.h"
#include "Account.h"
#include "Transaction.h"

using namespace std;

class ForestTree {
private:
    vector<NodePtr> rootAccounts; // Vector of root nodes representing the forest tree
    void cleanupTree();

public:
    // Constructor and Destructor
    ForestTree();

    ~ForestTree();

    void initialize();

    void buildFromFile(const string &filename);

    //needs fixing with tree node add acount maybe use inorder to add
    bool addAccount(const Account& newAccount, int parentNumber);

    bool addTransaction(int accountNumber, Transaction &transaction);

    bool deleteTransaction(int accountNumber, int transactionIndex);

    void printDetailedReport(int accountNumber, const string &filename) const;

    void printForestTree() const;

    NodePtr findAccount(int accountNumber) const;

private:
    // Helper function to recursively print tree nodes
    void printTreeHelper(NodePtr node, int level) const;
};

#endif // FORESTTREE_H

