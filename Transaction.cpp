//
// Created by Roa Al Assaad on 11/20/2024.
//

/**
 * @file Transaction.cpp
 * @brief Defines the implementation of the `Transaction` class, which represents a financial transaction.
 *
 * This file implements the methods of the `Transaction` class, including constructors, getters, setters, validation,
 * balance adjustment, and overloaded input/output stream operators.
 */

#include "Transaction.h"
#include <iomanip>
#include <ctime>
#include <limits>

using namespace std;

// Default Constructor
/**
 * @brief Default constructor for the `Transaction` class.
 *
 * Initializes the transaction with default values:
 * - transactionID: an empty string
 * - amount: 0.0
 * - debitCredit: 'D' (Debit)
 * - date: an empty string
 * - description: an empty string
 */
Transaction::Transaction() : transactionID(""), amount(0.0), debitCredit('D'), date(""), description("") {}

// Parameterized Constructor
/**
 * @brief Parameterized constructor for the `Transaction` class.
 *
 * Initializes the transaction with the provided values for transactionID, amount, debit/credit type, description,
 * and date. Performs validation for amount and debit/credit type, defaulting to 0 for negative amounts and 'D' for
 * invalid types.
 *
 * @param id The transaction ID
 * @param amt The amount of the transaction
 * @param type The type of transaction ('D' for debit, 'C' for credit)
 * @param desc The description of the transaction (optional, default is empty string)
 * @param dateStr The date of the transaction (optional, default is empty string)
 */
Transaction::Transaction(const string &id, double amt, char type, const string &desc, const string &dateStr) {

    transactionID = id;
    date = dateStr;
    description = desc;

    if (amt >= 0) {
        amount = amt;
    } else {
        cerr << "Amount must be non-negative. Setting to 0." << endl;
        amount = 0.0;
    }

    if (type == 'D' || type == 'C') {
        debitCredit = type;
    } else {
        cerr << "Invalid type. Defaulting to 'D' (Debit)." << endl;
        debitCredit = 'D';
    }
}

// Getters

/**
 * @brief Returns the transaction ID.
 *
 * @return The transaction ID
 */
string Transaction::getTransactionID() const {
    return transactionID;
}

/**
 * @brief Returns the amount involved in the transaction.
 *
 * @return The transaction amount
 */
double Transaction::getAmount() const {
    return amount;
}

/**
 * @brief Returns the type of transaction ('D' for Debit, 'C' for Credit).
 *
 * @return The transaction type
 */
char Transaction::getDebitCredit() const {
    return debitCredit;
}

/**
 * @brief Returns the date of the transaction.
 *
 * @return The transaction date
 */
string Transaction::getDate() const {
    return date;
}

/**
 * @brief Returns the description of the transaction.
 *
 * @return The transaction description
 */
string Transaction::getDescription() const {
    return description;
}

// Setters

/**
 * @brief Sets the transaction ID.
 *
 * @param id The new transaction ID
 */
void Transaction::setTransactionID(const string &id) {
    if (id.empty()) {
        time_t now = time(nullptr);
        transactionID = "FMR" + to_string(now);
    } else {
        transactionID = id;
    }
}

/**
 * @brief Sets the transaction amount.
 *
 * If the amount is negative, it will be set to 0, and a message will be displayed.
 *
 * @param amt The new transaction amount
 */
void Transaction::setAmount(double amt) {
    if (amt >= 0) {
        amount = amt;
    } else {
        cerr << "Amount must be non-negative. Setting to 0." << endl;
        amount = 0.0;
    }
}

/**
 * @brief Sets the type of transaction (debit or credit).
 *
 *
 * @param type The type of transaction ('D' for Debit, 'C' for Credit)
 */
void Transaction::setDebitCredit(char type) {
    debitCredit = toupper(type);
}

/**
 * @brief Sets the date of the transaction.
 *
 * If the date is not provided (empty string), the current date will be used.
 *
 * @param dateStr The new transaction date
 */
void Transaction::setDate(const string &dateStr) {
    if (dateStr.empty()) {
        time_t now = time(nullptr);
        char buffer[11];  // DD-MM-YY\0 needs 9 chars + safety
        strftime(buffer, sizeof(buffer), "%d-%m-%y", localtime(&now));
        date = buffer;
    } else {
        date = dateStr;
    }
}

/**
 * @brief Sets the description of the transaction.
 *
 * @param desc The new transaction description
 */
void Transaction::setDescription(const string &desc) {
    description = desc;
}

// Validation

/**
 * @brief Validates the transaction.
 *
 * Checks if the transaction type is either 'D' (Debit) or 'C' (Credit) and if the amount is non-negative.
 *
 * @return True if the transaction is valid, false otherwise
 */
bool Transaction::isValid() const {
    return (debitCredit == 'D' || debitCredit == 'C') && amount >= 0;
}

// Apply Transaction to Balance

/**
 * @brief Applies the transaction to a balance.
 *
 * If the transaction is valid, it updates the balance by adding or subtracting the amount based on the transaction type.
 *
 * @param balance The balance to apply the transaction to
 * @return True if the transaction was successfully applied, false otherwise
 */
bool Transaction::applyToBalance(double &balance) const {
    if (!isValid()) {
        cerr << "Invalid transaction. Cannot apply." << endl;
        return false;
    }
    balance += (debitCredit == 'D') ? amount : -amount; // Add for debit, subtract for credit
    return true;
}

// Overloaded Output Stream Operator

/**
 * @brief Overloads the output stream operator for the `Transaction` class.
 *
 * Outputs the details of the transaction (ID, amount, type, date, description) to the stream.
 *
 * @param os The output stream
 * @param transaction The transaction to output
 * @return The output stream
 */
ostream &operator<<(ostream &os, const Transaction &transaction) {
    os << "Transaction ID: " << transaction.getTransactionID() << "\n"
       << "Amount: " << fixed << setprecision(2) << transaction.getAmount() << "\n"
       << "Type: " << (transaction.getDebitCredit() == 'D' ? "Debit" : "Credit") << "\n"
       << "Date: " << transaction.getDate() << "\n"
       << "Description: " << transaction.getDescription();
    return os;
}

// Overloaded Input Stream Operator

/**
 * @brief Overloads the input stream operator for the `Transaction` class.
 *
 * Prompts the user to input the details of a transaction (ID, amount, type, description) and sets the values of the
 * `Transaction` object accordingly.
 *
 * @param is The input stream
 * @param transaction The transaction to input data into
 * @return The input stream
 */
istream &operator>>(istream &is, Transaction &transaction) {
    string description;
    double amount;
    char debitCredit;

    transaction.setTransactionID("");


    bool validAmount = false;
    while (!validAmount) {
        cout << "Enter Amount: ";
        if (is >> amount) {
            if (amount >= 0) {
                validAmount = true;
                transaction.setAmount(amount);
            } else {
                cout << "Amount must be non-negative. Please try again.\n";
            }
        } else {
            cout << "Invalid amount. Please enter a valid number.\n";
            is.clear(); // Clear error flags
            is.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
        }
    }

    bool validType = false;
    while (!validType) {
        cout << "Enter Type (D/C): ";
        if (is >> debitCredit) {
            debitCredit = toupper(debitCredit);
            if (debitCredit == 'D' || debitCredit == 'C') {
                validType = true;
                transaction.setDebitCredit(debitCredit);
            } else {
                cout << "Invalid type. Please enter 'D' for Debit or 'C' for Credit.\n";
            }
        } else {
            cout << "Invalid input. Please enter 'D' or 'C'.\n";
            is.clear();
            is.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    cout << "Enter Description: ";
    is.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear any remaining newline
    getline(is, description);
    transaction.setDescription(description);

    // Set current date
    transaction.setDate("");

    return is;
}


