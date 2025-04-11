#ifndef IMPLICANT_H
#define IMPLICANT_H

#include <vector>
#include <string>
#include <algorithm>
#include"term.h"
using namespace std;

class Implicant {
public:
vector<Term> terms; 
    string binary;
    bool isEssential;
    Implicant();
    Implicant(const Term & terms, unsigned int numVariables);
    
    bool operator<(const Implicant& other) const;
    bool operator==(const Implicant& other) const;
    
    // Returns Covered Minterms and Dont Cares by an Implicant
    pair<vector<unsigned int>, vector<unsigned int>> covers() const ;

};

#endif 