#include <iostream>
#include "fuzzifier.hh"
using namespace std;


int main() {
    Fuzzifier fuzzifier;

    fuzzifier.Init();
    fuzzifier.Fuzzify();

    cout << endl << "Finish Fuzzifying... Have a Fuzzy Day! " << endl << endl;

    return 0;
}
