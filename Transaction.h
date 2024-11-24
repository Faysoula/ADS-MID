//
// Created by Roa Al Assaad on 11/19/2024.
//

#ifndef ADS_MIDTERM_PROJECT_TRANSACTION_H
#define ADS_MIDTERM_PROJECT_TRANSACTION_H

#include <iostream>
#include <string>

using namespace std;

/**
 * @class Transaction
 * @brief Represents a financial transaction with details including ID, amount, debit/credit type, date, and description.
 *
 * The `Transaction` class allows for managing individual financial transactions, including their properties such as
 * transaction ID, amount, debit or credit type, date, and description. It also includes methods for validation,
 * applying to a balance, and input/output stream operations for reading and writing transaction data.
 */
class Transaction {
private:
    string transactionID; ///< The unique identifier for the transaction
    double amount;        ///< The amount involved in the transaction
    char debitCredit;     ///< The type of transaction: 'D' for debit, 'C' for credit
    string date;          ///< The date the transaction occurred
    string description;   ///< The description of the transaction

public:
    // Constructors

    /**
     * @brief Default constructor for Transaction class.
     *
     * Initializes a transaction with default values: empty transaction ID, zero amount, 'D' for debit, empty date,
     * and an empty description.
     */
    Transaction();

    /**
     * @brief Parameterized constructor for Transaction class.
     *
     * Initializes a transaction with the specified values for transaction ID, amount, debit/credit type, description,
     * and date.
     *
     * @param id The unique transaction ID
     * @param amt The amount involved in the transaction
     * @param type The type of transaction ('D' for debit, 'C' for credit)
     * @param desc The description of the transaction (optional, default is empty string)
     * @param dateStr The date of the transaction (optional, default is empty string)
     */
    Transaction(const string &id, double amt, char type, const string &desc = "", const string &dateStr = "");

    // Getters

    /**
     * @brief Returns the transaction ID.
     *
     * @return The transaction ID
     */
    string getTransactionID() const;

    /**
     * @brief Returns the transaction amount.
     *
     * @return The amount involved in the transaction
     */
    double getAmount() const;

    /**
     * @brief Returns the type of transaction (debit or credit).
     *
     * @return 'D' for debit, 'C' for credit
     */
    char getDebitCredit() const;

    /**
     * @brief Returns the date of the transaction.
     *
     * @return The date the transaction occurred
     */
    string getDate() const;

    /**
     * @brief Returns the description of the transaction.
     *
     * @return The description of the transaction
     */
    string getDescription() const;

    // Setters

    /**
     * @brief Sets the transaction ID.
     *
     * @param id The transaction ID to set
     */
    void setTransactionID(const string &id);

    /**
     * @brief Sets the transaction amount.
     *
     * @param amt The amount to set
     */
    void setAmount(double amt);

    /**
     * @brief Sets the type of transaction (debit or credit).
     *
     * @param type The type of transaction to set ('D' for debit, 'C' for credit)
     */
    void setDebitCredit(char type);

    /**
     * @brief Sets the date of the transaction.
     *
     * @param dateStr The date to set
     */
    void setDate(const string &dateStr);

    /**
     * @brief Sets the description of the transaction.
     *
     * @param desc The description to set
     */
    void setDescription(const string &desc);

    // Validation

    /**
     * @brief Validates the transaction.
     *
     * Checks whether the transaction has valid attributes (e.g., positive amount, valid debit/credit type).
     *
     * @return True if the transaction is valid, false otherwise
     */
    bool isValid() const;

    /**
     * @brief Applies the transaction to the specified balance.
     *
     * Depending on whether the transaction is a debit or credit, it will either subtract or add the amount to the balance.
     *
     * @param balance The balance to apply the transaction to
     * @return True if the transaction was successfully applied, false otherwise
     */
    bool applyToBalance(double &balance) const;
};

// Operators

/**
 * @brief Output stream operator for Transaction class.
 *
 * Outputs the transaction details (ID, amount, type, date, and description) to the output stream.
 *
 * @param os The output stream
 * @param transaction The transaction to output
 * @return The output stream
 */
ostream &operator<<(ostream &os, const Transaction &transaction);

/**
 * @brief Input stream operator for Transaction class.
 *
 * Reads the transaction details (ID, amount, type, date, and description) from the input stream.
 *
 * @param is The input stream
 * @param transaction The transaction to input data into
 * @return The input stream
 */
istream &operator>>(istream &is, Transaction &transaction);

#endif //ADS_MIDTERM_PROJECT_TRANSACTION_H



