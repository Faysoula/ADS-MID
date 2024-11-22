#include <iostream>
#include <string>
#include <filesystem>
#include "ForestTree.h"
#include <direct.h>

using namespace std;


void display_menu() {
    cout << "\nChart of Accounts Management System" << endl;
    cout << "1. Add Account\n" << endl;
    cout << "2. Apply Transaction\n" << endl;
    cout << "3. Generate Account Report\n" << endl;
    cout << "4. Delete Transaction\n" << endl;
    cout << "0. Exit\n" << endl;
    cout << "Enter choice: " << endl;
}

void ensure_reports_directory() {
    const string reportDir = "reports";
    if (_mkdir(reportDir.c_str()) == 0) {
        cout << "Created reports directory." << endl;
    }
    // If directory already exists, -1 is returned but that's okay
}

int main() {
    ForestTree tree;

    ensure_reports_directory();

    // Build chart of accounts from a file
    tree.buildFromFile("C:/Users/User/CLionProjects/ADS-MID/accountswithspace.txt");

    int choice;
    do {
        display_menu();
        cin >> choice;

        switch (choice) {
            case 1: {
                int accountNumber, parentNumber;
                string description;
                cout << "Enter account number: ";
                cin >> accountNumber;
                cout << "Enter description: ";
                cin.ignore();
                getline(cin, description);
                cout << "Enter parent account number (-1 for root account): ";
                cin >> parentNumber;

                if (tree.addAccount(accountNumber, description, parentNumber)) {
                    cout << "Account added successfully.\n";
                } else {
                    cout << "Failed to add account. Ensure the account number is unique and parent exists.\n";
                }
                break;
            }
            case 2: {
                int accountNumber;
                double amount;
                string type;
                cout << "Enter account number for transaction: ";
                cin >> accountNumber;
                cout << "Enter amount: ";
                cin >> amount;
                cout << "Enter type (D for debit, C for credit): ";
                cin >> type;

                if (type != "D" && type != "C") {
                    cout << "Invalid transaction type. Use 'D' for debit or 'C' for credit.\n";
                    break;
                }

                if (tree.addTransaction(accountNumber, amount, type)) {
                    cout << "Transaction applied successfully.\n";
                } else {
                    cout << "Failed to apply transaction. Ensure the account exists.\n";
                }
                break;
            }
            case 3: {
                int accountNumber;
                string reportName;
                cout << "Enter account number for report: ";
                cin >> accountNumber;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Enter report name (without extension): ";
                getline(cin, reportName);

                string outputPath = "reports/" + reportName + ".txt";
                try {
                    tree.printDetailedReport(accountNumber, outputPath);
                    cout << "Report generated successfully at: " << outputPath << endl;
                } catch (const runtime_error &e) {
                    cerr << "Error: " << e.what() << endl;
                }
                break;
            }
//            case 4: {
//                int accountNumber, transactionId;
//                cout << "Enter account number: ";
//                cin >> accountNumber;
//                cout << "Enter transaction ID to delete: ";
//                cin >> transactionId;
//
//                if (tree.deleteTransaction(accountNumber, transactionId)) {
//                    cout << "Transaction deleted successfully.\n";
//                } else {
//                    cout << "Failed to delete transaction. Ensure the transaction exists in the specified account.\n";
//                }
//                break;
//            }
            case 0:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice, try again.\n";
        }
    } while (choice != 0);

    return 0;
}
