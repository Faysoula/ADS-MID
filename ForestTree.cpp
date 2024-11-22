//
// Created by asus on 11/19/2024.
//
//Roa Al Assaad 11/22/2024 addTransaction and removeTransaction

#include "ForestTree.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>

using namespace std;

// Constructor
ForestTree::ForestTree() {}

// Destructor
ForestTree::~ForestTree() {
    cleanupTree();
}

void ForestTree::cleanupTree() {
    for (NodePtr root: rootAccounts) {
        delete root;
    }
    rootAccounts.clear();
}

// Function to initialize an empty forest tree
void ForestTree::initialize() {
    cleanupTree();
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
        istringstream iss(line);

        // Parse account number
        int accountNumber;
        if (!(iss >> accountNumber)) {
            continue;  // Skip invalid lines
        }

        // Parse description (everything up to the last number)
        string description;
        string word;
        string fullDescription;
        double balance = 0.0;

        // Read words until we hit the balance
        while (iss >> word) {
            // Try to convert to number (balance)
            try {
                balance = stod(word);
                break;  // If successful, this was the balance
            } catch (...) {
                // If not a number, this is part of the description
                if (!fullDescription.empty()) {
                    fullDescription += " ";
                }
                fullDescription += word;
            }
        }

        // Calculate parent number based on account number string
        string accStr = to_string(accountNumber);
        int parentNumber = accStr.length() > 1 ?
                           stoi(accStr.substr(0, accStr.length() - 1)) : -1;

        // Add account to tree
        addAccount(accountNumber, fullDescription, parentNumber);
    }

    file.close();
    cout << "Chart of accounts built from file successfully." << endl;
}

void ForestTree::printDetailedReport(int accountNumber, const string &filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Could not open file for writing: " + filename);
    }

    NodePtr accountNode = findAccount(accountNumber);
    if (!accountNode) {
        file << "Account not found: " << accountNumber << endl;
        return;
    }

    const Account &account = accountNode->getData();
    file << account << endl; // Using Account's operator<<

    file << "Transactions:" << endl;
    for (const Transaction &t: account.getTransactions()) {
        file << t << endl; // Using Transaction's operator<<
    }
}

void ForestTree::printForestTree() const {
    cout << "\nChart of Accounts:\n------------------\n";
    for (NodePtr root: rootAccounts) {
        if (root != nullptr) {
            printTreeHelper(root, 0);
        }
    }
    cout << "------------------\n";
}

NodePtr ForestTree::findAccount(int accountNumber) const {
    for (NodePtr root: rootAccounts) {
        if (root) {
            NodePtr found = root->findNode(root, accountNumber);
            if (found) return found;
        }
    }
    return nullptr;
}

// Helper function to print tree nodes recursively
void ForestTree::printTreeHelper(NodePtr node, int level) const {
    if (!node || !node->getData().getAccountNumber()) {
        return;
    }

    // Print indentation based on the level
    for (int i = 0; i < level; ++i) {
        cout << "  ";
    }

    // Print account details
    cout << node->getData().getAccountNumber() << " - "
         << node->getData().getDescription()
         << " (Balance: " << node->getData().getBalance() << ")" << endl;

    // Recur for child nodes
    printTreeHelper(node->getLeftChild(), level + 1);

    // Recur for sibling nodes
    printTreeHelper(node->getRightSibling(), level);
}

bool ForestTree::addAccount(int accountNumber, const string &description, int parentNumber) {
    // Create new account
    Account newAccount(accountNumber, description, 0.0);

    // Handle root accounts (single digit)
    if (parentNumber == -1) {
        if (findAccount(accountNumber)) {
            return false;  // Account already exists
        }
        NodePtr newNode = new TreeNode(newAccount);
        rootAccounts.push_back(newNode);
        return true;
    }

    // Find parent node for non-root accounts
    NodePtr parentNode = findAccount(parentNumber);
    if (!parentNode) {
        // If parent doesn't exist, try to find a suitable ancestor
        string accStr = to_string(accountNumber);
        string parentStr = accStr;
        while (parentStr.length() > 1 && !parentNode) {
            parentStr = parentStr.substr(0, parentStr.length() - 1);
            parentNode = findAccount(stoi(parentStr));
        }

        if (!parentNode) {
            return false;  // No suitable parent found
        }
    }

    // Check if account already exists
    if (findAccount(accountNumber)) {
        return false;
    }

    // Add the account under its parent
    bool success = false;
    for (NodePtr root: rootAccounts) {
        if (root->addAccountNode(root, newAccount)) {
            success = true;
            break;
        }
    }

    return success;
}

bool ForestTree::addTransaction(int accountNumber, double amount, const string &type) {
    // Validate transaction type
    if (type != "D" && type != "C") {
        cerr << "Error: Invalid transaction type. Must be 'D' (debit) or 'C' (credit)." << endl;
        return false;
    }

    // Find the account node by account number
    NodePtr accountNode = findAccount(accountNumber);
    if (!accountNode) {
        cerr << "Error: Account not found for account number: " << accountNumber << endl;
        return false;
    }

    // Create a unique transaction ID
    string transactionID = to_string(accountNumber) + "_" + to_string(time(nullptr));

    // Create the transaction
    Transaction transaction(transactionID, amount, type[0]);

    // Add the transaction and update balances
    try {
        accountNode->getData().addTransaction(transaction);
        accountNode->updateBalance(accountNode, transaction);
        return true;  // Success
    } catch (const invalid_argument &) {
        cerr << "Error: Invalid argument encountered while adding the transaction." << endl;
        return false;
    } catch (const out_of_range &) {
        cerr << "Error: Out-of-range exception encountered while adding the transaction." << endl;
        return false;
    } catch (...) {
        cerr << "Error: An unknown exception occurred while adding the transaction." << endl;
        return false;
    }
}