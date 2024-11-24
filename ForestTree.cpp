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
/**
 * @brief Default constructor for the ForestTree class.
 * Initializes the tree but does not allocate any nodes.
 */
ForestTree::ForestTree() {}

// Destructor
/**
 * @brief Destructor for the ForestTree class.
 * Deletes all nodes in the tree and clears the root accounts.
 */
ForestTree::~ForestTree() {
    cleanupTree();
}

/**
 * @brief Helper function to recursively delete all nodes in the tree.
 * Cleans up all dynamically allocated memory for the tree nodes.
 */
void ForestTree::cleanupTree() {
    for (NodePtr root: rootAccounts) {
        delete root;
    }
    rootAccounts.clear();
}

/**
 * @brief Initializes an empty forest tree by cleaning up any existing tree.
 * This method ensures the tree is empty before any new operations are performed.
 *
 * @details After calling this function, the tree will be reinitialized with no accounts.
 */
void ForestTree::initialize() {
    cleanupTree();
    cout << "Forest tree initialized successfully." << endl;
}

/**
 * @brief Builds a chart of accounts from a specified file.
 * The file should contain account details, one per line. Each line is parsed, and accounts are added to the tree.
 * If an account's parent is not found, it is treated as a root account.
 *
 * @param filename The name of the file containing the chart of accounts data.
 *
 * @details The file is expected to contain account information in a specific format. Each line should represent one account, with details such as account number and name.
 * Accounts are parsed using the `Account` class's extraction operator (operator>>), and are added to the tree structure.
 * In case of errors during file processing, the method catches exceptions and continues with the next line.
 */
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

/**
 * @brief Prints a detailed report of an account and its transaction history to a file.
 *
 * @param accountNumber The account number to generate the report for.
 * @param filename The name of the file where the report should be saved.
 *
 * @throws runtime_error If the file cannot be opened for writing.
 *
 * @details The report includes account details and all transactions associated with the account.
 * If no transactions are found, a message indicating no transactions will be written.
 */
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

/**
 * @brief Prints the entire chart of accounts (the forest tree) to the console.
 * This method traverses the tree and prints each account and its children.
 *
 * @details Each account is printed in a hierarchical format, with indentation to represent the tree structure.
 * If the tree is empty, a message indicating that will be printed instead.
 */
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

/**
 * @brief Finds an account by its account number.
 *
 * @param accountNumber The account number to search for.
 *
 * @return NodePtr A pointer to the node containing the account if found, or nullptr if not found.
 *
 * @details This method traverses the forest tree and searches for the account with the specified account number.
 * If the account is not found, nullptr is returned.
 */
NodePtr ForestTree::findAccount(int accountNumber) const {
    for (NodePtr root: rootAccounts) {
        if (root) {
            NodePtr found = root->findNode(root, accountNumber);
            if (found) return found;
        }
    }
    return nullptr;
}

/**
 * @brief Recursively prints the tree structure starting from a given node.
 *
 * @param node A pointer to the node to start printing from.
 * @param level The current level of indentation for the node being printed.
 *
 * @return void
 *
 * @details This helper function prints the details of each node in the tree recursively. The output includes
 * the account number, description, and balance. The level parameter is used to determine the indentation
 * for each node based on its depth in the tree.
 */
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

/**
 * @brief Adds a new account to the tree structure.
 *
 * @param newAccount The new account to be added.
 * @param parentNumber The account number of the parent to which the new account should be added.
 * If -1, the account is added as a root account.
 *
 * @return bool Returns true if the account was successfully added, false if it already exists or the parent is not found.
 *
 * @details This method adds an account to the tree. If the parent account is provided, the new account will be added
 * under the parent. If no parent is provided, the account is added as a root account. The method checks if the account
 * already exists before adding it, and if the parent is not found, it will search for an ancestor to add the account to.
 */
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

/**
 * @brief Adds a transaction to an account's transaction history.
 *
 * @param accountNumber The account number to which the transaction will be added.
 * @param transaction The transaction to be added to the account.
 *
 * @return bool Returns true if the transaction was successfully added, false if the account is not found or an error occurs.
 *
 * @details This method adds a transaction to the specified account's history and updates the account balance accordingly.
 * If the transaction is successfully added, the method attempts to save the transaction history to a file.
 */
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

/**
 * @brief Deletes a transaction from an account's transaction history.
 *
 * @param accountNumber The account number from which the transaction will be deleted.
 * @param transactionIndex The index of the transaction to delete.
 *
 * @return bool Returns true if the transaction was successfully deleted, false if the account or transaction is not found or an error occurs.
 *
 * @details This method removes a transaction from the specified account's history and updates the account balance accordingly.
 * If the transaction is successfully deleted, the method attempts to save the updated transaction history to a file.
 */
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

/**
 * @brief Saves the current state of the tree to a file, updating account balances.
 *
 * @param filename The name of the file to which the tree data should be saved.
 *
 * @return void
 *
 * @details This method reads all lines from the specified file into memory, processes each line to update account balances,
 * and then writes the updated data back to the file. Each account's balance is retrieved from the tree and updated in the file
 * accordingly. If an account is found in the tree, the balance is updated; otherwise, the original line is retained.
 * The method handles file reading, line processing, and file writing in a structured manner.
 */
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

/**
 * @brief Saves all transactions from the tree to a file.
 *
 * @param filename The name of the file to which the transaction data should be saved.
 *
 * @return void
 *
 * @details This method traverses the entire tree, saving all transactions for each account to the specified file.
 * Each transaction is saved in the format: account number, transaction ID, amount, debit/credit, date, and description.
 * The method uses a queue to traverse the tree level by level, ensuring that all accounts and their respective transactions
 * are processed and saved.
 */
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

/**
 * @brief Loads transactions from a file into the tree structure.
 *
 * @param filename The name of the file from which transaction data should be loaded.
 *
 * @return void
 *
 * @details This method reads each transaction from the specified file and attempts to add it to the corresponding account in
 * the tree. Each line in the file is expected to contain transaction data in the format: account number, transaction ID,
 * amount, debit/credit, date, and description. If the account number exists in the tree, the transaction is added to that account.
 * If the account cannot be found, the transaction is skipped. The method handles file reading and transaction parsing,
 * with error handling for invalid lines.
 */
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

/**
 * @brief Finds the root node for an account based on the first digit of the account number.
 *
 * @param accountNumber The account number for which the root node is to be found.
 *
 * @return NodePtr A pointer to the root node of the account if found, or nullptr if not found.
 *
 * @details This method attempts to find the root node of the account by comparing the first digit of the account number
 * with the first digit of the account numbers of all root accounts in the tree. If a match is found, the corresponding root node
 * is returned. If no match is found, nullptr is returned. This method assumes that accounts with the same first digit are
 * grouped together in the forest structure.
 */
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

/**
 * @brief Generates a transaction filename based on the provided accounts file name.
 *
 * @param accountsFile The name of the accounts file.
 *
 * @return string The generated transaction file name, which appends "_transactions.txt" to the accounts file name.
 *
 * @details This method takes the provided accounts file name and creates a corresponding transaction file name by
 * removing the file extension and appending "_transactions.txt". This helps to link the transaction file with the
 * respective accounts file in a consistent manner.
 */
string ForestTree::getTransactionFilename(const string &accountsFile) const {
    return accountsFile.substr(0, accountsFile.find_last_of('.')) + "_transactions.txt";
}

bool ForestTree::addAccountWithFile(int accountNumber, const string &description, double balance, string path) {
    Account newAccount;
    newAccount.setAccountNumber(accountNumber);
    newAccount.setDescription(description);
    newAccount.setBalance(balance);

    // Automatically determine parent account based on account number
    string accStr = to_string(accountNumber);
    int parentNumber = accStr.length() > 1 ? stoi(accStr.substr(0, accStr.length() - 1)) : -1;

    if (!addAccount(newAccount, parentNumber)) {
        return false;
    }

    // Create a transaction to update balances through the hierarchy
    if (balance != 0) {
        Transaction t("INIT", abs(balance), balance >= 0 ? 'D' : 'C', "Initial balance");
        addTransaction(accountNumber, t);
    }

    // Read all lines from the file
    vector<string> lines;
    string line;
    ifstream inFile(path);
    if (!inFile) {
        cerr << "Error opening file for reading" << endl;
        return false;
    }

    while (getline(inFile, line)) {
        lines.push_back(line);
    }
    inFile.close();

    // Find the correct position to insert the new account
    vector<string>::iterator insertPos = lines.end();
    string accNumStr = to_string(accountNumber);
    string parentStr = accStr.length() > 1 ? accStr.substr(0, accStr.length() - 1) : "";

    // First, find the parent's position
    vector<string>::iterator parentPos = lines.begin();
    for (; parentPos != lines.end(); ++parentPos) {
        istringstream iss(*parentPos);
        int currentAccNum;
        if (iss >> currentAccNum && currentAccNum == parentNumber) {
            break;
        }
    }

    // If we found the parent, start searching from there
    if (parentPos != lines.end()) {
        insertPos = parentPos + 1;
        for (; insertPos != lines.end(); ++insertPos) {
            istringstream iss(*insertPos);
            int currentAccNum;
            if (iss >> currentAccNum) {
                string currStr = to_string(currentAccNum);
                if (currStr.length() > parentStr.length() &&
                    currStr.substr(0, parentStr.length()) == parentStr) {
                    if (currStr.length() == accNumStr.length() &&
                        currentAccNum > accountNumber) {
                        break;
                    }
                } else if (currStr.length() <= parentStr.length() ||
                           currStr.substr(0, parentStr.length()) != parentStr) {
                    break;
                }
            }
        }
    } else {
        for (insertPos = lines.begin(); insertPos != lines.end(); ++insertPos) {
            istringstream iss(*insertPos);
            int currentAccNum;
            if (iss >> currentAccNum) {
                string currStr = to_string(currentAccNum);
                if (currStr[0] > accNumStr[0] ||
                    (currStr[0] == accNumStr[0] && currentAccNum > accountNumber)) {
                    break;
                }
            }
        }
    }

    // Update all balances in the lines based on the tree's current state
    for (auto &line: lines) {
        istringstream iss(line);
        int lineAccNum;
        if (iss >> lineAccNum) {
            NodePtr accNode = findAccount(lineAccNum);
            if (accNode) {
                string word;
                vector<string> words;
                while (iss >> word) {
                    words.push_back(word);
                }
                if (!words.empty()) {
                    words.pop_back(); // Remove old balance
                    ostringstream newLine;
                    newLine << lineAccNum;
                    for (const auto &w: words) {
                        newLine << " " << w;
                    }
                    newLine << " " << fixed << setprecision(2) << accNode->getData().getBalance();
                    line = newLine.str();
                }
            }
        }
    }

    // Create and insert the new line
    ostringstream newLine;
    newLine << accountNumber << " " << description << " " << fixed << setprecision(2) << balance;
    lines.insert(insertPos, newLine.str());

    // Write all lines back to the file
    ofstream outFile(path);
    if (!outFile) {
        cerr << "Error opening file for writing" << endl;
        return false;
    }

    for (const string &l: lines) {
        outFile << l << endl;
    }
    outFile.close();
    return true;
}
