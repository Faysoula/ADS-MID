#include <iostream>
#include <string>
#include <filesystem>
#include "ForestTree.h"
#include <direct.h>

using namespace std;


void display_menu() {
    cout << "\nChart of Accounts Management System" << endl;
    cout << "=======================================" << endl;
    cout << "1. Add Account" << endl;
    cout << "2. Apply Transaction" << endl;
    cout << "3. Generate Account Report" << endl;
    cout << "4. Delete Transaction" << endl;
    cout << "5. Display Chart of Accounts" << endl;
    cout << "6. Search Account" << endl;
    cout << "0. Exit" << endl;
    cout << "\nEnter choice: ";
}

void ensure_reports_directory() {
    const string reportDir = "reports";
    if (_mkdir(reportDir.c_str()) == 0) {
        cout << "Created reports directory." << endl;
    }
    // If directory already exists, -1 is returned but that's okay
}

string getProjectPath() {
    string userProfile = getenv("USERPROFILE"); // Gets C:\Users\User
    return userProfile + "\\CLionProjects\\ADS-MID\\accountswithspace.txt";
}

int main() {
    ForestTree tree;

    ensure_reports_directory();

    // Build chart of accounts from a file
    tree.buildFromFile(getProjectPath());

    int choice;
    do {
        display_menu();
        cin >> choice;

        switch (choice) {
            case 1: {
                Account newAccount;
                int parentNumber;

                cout << "Enter account details (number description balance):\n";
                cin >> newAccount;

                cout << "Enter parent account number (-1 for root account): ";
                cin >> parentNumber;

                if (tree.addAccount(newAccount, parentNumber)) {
                    cout << "\nAccount added successfully." << endl;
                } else {
                    cout << "\nFailed to add account. Ensure the account number is unique and parent exists." << endl;
                }
                break;
            }
            case 2: {
                int accountNumber;
                cout << "Enter account number for transaction: ";
                cin >> accountNumber;

                // Create a transaction using the overloaded >> operator
                Transaction newTransaction;
                cin >> newTransaction;  // This will prompt for all transaction details

                if (tree.addTransaction(accountNumber, newTransaction)) {
                    // Save changes to file after successful transaction
                    try {
                        tree.saveToFile(getProjectPath());
                        tree.saveTransactions(tree.getTransactionFilename(getProjectPath()));
                        cout << "\nTransaction applied and saved successfully." << endl;
                    } catch (const exception &e) {
                        cerr << "Transaction applied but failed to save: " << e.what() << endl;
                    }
                } else {
                    cout << "Failed to apply transaction." << endl;
                }
                cout << "-----------------------------------";
                break;
            }
            case 3: {
                int accountNumber;
                string reportName;
                bool validInput = false;

                // Clear input buffer first
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                // Keep asking until valid input is received
                while (!validInput) {
                    cout << "Enter account number for report: ";
                    string input;
                    getline(cin, input);

                    // Try to convert input to integer
                    try {
                        // Check if input contains any non-digit characters
                        if (input.find_first_not_of("0123456789") != string::npos) {
                            cout << "Error: Account number must be a positive integer.\n";
                            continue;
                        }

                        accountNumber = stoi(input);
                        if (accountNumber <= 0) {
                            cout << "Error: Account number must be positive.\n";
                            continue;
                        }

                        // If we get here, input is valid
                        validInput = true;

                    } catch (const invalid_argument &) {
                        cout << "Error: Invalid account number format.\n";
                        continue;
                    } catch (const out_of_range &) {
                        cout << "Error: Account number is too large.\n";
                        continue;
                    }
                }

                // Now get report name
                bool validReportName = false;
                while (!validReportName) {
                    cout << "Enter report name (without extension): ";
                    getline(cin, reportName);

                    // Validate report name
                    if (reportName.empty() || reportName.find_first_of("<>:\"/\\|?*") != string::npos) {
                        cout << "Error: Invalid report name. Please avoid special characters.\n";
                        continue;
                    }

                    validReportName = true;
                }

                string outputPath = "reports/" + reportName + ".txt";
                try {
                    tree.printDetailedReport(accountNumber, outputPath);
                    cout << "Report generated successfully at: " << outputPath << endl;
                } catch (const runtime_error &e) {
                    cerr << "Error: " << e.what() << endl;
                }
                break;
            }
            case 4: {
                int accountNumber;
                cout << "Enter account number: ";
                cin >> accountNumber;

                NodePtr accountNode = tree.findAccount(accountNumber);
                if (accountNode) {
                    const vector<Transaction> &transactions = accountNode->getData().getTransactions();
                    if (transactions.empty()) {
                        cout << "No transactions found for this account.\n";
                        break;
                    }

                    cout << "\nTransactions for account " << accountNumber << ":\n";
                    for (int i = 0; i < transactions.size(); i++) {
                        cout << "Index " << i << ": Amount = " << transactions[i].getAmount()
                             << " (" << transactions[i].getDebitCredit() << ")\n";
                    }

                    int transactionIndex;
                    cout << "\nEnter index of transaction to delete (0-" << transactions.size() - 1 << "): ";
                    cin >> transactionIndex;

                    if (tree.deleteTransaction(accountNumber, transactionIndex)) {
                        // Save changes to file after successful deletion
                        try {
                            tree.saveToFile(getProjectPath());
                            tree.saveTransactions(tree.getTransactionFilename(getProjectPath()));
                            cout << "Transaction deleted and changes saved successfully.\n";
                        } catch (const exception &e) {
                            cerr << "Transaction deleted but failed to save changes: " << e.what() << endl;
                        }
                    } else {
                        cout << "Failed to delete transaction.\n";
                    }
                } else {
                    cout << "Account not found.\n";
                }
                break;
            }
            case 5: {
                cout << "\nDisplaying full chart of accounts:\n";
                tree.printForestTree();
                break;
            }
            case 6: {
                int accountNumber;
                cout << "Enter account number to search: ";
                cin >> accountNumber;

                NodePtr accountNode = tree.findAccount(accountNumber);
                if (accountNode) {
                    const Account &account = accountNode->getData();
                    cout << "\nAccount Found:" << endl;
                    cout << "Account Number: " << account.getAccountNumber() << endl;
                    cout << "Description: " << account.getDescription() << endl;
                    cout << "Balance: " << fixed << setprecision(2) << account.getBalance() << endl;
                } else {
                    cout << "Account not found for account number: " << accountNumber << endl;
                }
                break;
            }

            case 0:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice, try again.\n";
        }
    } while (choice != 0);

    return 0;
}
