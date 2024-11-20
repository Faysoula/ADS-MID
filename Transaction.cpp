//
// Created by Roa Al Assaad on 11/20/2024.
//

#include "Transaction.h"
#include <iomanip>

using namespace std;

// Default Constructor
Transaction::Transaction() : transactionID(""), amount(0.0), debitCredit('D'), date(""), description("") {}

// Parameterized Constructor
Transaction::Transaction(const string& id, double amt, char type, const string& desc, const string& dateStr) {

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
string Transaction::getTransactionID() const {
        return transactionID;
}

double Transaction::getAmount() const {
        return amount;
}

char Transaction::getDebitCredit() const {
        return debitCredit;
}

string Transaction::getDate() const {
        return date;
}

string Transaction::getDescription() const {
        return description;
}

// Setters
void Transaction::setTransactionID(const string& id) {
        transactionID = id;
}

void Transaction::setAmount(double amt) {
    if (amt >= 0) {
        amount = amt;
    } else {
        cerr << "Amount must be non-negative. Setting to 0." << endl;
        amount = 0.0;
    }
}

void Transaction::setDebitCredit(char type) {
    if (type == 'D' || type == 'C') {
        debitCredit = type;
    } else {
        cerr << "Invalid type. Defaulting to 'D' (Debit)." << endl;
        debitCredit = 'D';
    }
}

void Transaction::setDate(const string& dateStr) {
        date = dateStr;
}

void Transaction::setDescription(const string& desc) {
        description = desc;
}

// Validation
bool Transaction::isValid() const {
    return (debitCredit == 'D' || debitCredit == 'C') && amount >= 0;
}

// Apply Transaction to Balance
bool Transaction::applyToBalance(double& balance) const {
    if (!isValid()) {
        cerr << "Invalid transaction. Cannot apply." << endl;
        return false;
    }
    balance += (debitCredit == 'D') ? amount : -amount; // Add for debit, subtract for credit
    return true;
}

// Static Functions
bool Transaction::compareByDate(const Transaction& t1, const Transaction& t2) {
    return t1.date < t2.date;
}

bool Transaction::compareByAmount(const Transaction& t1, const Transaction& t2) {
    return t1.amount < t2.amount;
}


// Overloaded Output Stream Operator
ostream& operator<<(ostream& os, const Transaction& transaction) {
    os << "Transaction ID: " << transaction.getTransactionID() << "\n"
       << "Amount: " << fixed << setprecision(2) << transaction.getAmount() << "\n"
       << "Type: " << (transaction.getDebitCredit() == 'D' ? "Debit" : "Credit") << "\n"
       << "Date: " << transaction.getDate() << "\n"
       << "Description: " << transaction.getDescription();
    return os;
}

// Overloaded Input Stream Operator
istream& operator>>(istream& is, Transaction& transaction) {
    string transactionID, date, description;
    double amount;
    char debitCredit;

    cout << "Enter Transaction ID: ";
    is >> transactionID;
    cout << "Enter Amount: ";
    is >> amount;
    cout << "Enter Type (D/C): ";
    is >> debitCredit;
    is.ignore();
    cout << "Enter Date (YYYY-MM-DD): ";
    getline(is, date);
    cout << "Enter Description: ";
    getline(is, description);

    transaction.setTransactionID(transactionID);
    transaction.setAmount(amount);
    transaction.setDebitCredit(debitCredit);
    transaction.setDate(date);
    transaction.setDescription(description);

    return is;
}

