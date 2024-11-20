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

class Account {
private:
    int accountNumber;
    string description;
    double balance;
    vector<Transaction> transactions;

public:
    // Constructors & Destructor
    Account();  // Default constructor
    Account(int num, const string& desc, double bal);  // Constructor with params
    Account(const Account& acc);  // Copy constructor
    ~Account();  // Destructor

    // Getters
    int getAccountNumber() const { return accountNumber; }
    string getDescription() const { return description; }
    double getBalance() const { return balance; }
    const vector<Transaction>& getTransactions() const { return transactions; }

    // Setters
    void setAccountNumber(int num) { accountNumber = num; }
    void setDescription(const string& desc) { description = desc; }
    void setBalance(double bal) { balance = bal; }

    // Operations
    void addTransaction(const Transaction& t);
    void removeTransaction(int index);
    void updateBalance(const Transaction& t);
    string getShortDescription() const;
    bool isParentOf(int otherAccountNum) const;

    // Operators
    ostream& operator<<(istream& os, const Account& account);
    istream& operator>>(istream& is, Account& account);
};


#endif //ACCOUNT_H
