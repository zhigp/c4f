#include <iostream>
using namespace std;


// Autoassociative network, NO asynchronous update

const size_t kDimention = 4;

class Neuron {
public:
    Neuron() {};
    Neuron(int *);

    int     Process(int *);

    void    set_activation(int dproduct ) { activation_ = dproduct; };
    int     get_activation() { return activation_; };

private:
    int activation_;
    int weightv_[kDimention];
};


class Hopfield {
public:
    Hopfield(int *, int *, int *, int *);   // parameter num need to be the same as kDimention

    void    Activate(int *);
    int     Threshold(int);

    int     get_output(int i) { return output_[i]; };

private:
    Neuron  neuron_[kDimention];
    int     output_[kDimention];
};

