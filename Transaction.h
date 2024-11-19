//
// Created by Roa Al Assaad on 11/19/2024.
//

#ifndef ADS_MIDTERM_PROJECT_TRANSACTION_H
#define ADS_MIDTERM_PROJECT_TRANSACTION_H

#include <iostream>
#include <string>

using namespace std;

class Transaction {
private:
    string transactionID;
    double amount;
    char debitCredit;
    string date;
    string description;

public:
    // Constructors
    Transaction();
    Transaction(const string& id, double amt, char type, const string& desc = "",
        const string& dateStr = "");

    // Getters
    string getTransactionID() const;
    double getAmount() const;
    char getDebitCredit() const;
    string getDate() const;
    string getDescription() const;

    // Setters
    void setTransactionID(const string& id);
    void setAmount(double amt);
    void setDebitCredit(char type);
    void setDate(const string& dateStr);
    void setDescription(const string& desc);

    // Operator Overloads
    friend ostream& operator<<(ostream& os, const Transaction& transaction);
    friend istream& operator>>(istream& is, Transaction& transaction);

    // Validation
    bool isValid() const;
    bool applyToBalance(double& balance) const;

    // Static Functions
    static bool compareByDate(const Transaction& t1, const Transaction& t2);
    static bool compareByAmount(const Transaction& t1, const Transaction& t2);
};

#endif //ADS_MIDTERM_PROJECT_TRANSACTION_H


