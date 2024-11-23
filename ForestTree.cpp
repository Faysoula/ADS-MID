//
// Created by asus on 11/19/2024.
//
//Roa Al Assaad 11/22/2024 addTransaction and removeTransaction

#include "ForestTree.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <iomanip>
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
void ForestTree::buildFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        // Create a string stream from the line
        istringstream lineStream(line);
        Account newAccount;

        try {
            // Use Account's operator>> to read the account details
            lineStream >> newAccount;

            // Calculate parent number based on account number string
            string accStr = to_string(newAccount.getAccountNumber());
            int parentNumber = accStr.length() > 1 ?
                             stoi(accStr.substr(0, accStr.length() - 1)) : -1;

            // Add account to tree
            addAccount(newAccount, parentNumber);

        } catch (const exception& e) {
            cerr << "Error processing line: " << line << endl;
            cerr << "Error details: " << e.what() << endl;
            continue;  // Skip this line and continue with the next one
        }
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

    // Print account header and information
    file << "Account Details:\n";
    file << "================\n";
    file << account << "\n\n";  // Using Account's operator<<

    // Print transactions
    file << "Transaction History:\n";
    file << "===================\n";
    const vector<Transaction> &transactions = account.getTransactions();

    if (transactions.empty()) {
        file << "No transactions recorded.\n";
    } else {
        for (const Transaction &t: transactions) {
            file << t << "\n\n";  // Using Transaction's operator<<
        }
    }

    file.close();
}

void ForestTree::printForestTree() const {
    if (rootAccounts.empty()) {
        cout << "Tree is empty." << endl;
        return;
    }

    cout << "\nChart of Accounts:\n==================\n";
    for (NodePtr root : rootAccounts) {
        if (root) {
            printTreeHelper(root, 0);
        }
    }
    cout << "==================\n";
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

bool ForestTree::addAccount(const Account& newAccount, int parentNumber) {
    int accNum = newAccount.getAccountNumber();

    // Handle root accounts (single digit)
    if (parentNumber == -1) {
        if (findAccount(accNum)) {
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
        string accStr = to_string(accNum);
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
    if (findAccount(accNum)) {
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

bool ForestTree::addTransaction(int accountNumber, Transaction& transaction) {
    // Find the account node and its root
    NodePtr accountNode = nullptr;
    NodePtr rootNode = nullptr;

    // Search through all root nodes to find both the account and its root
    for (NodePtr root: rootAccounts) {
        accountNode = root->findNode(root, accountNumber);
        if (accountNode) {
            rootNode = root;
            // Keep searching until we find the top-most root
            while (rootNode->getRightSibling() &&
                   to_string(rootNode->getRightSibling()->getData().getAccountNumber())[0] ==
                   to_string(accountNode->getData().getAccountNumber())[0]) {
                rootNode = rootNode->getRightSibling();
            }
            break;
        }
    }

    if (!accountNode) {
        cerr << "Error: Account not found for account number: " << accountNumber << endl;
        return false;
    }

    try {
        // First add the transaction to the account
        accountNode->getData().addTransaction(transaction);

        // Then update balances starting from the main root of this account's tree
        for (NodePtr root: rootAccounts) {
            if (to_string(root->getData().getAccountNumber())[0] ==
                to_string(accountNumber)[0]) {
                accountNode->updateBalance(root, transaction);
                break;
            }
        }
        return true;
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return false;
    }
}

bool ForestTree::deleteTransaction(int accountNumber, int transactionIndex) {
    // Find the account node and its root
    NodePtr accountNode = nullptr;
    NodePtr rootNode = nullptr;

    // Search through all root nodes to find both the account and its root
    for (NodePtr root: rootAccounts) {
        accountNode = root->findNode(root, accountNumber);
        if (accountNode) {
            rootNode = root;
            while (rootNode->getRightSibling() &&
                   to_string(rootNode->getRightSibling()->getData().getAccountNumber())[0] ==
                   to_string(accountNode->getData().getAccountNumber())[0]) {
                rootNode = rootNode->getRightSibling();
            }
            break;
        }
    }

    if (!accountNode) {
        cerr << "Error: Account not found for account number: " << accountNumber << endl;
        return false;
    }

    Account &account = accountNode->getData();
    const vector<Transaction> &transactions = account.getTransactions();

    // Validate transaction index
    if (transactionIndex < 0 || transactionIndex >= transactions.size()) {
        cerr << "Error: Invalid transaction index. Please enter a number between 0 and "
             << transactions.size() - 1 << endl;
        return false;
    }

    try {
        // Get the transaction before removing it to update balances
        Transaction deletedTransaction = transactions[transactionIndex];

        // Create an inverse transaction to update balances
        Transaction inverseTransaction(
                deletedTransaction.getTransactionID(),
                deletedTransaction.getAmount(),
                deletedTransaction.getDebitCredit() == 'D' ? 'C' : 'D'  // Invert D to C and C to D
        );

        // Remove the transaction from the account
        account.removeTransaction(transactionIndex);

        // Update balances through the hierarchy using the inverse transaction
        for (NodePtr root: rootAccounts) {
            if (to_string(root->getData().getAccountNumber())[0] ==
                to_string(accountNumber)[0]) {
                accountNode->updateBalance(root, inverseTransaction);
                break;
            }
        }

        return true;
    } catch (const exception &e) {
        cerr << "Error while deleting transaction: " << e.what() << endl;
        return false;
    }
}