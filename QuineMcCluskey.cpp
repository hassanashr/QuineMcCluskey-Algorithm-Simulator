#include"QuineMcCluskey.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<bitset>
#include<unordered_set>
#include<unordered_map>
#include<numeric>
#include<cmath>
using namespace std;
QuineMcCluskey::QuineMcCluskey(const std::string& filename)
{
readInput("TestCases/"+filename+".txt");
}
void QuineMcCluskey::readInput(const  string &filename)
{
ifstream file(filename);
if(!file)
{
    cerr<<"Error:Cannot open the file."<<endl;
    return;
}
string text;
getline(file, text);//reading number of variables
numVariables= stoi(text);
getline(file,text);// second line (minterms)
stringstream ss(text);
string term;
vector<int> MaxtermTemp;//to read the maxterms before changing them into minterms
bool MaxCase=false;
while(getline(ss,term, ','))
{
    if(term.size()>1)
    {
        if(term[0]=='M')
        {
MaxCase= true;
           if (isdigit(term[1]))
           MaxtermTemp.push_back(stoi(term.substr(1)) );
        }
    else if(term[0]=='m')
    {
        if (isdigit(term[1]))
        terms.push_back(Term(stoi(term.substr(1)),false));} //substr(1) to ignore the 'm'
    
    }

}
if(MaxCase)
{
for(int i=0;i<pow(2,numVariables);i++)// to add the values of the minterms and leave the maxterms.
{
    if (find(MaxtermTemp.begin(), MaxtermTemp.end(), i) == MaxtermTemp.end())
    terms.push_back(Term(i, false));

}

}
getline(file,text);//reading dont care terms
ss.clear();
ss.str(text);
while(getline(ss,term,','))
{

if(isdigit(term[1]))// to avoid the error of converting empty string to an integer (Not possible)
{
        terms.push_back(Term(stoi(term.substr(1)),true));

}

}
file.close();

}

string QuineMcCluskey:: toBinary(unsigned int num) const
{
    string binary = bitset<20>(num).to_string(); // Convert to 20-bit binary (algorithm supports up to 20 variables)
    return binary.substr(20 - numVariables); // Keep only required bits
}

bool QuineMcCluskey::differByOneBit(const string& a, const std::string& b) const
  
{
    int counter =0;
    for(int i=0; i<a.size();i++)
    {
        if(a[i]!= b[i])
        {
            counter++;
        }
        if(counter>1)
    {
        return false;
    }
    }
    
   return counter==1;
}

Implicant QuineMcCluskey::mergeImplicants(const Implicant& a, const Implicant& b) const {
    string newBinary = a.binary;
    int diffCount = 0;

    // Replace ALL differing bits with '-'
    for (int i = 0; i < newBinary.size(); i++) {
        if (a.binary[i] != b.binary[i]) {
            newBinary[i] = 'X';
            diffCount++;
        }
    }


    Implicant merged;
    merged.binary = newBinary;

    // Copy terms from both implicants
    merged.terms.insert(merged.terms.end(), a.terms.begin(), a.terms.end());
    merged.terms.insert(merged.terms.end(), b.terms.begin(), b.terms.end());

    return merged;
}



vector<Implicant> QuineMcCluskey::generatePrimeImplicants() {
    vector<vector<Implicant>> groups(numVariables + 1); // Grouping by number of 1s

    
    for (const auto& term : terms) {
        Implicant imp(term, numVariables);
        int onesCount = count(imp.binary.begin(), imp.binary.end(), '1');
        groups[onesCount].push_back(imp);
    }

    vector<Implicant> primeImplicants;
    bool mergedAny = true;

    // Iteratively merging implicants
    while (mergedAny) {
        mergedAny = false;
        vector<vector<Implicant>> newGroups(numVariables + 1); 
        unordered_set<string> mergedTracker; // Track merged implicants

        for (int i = 0; i < groups.size() - 1; ++i) {
            for (const auto& a : groups[i]) {
                for (const auto& b : groups[i + 1]) {
                    if (differByOneBit(a.binary, b.binary)) {
                        Implicant merged = mergeImplicants(a, b);
                        if (!merged.binary.empty()) { 
                            newGroups[i].push_back(merged);
                            mergedTracker.insert(a.binary);
                            mergedTracker.insert(b.binary);
                            mergedAny = true;
                        }
                    }
                }
            }
        }

        //  Move non-merged implicants to prime implicants
        for (const auto& group : groups) {
            for (const auto& imp : group) {
                if (mergedTracker.find(imp.binary) == mergedTracker.end()) {
                    primeImplicants.push_back(imp);
                }
            }
        }

        groups = std::move(newGroups); // Move new groups for next iteration
    }

    //  Remove duplicates
    sort(primeImplicants.begin(), primeImplicants.end());// sorting vector to bring duplicates together
    primeImplicants.erase(unique(primeImplicants.begin(), primeImplicants.end()), primeImplicants.end()); // use unique to shift duplicates to the end and return a pointer to the end of unique elements

    this->primeImplicants = primeImplicants;
    return primeImplicants;
}


void QuineMcCluskey::printPrimeImplicants() {
    vector<Implicant> PrimeImplicants = generatePrimeImplicants();

    for (const auto& imp : PrimeImplicants) {
        auto [mintermsCovered, dontCaresCovered] = imp.covers();

       
        cout << "Prime Implicant: " << binaryToExpression(imp.binary) << endl;

        // Print minterms
        cout << "Covers Minterms: ";
        for (unsigned int minterm : mintermsCovered) {
            cout << minterm << " ";
        }
        cout << endl;

        // Print don't cares
        cout << "Covers Don't Cares: ";
        for (unsigned int dontCare : dontCaresCovered) {
            cout << dontCare << " ";
        }
        cout << endl;

        cout << "----------------------" << endl;
    }
}
vector<Implicant> QuineMcCluskey::getEssentialPrimeImplicants() {
    unordered_map<unsigned int, vector<Implicant*>> mintermCoverage; //the key is the minterm number and the value here is the implicants covering this minterm
    vector<Implicant> essentialPrimeImplicants; //initializing a vector to store essential prime implicants in it in binary form


    for (auto& imp : primeImplicants) { //loop over the vector containing the the prime implicants
        auto [mintermsCovered, dontCaresCovered] = imp.covers(); //get the minterms covered by the current prime implicant
        for (unsigned int minterm : mintermsCovered) {
            mintermCoverage[minterm].push_back(&imp); //iterates over each minterm and adds pointer which enable us to track which implicants are covering those minterms
        }
    }

    unordered_set<string> essential_PrimeImplicant_Binary; // will be used to store the essential minterms in binary
    for (const auto& [minterm, implicants] : mintermCoverage) { //itterates over each minterm in mintermsCoverage
        if (implicants.size() == 1) {  //checks if the minterm is covered by only one prime implicant, in other words, columns containing only one cross
            Implicant* epi = implicants[0]; //since there is only one implicant, it must be at index 0 and this will be the essential prime implicant
            //this is to ensure that this prime implicant isn't already stored (avoiding duplicates)
            if (essential_PrimeImplicant_Binary.find(epi->binary) == essential_PrimeImplicant_Binary.end()) {
              //if it is not already stored, store it in the vector of epis and store its binary code in the set
              essentialPrimeImplicants.push_back(*epi); //store
                essential_PrimeImplicant_Binary.insert(epi->binary);
            }
        }
    }
    return essentialPrimeImplicants;
}
//coverts binary strings to expression
string QuineMcCluskey::binaryToExpression(const string& binary) const {
    string expression;
    if(binary== string (numVariables,'X'))
    {
        return "1";
    }
    for (int i = 0; i < binary.size(); ++i) {
        if (binary[i] == '1') {
            expression += char('A' + i);
        } else if (binary[i] == '0') {
            expression += char('A' + i);
            expression += "'";
        }
    }

    return expression;
}

void QuineMcCluskey::printEssentialPrimeImplicants() {
    vector<Implicant> EPIs = getEssentialPrimeImplicants();
    unordered_set<unsigned int> coveredMinterms;

    cout << "Essential Prime Implicants (EPIs): "<<endl;
    for (const auto& epi : EPIs) {
        cout << binaryToExpression(epi.binary) << endl;
        auto [minterms, dontCaresCovered] = epi.covers();
        coveredMinterms.insert(minterms.begin(), minterms.end());
    }

    cout<<endl;
    cout << "Minterms NOT covered by EPIs: "<<endl;
    for (const auto& term : terms) {
        if ((coveredMinterms.find(term.value) == coveredMinterms.end())&& !term.isDontCare ){// added this to remove dont-care terms
            cout << term.value << " ";
        }
    }
    cout << endl;
}
vector<string> QuineMcCluskey::solveAndMinimize() {
    vector<string> minimizedExpressions;
    vector<Implicant> essentialPIs = getEssentialPrimeImplicants();
    bool hasDontCares=false;// to handle 0 function case and only dont care case
    // Collect all minterms from the input
    unordered_set<unsigned int> allMinterms;
    for (const auto& term : terms) {
        if (!term.isDontCare) {
            allMinterms.insert(term.value);
        }
        else{
            hasDontCares= true;
        }
    }
    if(allMinterms.empty()&&hasDontCares)
    {
        minimizedExpressions.push_back("1");
        return minimizedExpressions;
    }
    else if(allMinterms.empty()&&!hasDontCares)
    {
        minimizedExpressions.push_back("0");
        return minimizedExpressions;
    }
    
    // Find which minterms are already covered by essential PIs
    unordered_set<unsigned int> coveredMinterms;
    for (const auto& epi : essentialPIs) {
        auto [minterms, dontCares] = epi.covers();
        for (unsigned int m : minterms) {
            coveredMinterms.insert(m);
        }
    }
    
    // Find minterms that still need to be covered
    unordered_set<unsigned int> uncoveredMinterms;
    for (unsigned int m : allMinterms) {
        if (coveredMinterms.find(m) == coveredMinterms.end()) {
            uncoveredMinterms.insert(m);
        }
    }
    
    // If all minterms are covered by essential PIs, we're done
    if (uncoveredMinterms.empty()) {
        // Create expression from essential PIs
        string expression;
        for (int i = 0; i < essentialPIs.size(); i++) {
            expression += binaryToExpression(essentialPIs[i].binary);
            if (i < essentialPIs.size() - 1) {
                expression += " + ";
            }
        }
        if (!expression.empty()) {
            minimizedExpressions.push_back(expression);
        }
        return minimizedExpressions;
    }
    
    // Otherwise, we need to find a minimal cover for remaining minterms
    vector<Implicant> remainingPIs;
    for (const auto& pi : primeImplicants) {
        // Skip essential PIs as they're already included
        bool isEssential = false;
        for (const auto& epi : essentialPIs) {
            if (pi.binary == epi.binary) {
                isEssential = true;
                break;
            }
        }
        if (!isEssential) {
            remainingPIs.push_back(pi);
        }
    }
    
    vector<vector<Implicant>> allMinimalCovers;
    
    // If there are uncovered minterms, find minimal covers
    if (!uncoveredMinterms.empty() && !remainingPIs.empty()) {
        vector<vector<Implicant>> allMinimalCovers=findMinimalCover(remainingPIs, uncoveredMinterms);

        for(int i=0;i<allMinimalCovers.size();i++)
        {
            
     // Combine essential PIs with minimal cover
        vector<Implicant> completeCover = essentialPIs;
        completeCover.insert(completeCover.end(), allMinimalCovers[i].begin(), allMinimalCovers[i].end());
         // Create expression from complete cover
         string expression;
         for (int i = 0; i < completeCover.size(); i++) {
             expression += binaryToExpression(completeCover[i].binary);
             if (i < completeCover.size() - 1) {
                 expression += " + ";
             }
         }
         if (!expression.empty()) {
             minimizedExpressions.push_back(expression);
         }
      else {
         // Only essential PIs are needed
         string expression;
         for (int i = 0; i < essentialPIs.size(); i++) {
             expression += binaryToExpression(essentialPIs[i].binary);
             if (i < essentialPIs.size() - 1) {
                 expression += " + ";
             }
         }
         if (!expression.empty()) {
             minimizedExpressions.push_back(expression);
         }
        } 
    }
       
    }
    
    return minimizedExpressions;
}

vector<vector<Implicant>> QuineMcCluskey::findMinimalCover(const vector<Implicant>& primeImplicants, const unordered_set<unsigned int>& mintermsTocover) {
    unordered_map<unsigned int, vector<int>> mintermToPIs;    // a map of which minterms are covered by which PIs

    for (int i = 0; i < primeImplicants.size(); i++) {
        auto [minterms, dontCares] = primeImplicants[i].covers();
        for (unsigned int m : minterms) {
            if (mintermsTocover.find(m) != mintermsTocover.end()) {
                mintermToPIs[m].push_back(i);
            }
        }
    }
    
    // Create the Product of Sums (POS) expression
    // Each sum term represents the PIs that cover a specific minterm
    vector<vector<int>> POS;
    for (const auto& [minterm, piIndices] : mintermToPIs) {
        POS.push_back(piIndices);
    }
    
    // Multiply out the POS to get Sum of Products (SOP)
    vector<unordered_set<int>> SOP;
    // Initialize with the first sum term
    if (!POS.empty()) {
        for (int pi : POS[0]) {
            unordered_set<int> product;
            product.insert(pi);
            SOP.push_back(product);
        }
        
        // Multiply with remaining sum terms
        for (int i = 1; i < POS.size(); i++) {
            vector<unordered_set<int>> newSOP;
            for (const auto& existingProduct : SOP) {
                for (int pi : POS[i]) {
                    unordered_set<int> newProduct = existingProduct;
                    newProduct.insert(pi);
                    newSOP.push_back(newProduct);
                }
            }
            SOP = newSOP;
        }
    }
    
    // Remove any product term that is a superset of another product term
    for (int i = 0; i < SOP.size(); i++) {
        for (int j = i + 1; j < SOP.size(); ) {
            bool removed = false;
            
            // Check if SOP[i] is a subset of SOP[j]
            if (includes(SOP[j].begin(), SOP[j].end(), SOP[i].begin(), SOP[i].end())) {
                SOP.erase(SOP.begin() + j);
                removed = true;
            }
            // Check if SOP[j] is a subset of SOP[i]
            else if (includes(SOP[i].begin(), SOP[i].end(), SOP[j].begin(), SOP[j].end())) {
                SOP.erase(SOP.begin() + i);
                i--;
                break;
            }
            
            if (!removed) j++;
        }
    }
    
    // Find the solution with minimum number of PIs
    int minSize = INT_MAX;
    vector<int> minIndicies;
    
    for (int i = 0; i < SOP.size(); i++) {
        if (SOP[i].size() < minSize) {
            minSize = SOP[i].size();
            minIndicies.clear();//remove previous covers
            minIndicies.push_back(i);
        }
        else if(SOP[i].size()==minSize)
        {
            minIndicies.push_back(i);
        }
    }
    
    // Convert the solution back to Implicants
    vector<vector<Implicant>> selectedPIs(minIndicies.size());
    if (!SOP.empty()) {
        for(int i=0;i<minIndicies.size();i++)
        {
        for (int piIndex : SOP[minIndicies[i]]) {
            selectedPIs[i].push_back(primeImplicants[piIndex]);
        }
    }
    }
    
    return selectedPIs;
}


void QuineMcCluskey::printMinimizedExpressions() {
    vector<string> expressions = solveAndMinimize();
    
    cout << "\nMinimized Boolean Expression(s):" << endl;
    if (expressions.empty()) {
        cout << "No solution found." << endl;
    } else {
        for (int i = 0; i < expressions.size(); i++) {
            cout << "Solution " << (i+1) << ": " << expressions[i] << endl;
        }
    }
}

string QuineMcCluskey::expressionToVerilog(const string& expression) const {
    if (expression == "1") return "1'b1";
    else if ( expression =="0")return "1'b0";
    vector<string> terms;
    int start = 0, end = 0;
    
    // Split the expression by '+' to get individual terms
    while ((end = expression.find(" + ", start)) != string::npos) {
        terms.push_back(expression.substr(start, end - start));
        start = end + 3;
    }
    terms.push_back(expression.substr(start));
    
    for (int i = 0; i < terms.size(); i++) {
        string& term = terms[i];
        string processedTerm;
        
        // Process each character
        for (int j = 0; j < term.length(); j++) {
            if (isalpha(term[j])) {
                if (!processedTerm.empty() && processedTerm.back() != '(' && processedTerm.back() != '&' && processedTerm.back() != '~') {
                    processedTerm += " & ";
                }
                processedTerm += term[j];
            } else if (term[j] == '\'') { // Handle negation
                processedTerm.insert(processedTerm.length() - 1, "~");
            }
        }
        
        // Wrap multi-variable terms in parentheses
        if (processedTerm.find("&") != string::npos) {
            processedTerm = "(" + processedTerm + ")";
        }
        terms[i] = processedTerm;
    }
    
    // Join terms with '|' (OR operator)
    return std::accumulate(terms.begin() + 1, terms.end(), terms[0], [](string a, const string& b) {
        return a + " | " + b;
    });
}

// Generates a Verilog module for the minimized Boolean expression
void QuineMcCluskey::generateVerilogModule(const string& moduleName) {
    vector<string> expressions = solveAndMinimize();
    
    if (expressions.empty()) {
        cout << "Cannot generate Verilog module: No solution found." << endl;
        return;
    }
    
    // Use the first solution for Verilog generation
    string verilogExpr = expressionToVerilog(expressions[0]);
    
    cout << "\nVerilog Module:" << endl;
    cout << "module " << moduleName << " (";
    
    if (verilogExpr == "1'b1" || verilogExpr == "1'b0") {
        cout << "output Y);" << endl;
        cout << "  assign Y = " << verilogExpr << ";" << endl;
    } else {
        // Generate input and output ports
        for (int i = 0; i < numVariables; i++) {
            cout << "input " << (char)('A' + i);
            if (i < numVariables - 1) cout << ", ";
        }
        cout << ", output Y);" << endl;
        
        // Assign the minimized expression to the output
        cout << "  assign Y = " << verilogExpr << ";" << endl;
    }
    cout << "endmodule" << endl;
}

// Generates a Verilog module using primitive gates for the minimized Boolean expression
void QuineMcCluskey::generatePrimitiveVerilogModule(const string& moduleName) {
    vector<string> expressions = solveAndMinimize();
    if (expressions.empty()) {
        cout << "Cannot generate Verilog module: No solution found." << endl;
        return;
    }

    string expression = expressions[0]; // Use the first solution for the Verilog module

    // Handle constant functions (0 or 1)
      if (expression == "0" || expression == "1") {
        cout << "module " << moduleName << " (Y);" << endl;
        cout << "  output Y;" << endl;
        if (expression == "0") {
            cout << "  supply0 gnd;" << endl;  // Primitive for logic 0
            cout << "  buf(Y, gnd);" << endl;  
        } else {
            cout << "  supply1 vcc;" << endl;  // Primitive for logic 1
            cout << "  buf(Y, vcc);" << endl;  
        }
        cout << "endmodule" << endl;
        return;
    }
    
    // Generate module header
    cout << "module " << moduleName << " (";
    for (int i = 0; i < numVariables; i++) {
        cout << (char)('A' + i);
        if (i < numVariables - 1) cout << ", ";
    }
    cout << ", Y);" << endl;

    // Declare input and output ports
    for (int i = 0; i < numVariables; i++) {
        cout << "  input " << (char)('A' + i) << ";" << endl;
    }
    cout << "  output Y;" << endl;

    // Split the expression into individual terms
    vector<string> terms;
    int pos = 0;
    while ((pos = expression.find(" + ")) != string::npos) {
        terms.push_back(expression.substr(0, pos));
        expression.erase(0, pos + 3);
    }
    terms.push_back(expression);

    // Declare wires for intermediate AND gates and NOT gates
    unordered_set<string> notWires;  
    for (int i = 0; i < terms.size(); i++) {
        cout << "  wire w" << i << ";" << endl;
        
        // Identify variables that require NOT gates
        for (int j = 0; j < terms[i].length(); j++) {
            if (isalpha(terms[i][j]) && j + 1 < terms[i].length() && terms[i][j + 1] == '\'') {
                string varName(1, terms[i][j]);
                if (notWires.find(varName) == notWires.end()) {
                    cout << "  wire not_" << varName << ";" << endl;
                    notWires.insert(varName);
                }
            }
        }
    }

    // Generate NOT gates for complemented variables
    for (const auto& varName : notWires) {
        cout << "  not(" << "not_" << varName << ", " << varName << ");" << endl;
    }

    // Generate AND gates for each term
    for (int i = 0; i < terms.size(); i++) {
        string term = terms[i];
        vector<string> inputs;
        
        // Parse each term to identify inputs
        for (int j = 0; j < term.length(); j++) {
            if (isalpha(term[j])) {
                string varName(1, term[j]);
                if (j + 1 < term.length() && term[j + 1] == '\'') {
                    inputs.push_back("not_" + varName); // Use NOT gate output for complemented variables
                    j++; 
                } else {
                    inputs.push_back(varName);
                }
            }
        }
        
        // Handle the case where a term has only one variable
    if (inputs.size() == 1) {
        cout << "  buf(w" << i << ", " << inputs[0] << ");" << endl;
    } else {
        cout << "  and(w" << i;
        for (const auto& input : inputs) {
            cout << ", " << input;
        }
        cout << ");" << endl;
    }
    }

    // Generate OR gate to combine all terms
    if (terms.size() == 1) {
    cout << "  buf(Y, w0);" << endl;
} else {
    cout << "  or(Y";
    for (size_t i = 0; i < terms.size(); i++) {
        cout << ", w" << i;
    }
    cout << ");" << endl;
}
    cout << "endmodule" << endl;
}
