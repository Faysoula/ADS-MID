//
// Created by asus on 11/19/2024.
//

#include "ForestTree.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;


// Constructor
ForestTree::ForestTree() {

}

// Destructor
ForestTree::~ForestTree() {
    for (Account* account : rootAccounts) {
        delete account;
    }
    rootAccounts.clear();
}

// Initialize an empty tree
void ForestTree::initialize() {
    for (Account* account : rootAccounts) {
        delete account;
    }
    rootAccounts.clear();
    cout << "Forest tree initialized successfully." << endl;
}

// Build chart of accounts from a file
void ForestTree::buildFromFile(const std::string& filename) {
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

        if (parentNumber == -1) {
            addAccount(accountNumber, description); // Add root account
        } else {
            addAccount(accountNumber, description, parentNumber); // Add subaccount
        }
    }

    file.close();
    cout << "Chart of accounts built from file successfully." << endl;
}

