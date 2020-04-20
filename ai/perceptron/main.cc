#include <iostream>
#include "perceptron.hh"
using namespace std;


float   inputv1[kDimention] = { 1.95, 0.27, 0.69, 1.25 };
float   inputv2[kDimention] = { 0.30, 1.05, 0.75, 0.19 };

float   wtv1[kDimention] = { 2, 3, 3, 2 };
float   wtv2[kDimention] = { 3, 0, 6, 2 };

float   threshold = 7.0;



int main() {

    Perceptron  pcpt1(wtv1);

    pcpt1.Activate(inputv1);
    pcpt1.Threshold(threshold);

    cout << endl << "Perceptron 1:" << endl;
    cout << "Activation:\t" << pcpt1.get_nout_activation() << endl;
    cout << "Output:\t\t" << pcpt1.get_nout_output() << endl << endl;


    Perceptron  pcpt2(wtv2);

    pcpt2.Activate(inputv2);
    pcpt2.Threshold(threshold);

    cout << "Perceptron 2:" << endl;
    cout << "Activation:\t" << pcpt2.get_nout_activation() << endl;
    cout << "Output:\t\t" << pcpt2.get_nout_output() << endl << endl;

    return 0;
}
