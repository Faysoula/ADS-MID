//
// Created by asus on 11/19/2024.
//

#ifndef FORESTTREE_H
#define FORESTTREE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Account.h"

using namespace std;

class ForestTree {
private:
    vector<Account*> rootAccounts; // Vector of root accounts

public:
    ForestTree();
    ~ForestTree();

    // Initialize an empty tree
    void initialize();

    // Build chart of accounts from a file
    void buildFromFile(const std::string& filename);

    // Add account (with optional parent)
    bool addAccount(int accountNumber, const std::string& description, int parentNumber = -1);

    // Add and delete transactions
    bool addTransaction(int accountNumber, double amount, const std::string& type);
    bool deleteTransaction(int accountNumber, int transactionId);

    // Print detailed report for an account
    void printDetailedReport(int accountNumber, const std::string& outputFilename);

    // Search for an account by number
    Account* searchAccount(int accountNumber) const;

    // Print the entire tree
    void printTree(const std::string& outputFilename) const;
};

#endif // FORESTTREE_H

