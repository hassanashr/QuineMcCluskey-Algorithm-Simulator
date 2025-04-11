#include "Implicant.h"
using namespace std;
#include<bitset>
Implicant::Implicant()
{}
Implicant::Implicant(const Term& term, unsigned int numVariables) 
{
    terms.push_back(term); //Store the first minterm
    binary = bitset<20>(term.value).to_string().substr(20 - numVariables); //Convert to binary
    isEssential = false; //
}

bool Implicant::operator<(const Implicant& other) const {
    return binary < other.binary;
}

bool Implicant::operator==(const Implicant& other) const {
    return binary == other.binary;
}
// Split terms vector into  two vectors: minterms covered and don't-cares covered
pair<vector<unsigned int>, vector<unsigned int>> Implicant::covers() const {
    vector<unsigned int> coveredMinterms;
    vector<unsigned int> coveredDontCares;

    for (const Term& t : terms) {
        if (t.isDontCare) {
            coveredDontCares.push_back(t.value);
        } else {
            coveredMinterms.push_back(t.value);
        }
    }

    return {coveredMinterms, coveredDontCares};
}

