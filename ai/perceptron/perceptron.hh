#include <cstdlib>

// 2 layer perceptron:
// 1 layer input neurons, 1 layer output neuron;
// 1 group weights;



const size_t kDimention = 4;

class NeuronIn {
public:
    NeuronIn() {};
    NeuronIn(float wt):weight_(wt) {};

    void    Activate(float in) { activation_ = in*weight_; };

    float   get_weight() { return weight_; };
    float   get_activation() { return activation_; };

private:
    float   weight_;
    float   activation_;
};


class NeuronOut {
public:
    NeuronOut() {};

    void    Activate(float *inputv, NeuronIn *neuronin);
    int     Threshold(float criteria);

    float   get_activation() { return activation_; };
    void    set_activation(float val) { activation_ = val; };
    int     get_output() { return output_; };
    void    set_output(int val) { output_ = val; };

private:
    float   activation_;
    int     output_;
};


class Perceptron {
public:
    Perceptron(float *weightv);

    void    Activate(float *inputv);
    void    Threshold(float criteria);

    float   get_nout_activation();
    int     get_nout_output();

private:
    NeuronIn    neuronin_[kDimention];
    NeuronOut   neuronout_;
};



