//
// Created by MayaH on 11/19/2024.
//
/**
 * @file Account.cpp
 * @brief Implements the `Account` class, which represents an individual account in the chart of accounts.
 *
 * This file defines the `Account` class, including methods for managing account properties such as account number,
 * description, balance, and associated transactions. The `Account` class provides functionalities to add or
 * remove transactions, adjust balances, and retrieve detailed account information for reporting purposes.
 */
#include "Account.h"
#include <iostream>
#include <iomanip>

using namespace std;
/**
 * @class Account
 * @brief Represents a financial account, including its details, balance, and transactions.
 *
 * This class provides functionalities to manage an account, including setting and retrieving
 * its details, managing transactions, and updating its balance.
 */

/**
 * @brief Default constructor for the Account class.
 *
 * Initializes the account number to 0, description to an empty string, and balance to 0.0.
 */
Account::Account() : accountNumber(0), description(""), balance(0.0) {}
/**
 * @brief Parameterized constructor for the Account class.
 *
 * @param num The account number.
 * @param desc The description of the account.
 * @param bal The initial balance of the account.
 */
Account::Account(int num, const string &desc, double bal) {
    accountNumber = num;
    description = desc;
    balance = bal;
}
/**
 * @brief Copy constructor for the Account class.
 *
 * @param acc The account object to be copied.
 */
Account::Account(const Account &acc) {
    accountNumber = acc.accountNumber;
    description = acc.description;
    balance = acc.balance;
    transactions = acc.transactions;
}
/**
 * @brief Destructor for the Account class.
 *
 * Cleans up resources used by the Account object, if any.
 */
Account::~Account() {}
/**
 * @brief Retrieves the account number.
 *
 * @return The account number.
 */
int Account::getAccountNumber() const {
    return accountNumber;
}

/**
 * @brief Retrieves the account balance.
 *
 * @return The account balance.
 */
double Account::getBalance() const {
    return balance;
}

/**
 * @brief Retrieves the list of transactions associated with the account.
 *
 * @return A constant reference to the vector of transactions.
 */
const vector<Transaction> &Account::getTransactions() const {
    return transactions;
}

/**
 * @brief Retrieves the total number of transactions.
 *
 * @return The number of transactions.
 */
int Account::getTransactionCount() const {
    return transactions.size();
}

/**
 * @brief Retrieves a specific transaction by its index.
 *
 * @param index The index of the transaction to retrieve.
 * @return The transaction at the specified index.
 * @throws std::out_of_range if the index is invalid.
 */
Transaction Account::getTransaction(int index) const {
    if (index >= 0 && index < transactions.size()) {
        return transactions[index];
    }
    throw out_of_range("Transaction index out of range :)");
}

/**
 * @brief Sets the account number.
 *
 * @param num The new account number.
 */
void Account::setAccountNumber(int num) {
    accountNumber = num;
}

/**
 * @brief Sets the account description.
 *
 * @param desc The new description of the account.
 */
void Account::setDescription(const string &desc) {
    description = desc;
}

/**
 * @brief Sets the account balance.
 *
 * @param bal The new balance of the account.
 */
void Account::setBalance(double bal) {
    balance = bal;
}

/**
 * @brief Updates a specific transaction and adjusts the account balance accordingly.
 *
 * @param index The index of the transaction to update.
 * @param t The new transaction data.
 * @throws std::out_of_range if the index is invalid.
 */
void Account::setTransaction(int index, const Transaction &t) {
    if (index >= 0 && index < transactions.size()) {
        Transaction oldT = transactions[index];
        if (oldT.getDebitCredit() == 'D') {
            balance -= oldT.getAmount();
        } else {
            balance += oldT.getAmount();
        }
        transactions[index] = t;
        updateBalance(t);
    } else {
        throw out_of_range("Transaction out of range :)");
    }
}
/**
 * @brief Adds a new transaction to the account.
 *
 * @param t The transaction to add.
 */
void Account::addTransaction(const Transaction &t) {
    transactions.push_back(t);
}

/**
 * @brief Removes a transaction from the account.
 *
 * @param index The index of the transaction to remove.
 * @throws std::out_of_range if the index is invalid.
 */
void Account::removeTransaction(int index) {
    if (index >= 0 && index < transactions.size()) {
        Transaction t = transactions[index];
        transactions.erase(transactions.begin() + index);
    }
}

/**
 * @brief Updates the account balance based on a transaction.
 *
 * @param t The transaction affecting the balance.
 */
void Account::updateBalance(const Transaction &t) {
    if (t.getDebitCredit() == 'D') {
        balance += t.getAmount();
    } else if (t.getDebitCredit() == 'C') {
        balance -= t.getAmount();
    }
}

/**
 * @brief Retrieves a short description of the account.
 *
 * Truncates the description to a maximum of 10 characters.
 *
 * @return The truncated account description.
 */
string Account::getShortDescription() const {
    return description.length() > 10 ? description.substr(0, 10) : description;
}

/**
 * @brief Overloads the output stream operator to print account details.
 *
 * @param os The output stream.
 * @param account The account to be printed.
 * @return The output stream with the account details.
 */

/*bool Account::isParentOf(int otherAccountNum) const {
    string parentNum = to_string(accountNumber);
    string childNum = to_string(otherAccountNum);
    return childNum.length()>parentNum.length() &&
        childNum.substr(0, parentNum.length()) == parentNum;
}*/

ostream &operator<<(ostream &os, const Account &account) {
    os << account.getAccountNumber() << " "
       << account.getShortDescription() << " "
       << fixed << std::setprecision(2) << account.getBalance();
    return os;
}

istream& operator>>(istream& is, Account& account) {
    int accNum;
    is >> accNum;  // Read account number
    account.setAccountNumber(accNum);

    // Skip any whitespace before the description
    is >> ws;

    string desc;
    double bal;

    // Read the entire line into a string
    string line;
    getline(is, line);

    // Create a stream from the line
    istringstream lineStream(line);

    // Vector to store all words
    vector<string> words;
    string word;

    // Read all words into vector
    while (lineStream >> word) {
        words.push_back(word);
    }

    if (!words.empty()) {
        // Last word should be the balance
        try {
            bal = stod(words.back());
            words.pop_back();  // Remove balance from words
        } catch (...) {
            bal = 0.0;  // Default balance if not found
        }

        // Join remaining words for description
        desc = "";
        for (size_t i = 0; i < words.size(); ++i) {
            if (i > 0) desc += " ";
            desc += words[i];
        }
    }

    account.setDescription(desc);
    account.setBalance(bal);
    return is;
}