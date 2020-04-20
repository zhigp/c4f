#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include "backpropagation.hh"
using namespace std;


const string tab("\t\t");
const string spt("  ");
const string delmt("--------------------------------------------------------------------------------");
const string delmt2("------------------------------------------------");


float Squash(float input) {
    // squashing function: using sigmoid

    if(input<-50)
        return 0.0;

    if(input>50)
        return 1.0;

    return (float) (1.0/(1.0+exp(-input)));
}


float RandF(bool init) {
    // generate a random float in [-1,1];
    int num;

    if(init)
        srand((unsigned)time(NULL));

    num = rand() % 101;

    return ( 2.0*(num/100.00)-1.0 );
}



//
// Layer In
//
LayerIn::LayerIn(size_t nin, size_t nout) {

    numin = nin;
    numout = nout;

    inputs = nullptr;
    outputs = new float[nout];

    weights = nullptr;
    errors = nullptr;
    errors_bp = nullptr;
}

LayerIn::~LayerIn() {

    delete []outputs;
}


void LayerIn::ListOutputs() {

    cout << tab;
    for(int i=0; i<numout; i++)
        cout << outputs[i] << spt;
    cout << endl;
}



//
// Layer Middle
//
LayerMid::LayerMid(size_t nin, size_t nout) {

    numin = nin;
    numout = nout;

    inputs = nullptr;
    outputs = new float[nout];

    weights = new float[nin*nout];      // weights[in][out]
    errors = nullptr;                   // errors -> errors_bp(next layer) for Middle Layer
    errors_bp = new float[nin];
}

LayerMid::~LayerMid() {

    delete []outputs;

    delete []weights;
    delete []errors_bp;
}


void LayerMid::CalculateOuts() {
    float sum = 0.0;

    for(int i=0; i<numout; i++) {
        for(int j=0; j<numin; j++)
            sum += inputs[j] * weights[j*numout+i];
        outputs[i] = Squash(sum);
        sum = 0.0;
    }
}

void LayerMid::CalculateErrors() {
    float err = 0.0;

    for(int i=0; i<numin; i++) {
        for(int j=0; j<numout; j++)
            err += weights[i*numout+j] * errors[j];
        errors_bp[i] = err;
        err = 0.0;
        errors_bp[i] *= (inputs[i]*(1.0-inputs[i]));
    }
}


void LayerMid::ListOutputs() {

    cout << tab;
    for(int i=0; i<numout; i++)
        cout << outputs[i] << spt;
    cout << endl;
}

void LayerMid::ListErrors() {
    int i;

    cout << tab;
    for(i=0; i<numout; i++)
        cout << errors[i] << spt;
    cout << "|" << spt;
    for(i=0; i<numin; i++)
        cout << errors_bp[i] << spt;
    cout << endl;
}

void LayerMid::ListWeights() {

    for(int i=0; i<numin; i++) {
        cout << tab;
        for(int j=0; j<numout; j++)
            cout << setw(kfWidth) << weights[i*numout+j];
        cout << endl;
    }
}


void LayerMid::RandomizeWeights() {

    RandF(true);    // only set the seed

    for(int i=0; i<numin; i++)
        for(int j=0; j<numout; j++)
            weights[i*numout+j] = RandF(false);
}

void LayerMid::UpdateWeights(float beta) {

    for(int i=0; i<numin; i++)
        for(int j=0; j<numout; j++)
            weights[i*numout+j] += beta * errors[j] * inputs[i];
}



//
// Layer Out
//
LayerOut::LayerOut(size_t nin, size_t nout) {

    numin = nin;
    numout = nout;

    inputs = nullptr;
    outputs = new float[nout];

    weights = new float[nin*nout];      // weights[in][out]
    errors = new float[nout];
    errors_bp = new float[nin];

    exp_outputs = new float[nout];
    error_sum = 0.0;
}

LayerOut::~LayerOut() {

    delete []outputs;

    delete []weights;
    delete []errors;
    delete []errors_bp;

    delete []exp_outputs;
}


void LayerOut::CalculateOuts() {
    float sum = 0.0;

    for(int i=0; i<numout; i++) {
        for(int j=0; j<numin; j++)
            sum += inputs[j] * weights[j*numout+i];
        outputs[i] = Squash(sum);
        sum = 0.0;
    }
}

void LayerOut::CalculateErrors() {
    int i,j;
    float err = 0.0, errsum=0.0;

    for(i=0; i<numout; i++) {
        errors[i] = exp_outputs[i] - outputs[i];
        errsum += errors[i];
    }

    for(i=0; i<numin; i++) {
        for(j=0;j<numout;j++)
            err += weights[i*numout+j] * errors[j];
        errors_bp[i] = err;
        err = 0.0;
        errors_bp[i] *= (inputs[i]*(1.0-inputs[i]));
    }

    error_sum = errsum;
}


void LayerOut::ListOutputs() {

    cout << tab;
    for(int i=0; i<numout; i++)
        cout << outputs[i] << spt;
    cout << endl;
}

void LayerOut::ListErrors() {
    int i;

    cout << tab;
    for(i=0; i<numout; i++)
        cout << errors[i] << spt;
    cout << "|" << spt;
    for(i=0; i<numin; i++)
        cout << errors_bp[i] << spt;
    cout << endl;
}

void LayerOut::ListWeights() {

    for(int i=0; i<numin; i++) {
        cout << tab;
        for(int j=0; j<numout; j++)
            cout << setw(kfWidth) << weights[i*numout+j];
        cout << endl;
    }
}


void LayerOut::RandomizeWeights() {

    RandF(true);      // only set the seed

    for(int i=0; i<numin; i++)
        for(int j=0; j<numout; j++)
            weights[i*numout+j] = RandF(false);
}

void LayerOut::UpdateWeights(float beta) {

    for(int i=0; i<numin; i++)
        for(int j=0; j<numout; j++)
            weights[i*numout+j] += beta * errors[j] * inputs[i];
}



//
// Network BP
//
NetworkBP::NetworkBP() : buf_nptns(0),
                         num_ptns(0),
                         cycles(0),
                         e_ss_cycle(0.0),
                         e_s_cycle(0.0),
                         e_rms_ptn(101.0),
                         e_ss_total(0.0),
                         e_s_total(0.0),
                         e_rms_cycle(0.0) {

    cout << endl << delmt << endl
         << "\t\t\t_____ BP Neural Network Simulator _____"
         << endl << delmt << endl << endl;

    init();
}

NetworkBP::~NetworkBP() {

    if(mode) {
        fs_train.close();
        fs_weights.close();
    } else {
        fs_test.close();
        fs_weights.close();
        fs_results.close();
    }

    for(int i=0; i<num_layers; i++)
        delete layerptr[i];

    delete []buffer;
}


void NetworkBP::Run() {
    int i = 0;
    float tmp = 0.0;

    if(mode) {
    // Training Mode
        RandomizeWeights();

        while(cycles<cycles_max && e_rms_ptn>error_tolerance) {

            e_ss_cycle = 0.0;

            fs_train.clear();
            fs_train.seekg(fs_train.beg);

            while(!fs_train.eof()) {
                BufferData(fs_train);
                for(i=0; i<buf_nptns; i++) {
                    FeedNeurons(i);
                    FeedForward();
                    PropBackward();
                    UpdateWeights();

                    tmp = dynamic_cast<LayerOut *>(layerptr[num_layers-1])->error_sum;
                    e_ss_cycle += tmp * tmp;
                }
            } // end of one cycle

            cycles++;
            e_s_cycle = sqrt(e_ss_cycle);
            e_rms_ptn = sqrt(e_ss_cycle/num_ptns);
            e_ss_total += e_ss_cycle;
            e_s_total = sqrt(e_ss_total);
            e_rms_cycle = sqrt(e_ss_total/cycles);

            StatsCycle();
        } // end of loop in cycles

        StatsTotal();
        WriteWeights();
    } else {
    // Testing Mode
        ReadWeights();

        while(!fs_test.eof()) {
            BufferData(fs_test);
            for(i=0; i<buf_nptns; i++) {
                FeedNeurons(i);
                FeedForward();
                WriteOutputs();
            }
        } // end of testing mode cycle, testing mode runs only 1 cycle

        cout << delmt << endl << endl;
    } // end of Testing Mode Run
}


void NetworkBP::ListOutputs() {

    cout << "outputs of the output layer(cycle:" << cycles << "):" << endl;
    layerptr[num_layers-1]->ListOutputs();
}

void NetworkBP::ListErrors() {

    cout << "errors of the output layer(cycle:" << cycles << "):" << endl;
    layerptr[num_layers-1]->ListErrors();
}

void NetworkBP::ListWeights() {

    cout.setf(ios::fixed);
    cout.precision(kfPrecision);

    cout << "Weights[in][out] of all layers(cycle:" << cycles << "):" << endl;
    for(int i=1; i<num_layers; i++) {
        cout << "\tlayer id: " << i << endl;
        layerptr[i]->ListWeights();
    }
}


// network internal operations
void NetworkBP::init() {

    do {
        cout << "select the running mode ( 1 :Training, 0 :Testing ):\t";
        cin.clear();
        cin >> mode;
    } while(cin.fail());

    // get learning rate, error tolerance, max_cycles
    if(mode) {
        cout << endl << delmt << endl << "\t\t\t Running In Training Mode " << endl << delmt << endl;

        do {
            cout << "enter the learning rate [0.01,1.00]:\t\t\t";
            cin >> beta;
        } while(beta<0.00999999 || beta>1.00);

        do {
            cout << "enter the error tolerance [0.001,100.00]:\t\t";
            cin >> error_tolerance;
        } while(error_tolerance<0.001 || error_tolerance>100.00);

        do {
            cout << "enter the max cycles (>4, try 800):\t\t\t";
            cin >> cycles_max;
        } while(cycles_max<5);

        fs_train.open(kFnTrain);
        fs_weights.open(kFnWeights,ios::out);
        fs_trainstats.open(kFnTrainStats);
        if(fs_train.fail()) {
            cout << "could not open file \"" << kFnTrain << "\" for read!" << endl;
            exit(1);
        }
        if(fs_train.peek()==EOF) {
            cout << "no data in file " << kFnTrain << "!" << endl;
            exit(2);
        }


    } else {
        cout << endl << delmt << endl << "\t\t\t Running In Testing Mode" << endl << delmt << endl;

        fs_test.open(kFnTest);
        fs_weights.open(kFnWeights,ios::in);
        fs_results.open(kFnResults);
        if(fs_test.fail()) {
            cout << "could not open file \"" << kFnTest << "\" for read!" << endl;
            exit(1);
        }
        if(fs_weights.fail()) {
            cout << "could not open file \"" << kFnWeights << "\" for read!" << endl;
            exit(1);
        }
        if(fs_test.peek()==EOF) {
            cout << "no data in file " << kFnTest << "!" << endl;
            exit(2);
        }
        if(fs_weights.peek()==EOF) {
            cout << "no data in file " << kFnWeights << "!" << endl;
            exit(2);
        }
    }


    // get layer sizes:
    do {
        cout << "enter num of layers will construct (3~" << kMaxLayers << "):\t\t";
        cin >> num_layers;
    } while(num_layers>kMaxLayers || num_layers<3);

    // get neuron sizes in each layer:
    int i=0;
    for(i=0; i<kMaxLayers; i++) {
        num_neurons[i] = 0;
        layerptr[i] = nullptr;
    }

    do {
        cout << "enter num of neurons in each layer (2~" << kMaxNeurons << " ea/layer):\t";
        for(i=0;i<num_layers;i++) {
            cin >> num_neurons[i];
            if(num_neurons[i]>kMaxNeurons || num_neurons[i]<2)
              break;
        }
    } while(num_neurons[num_layers-1]<2 || num_neurons[num_layers-1]>kMaxNeurons);

    //  construct layers
    layerptr[0] = new LayerIn(0,num_neurons[0]);
    for(i=0; i<num_layers-1; i++)
        layerptr[i+1] = new LayerMid(num_neurons[i], num_neurons[i+1]);
    layerptr[num_layers-1] = new LayerOut(num_neurons[num_layers-2], num_neurons[num_layers-1]);

    //  link layers
    for(i=1; i<num_layers; i++)
        layerptr[i]->inputs = layerptr[i-1]->outputs;
    for(i=1; i<num_layers-1; i++)
        layerptr[i]->errors = layerptr[i+1]->errors_bp;

    //  allocate a buffer for training/testing data
    int n = num_neurons[0] + num_neurons[num_layers-1];

    if(mode)
        buffer = new float[n*kBufferSz];
    else
        buffer = new float[num_neurons[0]*kBufferSz];

    cout << delmt << endl;
}


void NetworkBP::RandomizeWeights() {

    for(int i=1; i<num_layers; i++)
        layerptr[i]->RandomizeWeights();
}


void NetworkBP::BufferData(ifstream &fsin) {
    // fsin could be training data or test data
    int itemlen, cnt=0, i;

    if(mode)
        itemlen = num_neurons[0] + num_neurons[num_layers-1];
    else
        itemlen = num_neurons[0];

    while(cnt<kBufferSz && !fsin.eof()) {
        for(i=0; i<itemlen; i++)
            fsin >> buffer[itemlen*cnt+i];
        cnt++;
    }

    if(fsin.eof())
        cnt--;

    if(cycles==0)
        num_ptns += cnt;

    buf_nptns = cnt;
}

void NetworkBP::FeedNeurons(int bufindex) {
    int nin = num_neurons[0], nout = num_neurons[num_layers-1];
    int idx,i;

    if(mode)
        idx = bufindex * (nin + nout);
    else
        idx = bufindex * nin;

    for(i=0; i<nin; i++)
        layerptr[0]->outputs[i] = buffer[idx+i];

    if(mode)
        for(i=0; i<nout; i++)
            dynamic_cast<LayerOut *>(layerptr[num_layers-1])->exp_outputs[i] = buffer[idx+nin+i];
}

void NetworkBP::FeedForward() {

    for(int i=1; i<num_layers; i++)
        layerptr[i]->CalculateOuts();
}

void NetworkBP::PropBackward() {

    for(int i=num_layers-1; i>0; i--)
        layerptr[i]->CalculateErrors();
}

void NetworkBP::UpdateWeights() {

    for(int i=1; i<num_layers; i++)
        layerptr[i]->UpdateWeights(beta);
}


void NetworkBP::StatsCycle() {

//    cout << "cycle:\t\t\t\t" << cycles << endl
//         << "sum error of the cycle:\t\t" << e_s_cycle << endl
//         << "rms error/pattern:\t\t" << e_rms_ptn << endl
//         << "sum error of all cycles:\t" << e_s_total << endl
//         << "rms error/cycle:\t\t" << e_rms_cycle << endl
//         << delmt2 << endl;

    fs_trainstats << "cycle:                     " << cycles << endl
                  << "sum error of the cycle:    " << e_s_cycle << endl
                  << "rms error/pattern:         " << e_rms_ptn << endl
                  << "sum error of all cycles:   " << e_s_total << endl
                  << "rms error/cycle:           " << e_rms_cycle << endl
                  << delmt2 << endl;
}

void NetworkBP::StatsTotal() {
    int i=0;

    cout << endl << delmt << endl
         << "\t\t\t_____ Training Statistics _____" << endl << delmt << endl << endl
         << "Learning Rate:\t\t\t\t" << beta << endl
         << "Error Tolerance:\t\t\t" << error_tolerance << endl
         << "Num of Layers in Network:\t\t" << num_layers << endl
         << "Num of Neurons in Each Layer:\t\t";

    for(i=0; i<num_layers; i++)
        cout << num_neurons[i] << spt;

    cout << endl << "Max Allowed Cycles:\t\t\t" << cycles_max << endl
         << "Total Cycles Ran:\t\t\t" << cycles << endl
         << "Num of Patterns/Cycle:\t\t\t" << num_ptns << endl
         << "Sum Error of Last Cycle:\t\t" << e_s_cycle << endl
         << "RMS Error/Pattern of Last Cycle:\t" << e_rms_ptn << endl
         << "Sum Error of All Cycles:\t\t" << e_s_total << endl
         << "RMS Error/Cycle:\t\t\t" << e_rms_cycle << endl
         << endl << delmt << endl << endl;

//    ListWeights();

    fs_trainstats << endl << delmt << endl
                  << "                        _____ Training Statistics _____" << endl << delmt << endl << endl
                  << "Learning Rate:                     " << beta << endl
                  << "Error Tolerance:                   " << error_tolerance << endl
                  << "Num of Layers in Network:          " << num_layers << endl
                  << "Num of Neurons in Each Layer:      ";

    for(i=0; i<num_layers; i++)
        fs_trainstats << num_neurons[i] << spt;

    fs_trainstats << endl << "Max Allowed Cycles:                " << cycles_max << endl
                  << "Total Cycles Ran:                  " << cycles << endl
                  << "Num of Patterns/Cycle:             " << num_ptns << endl
                  << "Sum Error of Last Cycle:           " << e_s_cycle << endl
                  << "RMS Error/Pattern of Last Cycle:   " << e_rms_ptn << endl
                  << "Sum Error of All Cycles:           " << e_s_total << endl
                  << "RMS Error/Cycle:                   " << e_rms_cycle << endl
                  << endl << delmt << endl << endl;
}


void NetworkBP::WriteWeights() {
    int i, j, k, nin, nout;

    fs_weights.setf(ios::fixed);
    fs_weights.precision(kfPrecision);

    for(i=1; i<num_layers; i++) {
        nin = layerptr[i]->numin;
        nout= layerptr[i]->numout;

        for(j=0; j<nin; j++) {
            for(k=0; k<nout; k++)
                fs_weights << setw(kfWidth) << layerptr[i]->weights[j*nout+k];
            fs_weights << endl;
        }
    }
}

void NetworkBP::ReadWeights() {
    int i, j, k, nin, nout;

    for(i=1; i<num_layers; i++) {
        nin = layerptr[i]->numin;
        nout = layerptr[i]->numout;

        for(j=0; j<nin; j++)
            for(k=0; k<nout; k++)
                fs_weights >> layerptr[i]->weights[j*nout+k];
    }
}

void NetworkBP::WriteOutputs() {
    int i = 0;
    int nout = layerptr[num_layers-1]->numout;

    cout.setf(ios::fixed);
    cout.precision(kfPrecision);
    fs_results.setf(ios::fixed);
    fs_results.precision(kfPrecision);

    for(i=0; i<nout; i++) {
        cout << setw(kfWidth) << layerptr[num_layers-1]->outputs[i];
        fs_results << setw(kfWidth) << layerptr[num_layers-1]->outputs[i];
    }

    cout << endl;
    fs_results << endl;
}




