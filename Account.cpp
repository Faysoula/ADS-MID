//
// Created by MayaH on 11/19/2024.
//

#include "Account.h"
#include <iostream>
#include <iomanip>

using namespace std;

Account::Account() : accountNumber(0), description(""), balance(0.0) {}

Account::Account(int num, const string &desc, double bal) {
    accountNumber = num;
    description = desc;
    balance = bal;
}

Account::Account(const Account &acc) {
    accountNumber = acc.accountNumber;
    description = acc.description;
    balance = acc.balance;
    transactions = acc.transactions;
}

Account::~Account() {}

int Account::getAccountNumber() const {
    return accountNumber;
}

string Account::getDescription() const {
    return description;
}

double Account::getBalance() const {
    return balance;
}

const vector<Transaction> &Account::getTransactions() const {
    return transactions;
}

int Account::getTransactionCount() const {
    return transactions.size();
}

Transaction Account::getTransaction(int index) const {
    if (index >= 0 && index < transactions.size()) {
        return transactions[index];
    }
    throw out_of_range("Transaction index out of range :)");
}

void Account::setAccountNumber(int num) {
    accountNumber = num;
}

void Account::setDescription(const string &desc) {
    description = desc;
}

void Account::setBalance(double bal) {
    balance = bal;
}

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

void Account::addTransaction(const Transaction &t) {
    transactions.push_back(t);
    updateBalance(t);
}

void Account::removeTransaction(int index) {
    if (index >= 0 && index < transactions.size()) {
        Transaction t = transactions[index];
        if (t.getDebitCredit() == 'D') {
            balance -= t.getAmount();
        } else {
            balance += t.getAmount();
        }
        transactions.erase(transactions.begin() + index);
    }
}

void Account::updateBalance(const Transaction &t) {
    if (t.getDebitCredit() == 'D') {
        balance += t.getAmount();
    } else if (t.getDebitCredit() == 'C') {
        balance -= t.getAmount();
    }
}

string Account::getShortDescription() const {
    return description.length() > 10 ? description.substr(0, 10) : description;
}

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

istream &operator>>(istream &is, Account &account) {
    string line;
    if (getline(is, line)) {
        istringstream iss(line);

        //read acc number
        int accNum;
        account.setAccountNumber(accNum);

        //skip whitespace
        iss >> ws;

        string desc;
        string word;
        double balance;

        while (iss >> word) {
            //creates a new string stream containing the curr word
            istringstream balanceCheck(word);

            //extract a double from the string stream
            if (balanceCheck >> balance) {
                account.setBalance(balance);
                break;
            }
            if (!desc.empty())
                desc += " ";
            desc += word;
        }
        account.setDescription(desc);
    }
    return is;
}
