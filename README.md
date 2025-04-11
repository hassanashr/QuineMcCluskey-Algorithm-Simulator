# Quine-McCluskey Logic Minimization Simulator

## Team Members
- Hassan Ashraf
- Yousef Elmenshawi
- Ahmed Soliman

## Overview
This project implements the Quine-McCluskey algorithm to minimize Boolean functions. The algorithm is a tabular method used for simplifying Boolean expressions, commonly employed in digital design for reducing logic circuit complexity. The program reads Boolean expressions, computes prime implicants, identifies essential prime implicants, and then generates the minimal Boolean expression. Additionally, it can output the minimized expression in Verilog format for hardware design.

## Features
- **Boolean Expression Input**: Accepts minterms, maxterms, and don't cares.
- **Prime Implicant Generation**: Computes all prime implicants and displays them.
- **Essential Prime Implicants (EPIs)**: Identifies essential prime implicants that cover unique minterms.
- **Minimal Cover Generation**: Reduces the expression to its minimal form by selecting the smallest set of implicants.
- **Verilog Code Generation**: Converts the minimized Boolean expression to Verilog code for hardware synthesis.
- **Test Cases**: Includes 10 test cases to verify the correctness of the implementation.

## How to Use?

### Compile the Program:
- The project is implemented in C++. Compile using your preferred C++ compiler (e.g., g++, clang++, or an IDE like CLion).
- Make sure all necessary source files (e.g., QuineMcCluskey.cpp, Implicant.cpp, Term.cpp) are included in the build.

### Run the Program:
- Once compiled, run the program.
- The program will prompt you to enter the name of a test case. Each test case contains a predefined Boolean expression.
- The results (prime implicants, essential prime implicants, minimized Boolean expression, and Verilog output) will be printed in the terminal.

### Output:
For each test case, the program will print:
- The list of prime implicants.
- The essential prime implicants.
- The minimized Boolean expression.
- The generated Verilog code.

## Data Structures Used
- **Vectors**: Used to store collections of prime implicants, terms, and Boolean expressions.
- **Unordered Maps**: Used for mapping minterms to the prime implicants that cover them.
- **Unordered Sets**: Used to store essential prime implicants and ensure no duplicates.
- **Strings**: Used for representing Boolean expressions and their binary forms.

## Algorithms
- **Quine-McCluskey Algorithm**: The core algorithm for minimizing Boolean expressions by finding prime implicants and essential prime implicants.
- **Greedy Algorithm**: Used to select the minimal set of implicants that cover all the required minterms.
- **Set Operations**: Operations such as set inclusion and difference are used to identify redundant terms and reduce the Boolean expression.

## Input Format
The input is provided in a text file with the following structure:
- First line: An integer (1 ≤ 𝑁 ≤ 20), representing the number of variables in the Boolean function.
- Second line: A comma-separated list of minterms (indicated by m) or maxterms (indicated by M).
- Third line: A comma-separated list of don't-care terms (indicated by d).

## Requirements
- C++ Compiler (e.g., GCC, Clang)
- C++11 or higher for modern language features.

## Files
- **QuineMcCluskey.h**: Header file for the Quine-McCluskey algorithm.
- **QuineMcCluskey.cpp**: Implementation of the Quine-McCluskey algorithm.
- **Implicant.h**: Header file for Implicant data structure.
- **Implicant.cpp**: Implementation of Implicant data structure.
- **Term.h**: Header file for Term data structure.
- **Term.cpp**: Implementation of Term data structure.
- **main.cpp**: Main entry point to run the program and test cases.

## Test Cases
The project includes the following test cases:
1. **Test Case 1**: Function with multiple minimal coverage solution.
2. **Test Case 2**: Function with only don't care terms.
3. **Test Case 3**: Function with only one variable and single minterm.
4. **Test Case 4**: Function with no essential prime implicants.
5. **Test Case 5**: Function with no minterms and no dont care terms (0).
6. **Test Case 6**: Function with maxterms.
7. **Test Case 7**: Function with a single maxterm.
8. **Test Case 8**: Function with overlapping minterms and dont care terms.
9. **Test Case 9**: Function with all minterms Case.
10. **Test Case 10**: Function with 20 variables (Edge Case).

## Contributions
If you wish to help with the development of the project, create a pull request, and send an email to: hassan.ashraf@aucegypt.edu to faciliate the integration of your additions to the codebase.
