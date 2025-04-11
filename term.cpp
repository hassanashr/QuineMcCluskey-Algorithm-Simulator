#include "Term.h"
using namespace std;
Term::Term(unsigned int v, bool dc) : value(v), isDontCare(dc) {}

bool Term::operator<(const Term& other) const {
    return value < other.value;
}
bool Term::operator==(const Term& other) const {
    return value == other.value;
}
