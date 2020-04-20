#include "hopfield.hh"


Neuron::Neuron(int *wt) {
    for(int i=0; i<kDimention; i++)
        weightv_[i] = *(wt+i);
}

int Neuron::Process(int *x) {
    int result = 0;

    for(int i=0; i<kDimention; i++)
        result += x[i] * weightv_[i];

    set_activation(result);

    return result;
}



Hopfield::Hopfield(int *a, int *b, int *c, int *d) {
    neuron_[0] = Neuron(a);
    neuron_[1] = Neuron(b);
    neuron_[2] = Neuron(c);
    neuron_[3] = Neuron(d);
}

void Hopfield::Activate(int *pattern) {
    cout << "Neuron\t" << "Activation\t" << "Output" << endl;
    for(int i=0; i<kDimention; i++) {
        neuron_[i].Process(pattern);
        output_[i] = Threshold(neuron_[i].get_activation());
        cout << "   " << i << "\t   " << neuron_[i].get_activation() << "\t\t   " << output_[i] << endl;
    }
}

int Hopfield::Threshold(int k) {
    if(k>=0)
        return 1;
    return 0;
}
