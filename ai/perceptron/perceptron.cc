#include "perceptron.hh"


void NeuronOut::Activate(float *inputv, NeuronIn *neuronin) {

    activation_ = 0.0;

    for(int i=0; i<kDimention; i++) {
        neuronin[i].Activate(inputv[i]);
        activation_ += neuronin[i].get_activation();
    }
}


int NeuronOut::Threshold(float criteria) {
    if(activation_>=criteria)
        output_ = 1;
    else
        output_ = 0;

    return output_;
}



Perceptron::Perceptron(float *weightv) {
    for(int i=0; i<kDimention; i++)
        neuronin_[i] = NeuronIn(weightv[i]);

    neuronout_ = NeuronOut();
    neuronout_.set_activation(0.0);
    neuronout_.set_output(0);
}

void Perceptron::Activate(float *inputv) {
    neuronout_.Activate(inputv, neuronin_);
}

void Perceptron::Threshold(float criteria) {
    neuronout_.Threshold(criteria);
}

float Perceptron::get_nout_activation() {
    return neuronout_.get_activation();
}

int Perceptron::get_nout_output() {
    return neuronout_.get_output();
}
