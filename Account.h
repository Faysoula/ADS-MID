//
// Created by MayaH on 11/19/2024.
//

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>
#include <iostream>
#include "Transaction.h"
using namespace std;

/**
 * @class Account
 * @brief Represents a financial account with account number, description, balance, and transactions.
 *
 * @details The `Account` class models a financial account, which includes an account number, a description, a balance,
 * and a list of transactions. It provides methods for adding, removing, and updating transactions, as well as managing
 * the balance. The class also supports input and output operations for account data.
 */
class Account {
private:
    int accountNumber;               ///< The account number
    string description;              ///< The description of the account
    double balance;                  ///< The current balance of the account
    vector<Transaction> transactions;///< The list of transactions associated with the account

public:
    // Constructors & Destructor

    /**
     * @brief Default constructor for Account class.
     *
     * Initializes the account with default values: account number 0, empty description, and balance 0.0.
     */
    Account();

    /**
     * @brief Parameterized constructor for Account class.
     *
     * Initializes the account with a given account number, description, and balance.
     *
     * @param num The account number
     * @param desc The account description
     * @param bal The account balance
     */
    Account(int num, const string& desc, double bal);

    /**
     * @brief Copy constructor for Account class.
     *
     * Creates a copy of the provided `Account` object.
     *
     * @param acc The account to copy
     */
    Account(const Account& acc);

    /**
     * @brief Destructor for Account class.
     *
     * Destructor that does not require specific cleanup as no dynamic memory is used.
     */
    ~Account();

    // Getters

    /**
     * @brief Returns the account number.
     *
     * @return The account number
     */
    int getAccountNumber() const;

    /**
     * @brief Returns the account description.
     *
     * @return The description of the account
     */
    string getDescription() const;

    /**
     * @brief Returns the current balance of the account.
     *
     * @return The balance of the account
     */
    double getBalance() const;

    /**
     * @brief Returns a reference to the list of transactions.
     *
     * @return The list of transactions associated with the account
     */
    const vector<Transaction>& getTransactions() const;

    /**
     * @brief Returns the number of transactions in the account.
     *
     * @return The transaction count
     */
    int getTransactionCount() const;

    /**
     * @brief Returns the transaction at the specified index.
     *
     * @param index The index of the transaction to retrieve
     * @return The transaction at the specified index
     * @throws out_of_range If the index is out of range
     */
    Transaction getTransaction(int index) const;

    // Setters

    /**
     * @brief Sets the account number.
     *
     * @param num The account number to set
     */
    void setAccountNumber(int num);

    /**
     * @brief Sets the account description.
     *
     * @param desc The account description to set
     */
    void setDescription(const string& desc);

    /**
     * @brief Sets the account balance.
     *
     * @param bal The balance to set
     */
    void setBalance(double bal);

    /**
     * @brief Sets the transaction at the specified index.
     *
     * Updates the transaction at the specified index, adjusting the balance accordingly.
     *
     * @param index The index of the transaction to replace
     * @param t The new transaction to set at the specified index
     * @throws out_of_range If the index is out of range
     */
    void setTransaction(int index, const Transaction& t);

    // Operations

    /**
     * @brief Adds a transaction to the account.
     *
     * Adds the specified transaction to the account's list of transactions and updates the balance.
     *
     * @param t The transaction to add
     */
    void addTransaction(const Transaction& t);

    /**
     * @brief Removes the transaction at the specified index.
     *
     * Removes the transaction from the list and adjusts the balance accordingly.
     *
     * @param index The index of the transaction to remove
     */
    void removeTransaction(int index);

    /**
     * @brief Updates the balance of the account based on a transaction.
     *
     * Adjusts the balance by adding or subtracting the transaction amount, depending on whether it's a debit or credit.
     *
     * @param t The transaction to process
     */
    void updateBalance(const Transaction& t);

    /**
     * @brief Returns a short version of the account description.
     *
     * If the description is longer than 10 characters, returns the first 10 characters; otherwise, returns the full description.
     *
     * @return A short version of the account description
     */
    string getShortDescription() const;
    //bool isParentOf(int otherAccountNum) const;

};

// Operators
/**
     * @brief Output stream operator for Account class.
     *
     * Outputs the account's number, short description, and balance to the output stream.
     *
     * @param os The output stream
     * @param account The account to output
     * @return The output stream
     */
ostream& operator<<(ostream& os, const Account& account);
/**
     * @brief Input stream operator for Account class.
     *
     * Reads an account's data from the input stream, including account number, description, and balance.
     *
     * @param is The input stream
     * @param account The account to input data into
     * @return The input stream
     */
istream& operator>>(istream& is, Account& account);

#endif //ACCOUNT_H
