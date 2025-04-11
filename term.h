#ifndef TERM_H
#define TERM_H
using namespace std;
class Term {
public:
    unsigned int value;
    bool isDontCare;
    Term(unsigned int v, bool dc = false);
    bool operator<(const Term& other) const;
    bool operator==(const Term& other) const;
};

#endif 