#include "QuineMcCluskey.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void runTest(const string& filename) {
    QuineMcCluskey qm(filename);
    if(qm.run) {
        cout << "\n=== Running: " << filename << " ===\n";
        cout << "\n=== Prime Implicants ===\n";
        qm.printPrimeImplicants();

        cout << "\n=== Essential Prime Implicants ===\n";
        qm.printEssentialPrimeImplicants();

        cout << "\n=== Minimized Boolean Expression ===\n";
        qm.printMinimizedExpressions();

        cout << "\n=== Verilog Module ===\n";
        qm.generateVerilogModule("Function");

        cout << "\n=== Primitive Verilog Module ===\n";
        qm.generatePrimitiveVerilogModule("Function_Primitive");
    }
}

int main() {
    vector<string> testFiles = {
        "0minterms and 0dont-cares",
        "All_Minterms_Case",
        "Maxterms Case",
        "Multiple_Solutions",
        "No Essential Prime Implicants Case",
        "Only Dont-Care terms Case",
        "Overlapping Terms Case",
        "Single Maxterm Case",
        "High Variable Number Case",
        "Single Variable and Single Minterm Case"
    };
    while (true) {
        cout << "Choose an option:\n";
        cout << "1. Run all tests\n";
        cout << "2. Select a test to run\n";
        cout << "3. Exit\n";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            for (const string& file : testFiles) {
                runTest(file);
            }
        } else if (choice == 2) {
            cout << "Select a test by number:\n";
            for (size_t i = 0; i < testFiles.size(); ++i) {
                cout << i + 1 << ". " << testFiles[i] << "\n";
            }
            int testChoice;
            cin >> testChoice;
            cin.ignore();

            if (testChoice >= 1 && testChoice <= testFiles.size()) {
                runTest(testFiles[testChoice - 1]);
            } else {
                cout << "Invalid selection.\n";
            }
        } else if (choice == 3) {
            cout << "Are you sure you want to exit? (y/n): ";
            char confirm;
            cin >> confirm;
            cin.ignore();
            if (confirm == 'y' || confirm == 'Y') {
                cout << "Exiting program.\n";
                break;
            }
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}
