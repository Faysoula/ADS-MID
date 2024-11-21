#include <iostream>
#include <string>
#include "ForestTree.h"

using namespace std;

void display_menu() {
    cout << "1. Add Account\n";
    cout << "2. Apply Transaction\n";
    cout << "3. Generate Account Report\n";
    cout << "4. Delete Transaction\n";
    cout << "0. Exit\n";
    cout << "Enter choice: ";
}

int main() {
    ForestTree tree;

    // Build chart of accounts from a file
    tree.buildFromFile("accountswithspace.txt");

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
                string outputFilename;
                cout << "Enter account number for report: ";
                cin >> accountNumber;
                cout << "Enter output filename: ";
                cin >> outputFilename;

                tree.printDetailedReport(accountNumber, outputFilename);
                cout << "Report generated: " << outputFilename << endl;
                break;
            }
            case 4: {
                int accountNumber, transactionId;
                cout << "Enter account number: ";
                cin >> accountNumber;
                cout << "Enter transaction ID to delete: ";
                cin >> transactionId;

                if (tree.deleteTransaction(accountNumber, transactionId)) {
                    cout << "Transaction deleted successfully.\n";
                } else {
                    cout << "Failed to delete transaction. Ensure the transaction exists in the specified account.\n";
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
