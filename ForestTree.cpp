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
#include <queue>

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

        } catch (const exception &e) {
            cerr << "Error processing line: " << line << endl;
            cerr << "Error details: " << e.what() << endl;
            continue;  // Skip this line and continue with the next one
        }
    }

    file.close();
    cout << "Chart of accounts built from file successfully." << endl;
    loadTransactions(getTransactionFilename(filename));
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
    for (NodePtr root: rootAccounts) {
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

bool ForestTree::addAccount(const Account &newAccount, int parentNumber) {
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

bool ForestTree::addTransaction(int accountNumber, Transaction &transaction) {
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
        cout << "Error: Account not found for account number: " << accountNumber << endl;
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

        try {
            saveTransactions(getTransactionFilename("accountswithspace.txt"));
        } catch (const exception &e) {
            cerr << "Warning: Failed to save transactions: " << e.what() << endl;
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
        cout << "Error: Account not found for account number: " << accountNumber << endl;
        return false;
    }

    Account &account = accountNode->getData();
    const vector<Transaction> &transactions = account.getTransactions();

    // Validate transaction index
    if (transactionIndex < 0 || transactionIndex >= transactions.size()) {
        cout << "Error: Invalid transaction index. Please enter a number between 0 and "
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

        try {
            saveTransactions(getTransactionFilename("accountswithspace.txt"));
        } catch (const exception &e) {
            cerr << "Warning: Failed to save transactions: " << e.what() << endl;
        }

        return true;

        return true;
    } catch (const exception &e) {
        cerr << "Error while deleting transaction: " << e.what() << endl;
        return false;
    }
}

void ForestTree::saveToFile(const string &filename) const {
    // First, read all lines from the file into memory
    ifstream inFile(filename);
    if (!inFile) {
        throw runtime_error("Unable to open file for reading: " + filename);
    }

    vector<string> lines;
    string line;

    while (getline(inFile, line)) {
        lines.push_back(line);
    }
    inFile.close();

    // Update balances in the lines
    for (auto &line: lines) {
        istringstream iss(line);
        int accountNum;
        string description;

        if (!(iss >> accountNum)) {
            continue; // Skip invalid lines
        }

        // Find this account in our tree
        NodePtr accountNode = findAccount(accountNum);
        if (!accountNode) {
            continue; // Account not found, keep original line
        }

        // Read until the last number (current balance)
        string word;
        vector<string> words;
        while (iss >> word) {
            words.push_back(word);
        }

        if (!words.empty()) {
            // Remove the last word (old balance)
            words.pop_back();

            // Create new line with updated balance
            ostringstream newLine;
            newLine << accountNum;
            for (const auto &w: words) {
                newLine << " " << w;
            }
            newLine << " " << fixed << setprecision(2) << accountNode->getData().getBalance();

            line = newLine.str();
        }
    }

    // Write updated content back to file
    ofstream outFile(filename);
    if (!outFile) {
        throw runtime_error("Unable to open file for writing: " + filename);
    }

    for (const auto &line: lines) {
        outFile << line << endl;
    }
}

void ForestTree::saveTransactions(const string &filename) const {
    ofstream file(filename);
    if (!file) {
        throw runtime_error("Unable to open transaction file for writing: " + filename);
    }

    // For each account in the tree
    for (NodePtr root: rootAccounts) {
        if (!root) continue;

        // Use a queue to traverse all nodes
        queue<NodePtr> nodeQueue;
        nodeQueue.push(root);

        while (!nodeQueue.empty()) {
            NodePtr current = nodeQueue.front();
            nodeQueue.pop();

            // Save transactions for current account
            const Account &account = current->getData();
            const vector<Transaction> &transactions = account.getTransactions();

            for (const Transaction &t: transactions) {
                file << account.getAccountNumber() << "|"
                     << t.getTransactionID() << "|"
                     << t.getAmount() << "|"
                     << t.getDebitCredit() << "|"
                     << t.getDate() << "|"
                     << t.getDescription() << endl;
            }

            // Add child and sibling to queue
            if (current->getLeftChild()) nodeQueue.push(current->getLeftChild());
            if (current->getRightSibling()) nodeQueue.push(current->getRightSibling());
        }
    }
    file.close();
}

void ForestTree::loadTransactions(const string &filename) {
    ifstream file(filename);
    if (!file) {
        return; // It's okay if the file doesn't exist yet
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string field;
        vector<string> fields;

        // Split line by '|'
        while (getline(iss, field, '|')) {
            fields.push_back(field);
        }

        if (fields.size() < 6) continue; // Skip invalid lines

        try {
            int accountNum = stoi(fields[0]);
            NodePtr accountNode = findAccount(accountNum);
            if (!accountNode) continue;

            // Create and add transaction
            Transaction t(fields[1],                    // ID
                          stod(fields[2]),               // Amount
                          fields[3][0],                  // Debit/Credit
                          fields[5],                     // Description
                          fields[4]);                    // Date

            // Add transaction without updating file
            accountNode->getData().addTransaction(t);
            //accountNode->updateBalance(findRootForAccount(accountNum), t);

        } catch (const exception &e) {
            cerr << "Error loading transaction: " << e.what() << endl;
            continue;
        }
    }
    file.close();
}

// Helper function to find root node for an account
NodePtr ForestTree::findRootForAccount(int accountNumber) const {
    string accStr = to_string(accountNumber);
    char firstDigit = accStr[0];
    for (NodePtr root: rootAccounts) {
        if (root && to_string(root->getData().getAccountNumber())[0] == firstDigit) {
            return root;
        }
    }
    return nullptr;
}

string ForestTree::getTransactionFilename(const string &accountsFile) const {
    return accountsFile.substr(0, accountsFile.find_last_of('.')) + "_transactions.txt";
}
