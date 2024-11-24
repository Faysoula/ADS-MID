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

/**
 * @class ForestTree
 * @brief Represents a forest tree data structure for managing accounts and transactions.
 *
 * @details This class manages a collection of accounts, organized as a forest of trees, with each account represented
 * by a node. It provides methods for adding, deleting, and updating accounts and transactions, as well as loading and
 * saving account data to and from files. The class also includes functionality to print reports and details about the tree
 * structure.
 */
class ForestTree {
private:
    /**
     * @brief A vector of root nodes representing the forest tree.
     *
     * @details This vector holds the root nodes of the various trees in the forest. Each tree represents a group of
     * accounts that share a common root.
     */
    vector<NodePtr> rootAccounts;

    /**
     * @brief Cleans up the tree, deleting all nodes.
     *
     * @details This private helper method is responsible for deallocating memory and cleaning up the tree when the
     * ForestTree object is destroyed or reset.
     */
    void cleanupTree();

public:
    /**
     * @brief Default constructor for the ForestTree class.
     *
     * @details Initializes an empty forest with no accounts or nodes. The constructor prepares the object to be used
     * for further operations such as adding accounts and transactions.
     */
    ForestTree();

    /**
     * @brief Destructor for the ForestTree class.
     *
     * @details Ensures that any dynamically allocated memory is cleaned up properly when the ForestTree object is
     * destroyed.
     */
    ~ForestTree();

    /**
     * @brief Initializes the forest tree.
     *
     * @details This method is used to set up the forest tree, typically after the object has been created. It prepares
     * the structure for further operations.
     */
    void initialize();

    /**
     * @brief Builds the tree structure from a file.
     *
     * @param filename The name of the file containing account data.
     *
     * @return void
     *
     * @details This method reads account data from the specified file and builds the forest tree structure accordingly.
     * Each account in the file is parsed and added as a node in the tree.
     */
    void buildFromFile(const string &filename);

    /**
     * @brief Adds a new account to the tree.
     *
     * @param newAccount The account to be added.
     * @param parentNumber The account number of the parent account.
     *
     * @return bool True if the account is successfully added, false otherwise.
     *
     * @details This method adds a new account as a child of the account specified by the parentNumber. If the parent
     * account is found, the new account is added to the tree structure.
     */
    bool addAccount(const Account &newAccount, int parentNumber);

    /**
     * @brief Adds a transaction to an account.
     *
     * @param accountNumber The account number to which the transaction will be added.
     * @param transaction The transaction to be added.
     *
     * @return bool True if the transaction is successfully added, false otherwise.
     *
     * @details This method adds a transaction to the account specified by accountNumber. The transaction is appended
     * to the list of transactions for the account.
     */
    bool addTransaction(int accountNumber, Transaction &transaction);

    /**
     * @brief Deletes a transaction from an account.
     *
     * @param accountNumber The account number from which the transaction will be deleted.
     * @param transactionIndex The index of the transaction to be deleted.
     *
     * @return bool True if the transaction is successfully deleted, false otherwise.
     *
     * @details This method removes a transaction from the account specified by accountNumber. The transaction is
     * identified by its index in the list of transactions for the account.
     */
    bool deleteTransaction(int accountNumber, int transactionIndex);

    /**
     * @brief Prints a detailed report of an account to a file.
     *
     * @param accountNumber The account number for which the report is to be generated.
     * @param filename The name of the file to save the report.
     *
     * @return void
     *
     * @details This method generates a detailed report of the account specified by accountNumber and saves it to the
     * file specified by filename. The report includes information about the account's transactions and balances.
     */
    void printDetailedReport(int accountNumber, const string &filename) const;

    /**
     * @brief Prints the structure of the forest tree.
     *
     * @return void
     *
     * @details This method prints the entire forest tree structure, showing all accounts and their relationships
     * with parent and child accounts.
     */
    void printForestTree() const;

    /**
     * @brief Finds an account by its account number.
     *
     * @param accountNumber The account number to search for.
     *
     * @return NodePtr A pointer to the node containing the account if found, or nullptr if not found.
     *
     * @details This method searches the forest tree for an account with the specified account number. If the account
     * is found, the corresponding node is returned, otherwise, nullptr is returned.
     */
    NodePtr findAccount(int accountNumber) const;

    /**
     * @brief Saves the forest tree structure to a file.
     *
     * @param filename The name of the file to save the tree structure.
     *
     * @return void
     *
     * @details This method saves the entire forest tree structure to a file. Each node in the tree is written to the
     * file, preserving the hierarchy of accounts.
     */
    void saveToFile(const string &filename) const;

    /**
     * @brief Saves all transactions to a file.
     *
     * @param filename The name of the file to save the transactions.
     *
     * @return void
     *
     * @details This method saves all transactions from all accounts in the forest tree to a file. Each transaction
     * is saved with relevant details, such as transaction ID, amount, date, and description.
     */
    void saveTransactions(const string &filename) const;

    /**
     * @brief Loads transactions from a file.
     *
     * @param filename The name of the file to load the transactions from.
     *
     * @return void
     *
     * @details This method reads transaction data from the specified file and adds each transaction to the corresponding
     * account in the forest tree.
     */
    void loadTransactions(const string &filename);

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
    string getTransactionFilename(const string &accountsFile) const;

    /**
     * @brief Adds a new account to both the tree structure and the file.
     *
     * @param accountNumber The account number
     * @param description The account description
     * @param balance The initial balance
     * @return bool Returns true if the account was successfully added, false otherwise
     */
    bool addAccountWithFile(int accountNumber, const string &description, double balance, string path);

private:
    /**
     * @brief Helper function to recursively print tree nodes.
     *
     * @param node The current node to print.
     * @param level The current level of the tree.
     *
     * @return void
     *
     * @details This private helper method is used to recursively print the nodes of the tree, starting from the
     * given node. It prints the structure of the tree, showing each node's account number and level in the hierarchy.
     */
    void printTreeHelper(NodePtr node, int level) const;

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
    NodePtr findRootForAccount(int accountNumber) const;
};

#endif // FORESTTREE_H


