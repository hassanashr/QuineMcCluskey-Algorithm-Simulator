#ifndef QUINEMCCLUSKEY_H
#define QUINEMCCLUSKEY_H

#include <vector>
#include <string>
#include "Implicant.h"
#include"term.h"
#include <unordered_set>
using namespace std;
class QuineMcCluskey {
public:
    QuineMcCluskey(const string& filename);
    void readInput(const string& filename);
    vector<Implicant> generatePrimeImplicants();
    void printPrimeImplicants() ;
    void printEssentialPrimeImplicants();
vector<string> solveAndMinimize();
void printMinimizedExpressions();
void generateVerilogModule(const string& moduleName = "boolean_function");
void generatePrimitiveVerilogModule(const string& moduleName);
bool run = true;
private:
unsigned int numVariables;
std::vector<Term> terms; // minterms and dont-care terms
std::vector<Implicant> primeImplicants;
std::string toBinary(unsigned int num) const;
bool differByOneBit(const string &a, const std::string &b) const;        // check if two implicants are logically adjacent
Implicant mergeImplicants(const Implicant &a, const Implicant &b) const; // merging Implicants ie: minimizing logic to get to prime implicants
vector<Implicant> getEssentialPrimeImplicants();                         // gets the essential prime implicants in binary form
string binaryToExpression(const string &binary) const;                   // converts essential prime implicants in binary form to boolean expresssions
vector<vector<Implicant>> findMinimalCover(const vector<Implicant>& primeImplicants, const unordered_set<unsigned int>& mintermsTocover);
string expressionToVerilog(const string& expression) const;
};

#endif
