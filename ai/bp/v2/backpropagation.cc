#include <iostream>
#include <iomanip>
#include <random>
#include <functional>
#include <cmath>
#include "backpropagation.hh"
using namespace std;


const string tab("\t\t");
const string spt("  ");
const string delmt("--------------------------------------------------------------------------------");
const string delmt2("------------------------------------------------");


void HandleAllocFail() {
    cerr << "Unable to construct requested object: out of memory!" << endl;
    exit(-1);
}


default_random_engine               gener;
uniform_real_distribution<float>    dist(-1.0,1.0);
auto randomf = bind(dist,gener);


float Squash(float input) {
    // squashing function: using sigmoid

    if(input<-50)
        return 0;

    if(input>50)
        return 1.0;

    return (float) (1.0/(1.0+exp(-input)));
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

void LayerIn::PrintOutputs() {

    cout << tab;
    for(int i=0; i<numout; ++i)
        cout << outputs[i] << spt;
    cout << endl;
}



//
// Layer Middle
//
LayerMid::LayerMid(size_t nin, size_t nout) {
    size_t num = nin * nout;

    numin = nin;
    numout = nout;

    inputs = nullptr;
    outputs = new float[nout];

    weights = new float[num];
    errors = nullptr;
    errors_bp = new float[nin];

    deltawt_accum = new float[num];
    deltawt_prev = new float[num];

    for(int i=0; i<num; ++i) {
        deltawt_accum[i] = 0;
        deltawt_prev[i] = 0;
    }
}

LayerMid::~LayerMid() {

    delete []outputs;

    delete []weights;
    delete []errors_bp;

    delete []deltawt_accum;
    delete []deltawt_prev;
}


void LayerMid::RandomizeWeights() {

    for(int i=0; i<numin; ++i)
        for(int j=0; j<numout; ++j)
            weights[i*numout+j] = randomf();
}


void LayerMid::CalculateOuts() {
    float sum = 0;

    for(int i=0; i<numout; ++i) {
        for(int j=0; j<numin; ++j)
            sum += inputs[j] * weights[j*numout+i];
        outputs[i] = Squash(sum);
        sum = 0;
    }
}

void LayerMid::CalculateErrors() {
    float err = 0;

    for(int i=0; i<numin; ++i) {
        for(int j=0; j<numout; ++j)
            err += weights[i*numout+j] * errors[j];
        errors_bp[i] = err;
        err = 0;
        errors_bp[i] *= (inputs[i]*(1.0-inputs[i]));
    }
}

void LayerMid::UpdateWeights(float beta, float alpha) {
    int i,j,idx;
    float delta = 0;

    for(i=0; i<numin; ++i) {
        idx = i*numout;
        for(j=0; j<numout; ++j) {
            delta = beta * errors[j] * inputs[i] +
                    alpha * deltawt_prev[idx+j];
            weights[idx+j] += delta;
            deltawt_accum[idx+j] += delta;
        }
    }
}


void LayerMid::PrintOutputs() {

    cout << tab;
    for(int i=0; i<numout; ++i)
        cout << outputs[i] << spt;
    cout << endl;
}

void LayerMid::PrintErrors() {
    int i;

    cout << tab;
    for(i=0; i<numout; ++i)
        cout << errors[i] << spt;
    cout << "|" << spt;
    for(i=0; i<numin; ++i)
        cout << errors_bp[i] << spt;
    cout << endl;
}

void LayerMid::PrintWeights() {
    int idx;

    for(int i=0; i<numin; ++i) {
        cout << tab;
        idx = i*numout;
        for(int j=0; j<numout; ++j)
            cout << setw(kfWidth) << weights[idx+j];
        cout << endl;
    }
}



//
// Layer Out
//
LayerOut::LayerOut(size_t nin, size_t nout) {
    size_t num = nin*nout;

    numin = nin;
    numout = nout;

    inputs = nullptr;
    outputs = new float[nout];

    weights = new float[num];
    errors = new float[nout];
    errors_bp = new float[nin];

    deltawt_accum = new float[num];
    deltawt_prev = new float[num];

    exp_outputs = new float[nout];
    error_sum = 0;

    for(int i=0; i<num; ++i) {
        deltawt_accum[i] = 0;
        deltawt_prev[i] = 0;
    }
}

LayerOut::~LayerOut() {

    delete []outputs;

    delete []weights;
    delete []errors;
    delete []errors_bp;

    delete []deltawt_accum;
    delete []deltawt_prev;

    delete []exp_outputs;
}


void LayerOut::RandomizeWeights() {

    for(int i=0; i<numin; ++i)
        for(int j=0; j<numout; ++j)
            weights[i*numout+j] = randomf();
}


void LayerOut::CalculateOuts() {
    float sum = 0;

    for(int i=0; i<numout; ++i) {
        for(int j=0; j<numin; ++j)
            sum += inputs[j] * weights[j*numout+i];
        outputs[i] = Squash(sum);
        sum = 0;
    }
}

void LayerOut::CalculateErrors() {
    int i,j;
    float err = 0, errsum=0;

    for(i=0; i<numout; ++i) {
        errors[i] = exp_outputs[i] - outputs[i];
        errsum += errors[i];
    }

    for(i=0; i<numin; ++i) {
        for(j=0;j<numout;++j)
            err += weights[i*numout+j] * errors[j];
        errors_bp[i] = err;
        err = 0;
        errors_bp[i] *= (inputs[i]*(1.0-inputs[i]));
    }

    error_sum = errsum;
}

void LayerOut::UpdateWeights(float beta, float alpha) {
    int i,j,idx;
    float delta = 0;

    for(i=0; i<numin; ++i) {
        idx = i*numout;
        for(j=0; j<numout; ++j) {
            delta = beta * errors[j] * inputs[i] +
                    alpha * deltawt_prev[idx+j];
            weights[idx+j] += delta;
            deltawt_accum[idx+j] += delta;
        }
    }
}


void LayerOut::PrintOutputs() {

    cout << tab;
    for(int i=0; i<numout; ++i)
        cout << outputs[i] << spt;
    cout << endl;
}

void LayerOut::PrintErrors() {
    int i;

    cout << tab;
    for(i=0; i<numout; ++i)
        cout << errors[i] << spt;
    cout << "|" << spt;
    for(i=0; i<numin; ++i)
        cout << errors_bp[i] << spt;
    cout << endl;
}

void LayerOut::PrintWeights() {
    int idx;

    for(int i=0; i<numin; ++i) {
        cout << tab;
        idx = i*numout;
        for(int j=0; j<numout; ++j)
            cout << setw(kfWidth) << weights[idx+j];
        cout << endl;
    }
}



//
// Network BP
//
NetworkBP::NetworkBP() : buf_nptns(0),
                         num_ptns(0),
                         cycles(0),
                         e_ss_cycle(0),
                         e_s_cycle(0),
                         e_rms_ptn(101.0),
                         e_ss_total(0),
                         e_s_total(0),
                         e_rms_cycle(0) {

    set_new_handler(HandleAllocFail);

    cout << endl << delmt << endl
         << "\t\t\t_____ BP Neural Network Simulator _____"
         << endl << delmt << endl << endl;

    do {
        cout << "select the running mode ( 1 :Training, 0 :Testing):\t";
        cin.clear();
        cin >> mode;
    } while(cin.fail());

    if(mode)
        AcquireTrainingParms();
    else
        AcquireTestingParms();

    AcquireNetworkSz();
    ConstructNetwork();
    AllocBuffer();

    cout << delmt << endl;
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

    for(int i=0; i<num_layers; ++i)
        delete layerptr[i];

    delete []buffer;
}


void NetworkBP::Run() {
    int i = 0;
    float tmp = 0;

    if(mode) {  // Training Mode

        RandomizeWeights();

        while(cycles<cycles_max && e_rms_ptn>error_tolerance) {
            e_ss_cycle = 0;
            fs_train.clear();
            fs_train.seekg(fs_train.beg);

            while(!fs_train.eof()) {

                BufferData(fs_train);

                for(i=0; i<buf_nptns; ++i) {
                    FeedNeurons(i);
                    ApplyNoise();
                    FeedForward();
                    PropBackward();
                    UpdateWeights();

                    tmp = dynamic_cast<LayerOut *>(layerptr[num_layers-1])->error_sum;
                    e_ss_cycle += tmp * tmp;
                }
            } // end of one cycle

            cycles++;
            UpdateNoise();
            UpdateMomentum();
            UpdateStatParms();
            WriteCycleStats();
        } // end of loop in cycles

        PrintTotalStats();
        WriteTotalStats();
        WriteWeights();

    } else {    // Testing Mode

        ReadWeights();

        while(!fs_test.eof()) {

            BufferData(fs_test);

            for(i=0; i<buf_nptns; ++i) {
                FeedNeurons(i);
                FeedForward();
                WriteOutputs();
            }
        } // end of testing mode cycle, testing mode runs only 1 cycle
        cout << delmt << endl;
    } // end of Testing Mode Run
}


// network internal operations
void NetworkBP::AcquireTrainingParms() {

    cout << endl << delmt << endl << "\t\t\t Running In Training Mode " << endl << delmt << endl;

    do {
        cout << "enter the noise factor [0.00,1.00):\t\t\t";
        cin >> zeta_ini;
    } while(zeta_ini<0 || zeta_ini>=1.0);

    zeta = zeta_ini;

    do {
        cout << "enter the learning rate [0.01,1.00]:\t\t\t";
        cin >> beta;
    } while(beta<0.00999999 || beta>1.0);

    do {
        cout << "enter the learning momentum [0.00,1.00]:\t\t";
        cin >> alpha;
    } while(alpha<0 || alpha>1.0);

    do {
        cout << "enter the error tolerance [0.001,100.00]:\t\t";
        cin >> error_tolerance;
    } while(error_tolerance<0.001 || error_tolerance>100.0);

    do {
        cout << "enter the max cycles (>4, try 800):\t\t\t";
        cin >> cycles_max;
    } while(cycles_max<5);

    fs_train.open(kFnTrain);
    fs_weights.open(kFnWeights,ios::out);
    fs_trainstats.open(kFnTrainStats);
    if(fs_train.fail()) {
        cerr << "could not open file \"" << kFnTrain << "\" for read!" << endl;
        exit(1);
    }
    if(fs_train.peek()==EOF) {
        cerr << "no data in file " << kFnTrain << "!" << endl;
        exit(2);
    }
}

void NetworkBP::AcquireTestingParms() {

    cout << endl << delmt << endl << "\t\t\t Running In Testing Mode" << endl << delmt << endl;

    fs_test.open(kFnTest);
    fs_weights.open(kFnWeights,ios::in);
    fs_results.open(kFnResults);
    if(fs_test.fail()) {
        cerr << "could not open file \"" << kFnTest << "\" for read!" << endl;
        exit(1);
    }
    if(fs_weights.fail()) {
        cerr << "could not open file \"" << kFnWeights << "\" for read!" << endl;
        exit(1);
    }
    if(fs_test.peek()==EOF) {
        cerr << "no data in file " << kFnTest << "!" << endl;
        exit(2);
    }
    if(fs_weights.peek()==EOF) {
        cerr << "no data in file " << kFnWeights << "!" << endl;
        exit(2);
    }
}

void NetworkBP::AcquireNetworkSz() {

    // get layer sizes:
    do {
        cout << "enter num of layers will construct (3~" << kMaxLayers << "):\t\t";
        cin >> num_layers;
    } while(num_layers>kMaxLayers || num_layers<3);

    // get neuron sizes in each layer:
    int i=0;
    for(i=0; i<kMaxLayers; ++i) {
        num_neurons[i] = 0;
        layerptr[i] = nullptr;
    }

    do {
        cout << "enter num of neurons in each layer (2~" << kMaxNeurons << " ea/layer):\t";
        for(i=0;i<num_layers;++i) {
            cin >> num_neurons[i];
            if(num_neurons[i]>kMaxNeurons || num_neurons[i]<2)
              break;
        }
    } while(num_neurons[num_layers-1]<2 || num_neurons[num_layers-1]>kMaxNeurons);
}

void NetworkBP::ConstructNetwork() {
    int i=0;

    // construct layers
    layerptr[0] = new LayerIn(0,num_neurons[0]);
    for(i=0; i<num_layers-1; ++i)
        layerptr[i+1] = new LayerMid(num_neurons[i], num_neurons[i+1]);
    layerptr[num_layers-1] = new LayerOut(num_neurons[num_layers-2], num_neurons[num_layers-1]);

    //  link layers
    for(i=1; i<num_layers; ++i)
        layerptr[i]->inputs = layerptr[i-1]->outputs;
    for(i=1; i<num_layers-1; ++i)
        layerptr[i]->errors = layerptr[i+1]->errors_bp;
}

void NetworkBP::AllocBuffer() {
    int n = num_neurons[0] + num_neurons[num_layers-1];

    if(mode)
        buffer = new float[n*kBufferSz];
    else
        buffer = new float[num_neurons[0]*kBufferSz];
}


void NetworkBP::RandomizeWeights() {

    for(int i=1; i<num_layers; ++i)
        layerptr[i]->RandomizeWeights();
}

void NetworkBP::ReadWeights() {
    int i, j, k, nin, nout;

    for(i=1; i<num_layers; ++i) {
        nin = layerptr[i]->numin;
        nout = layerptr[i]->numout;

        for(j=0; j<nin; ++j)
            for(k=0; k<nout; ++k)
                fs_weights >> layerptr[i]->weights[j*nout+k];
    }
}


void NetworkBP::BufferData(ifstream &fsin) {
    // fsin could be training data or test data
    int itemlen, cnt=0, i;

    if(mode)
        itemlen = num_neurons[0] + num_neurons[num_layers-1];
    else
        itemlen = num_neurons[0];

    while(cnt<kBufferSz && !fsin.eof()) {
        for(i=0; i<itemlen; ++i)
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

    for(i=0; i<nin; ++i)
        layerptr[0]->outputs[i] = buffer[idx+i];

    if(mode)
        for(i=0; i<nout; ++i)
            dynamic_cast<LayerOut *>(layerptr[num_layers-1])->exp_outputs[i] = buffer[idx+nin+i];
}

void NetworkBP::ApplyNoise() {
    int nout = layerptr[0]->numout;
    float *pout = layerptr[0]->outputs;

    for(int i=0; i<nout; ++i)
        pout[i] *= ( 1 + beta*randomf() );
}

void NetworkBP::FeedForward() {

    for(int i=1; i<num_layers; ++i)
        layerptr[i]->CalculateOuts();
}

void NetworkBP::PropBackward() {

    for(int i=num_layers-1; i>0; --i)
        layerptr[i]->CalculateErrors();
}

void NetworkBP::UpdateWeights() {

    for(int i=1; i<num_layers; ++i)
        layerptr[i]->UpdateWeights(beta, alpha);
}

void NetworkBP::UpdateNoise() {

    if(cycles>0.7*cycles_max)
        zeta = 0;
    else if(cycles>0.5*cycles_max)
        zeta = 0.25 * zeta_ini;
    else if(cycles>0.3*cycles_max)
        zeta = 0.50 * zeta_ini;
    else if(cycles>0.1*cycles_max)
        zeta = 0.75 * zeta_ini;
}

void NetworkBP::UpdateMomentum() {
    int i,j,k,idx;
    size_t nin, nout;
    float *tmp;

    for(i=1; i<num_layers; ++i) {
        tmp = layerptr[i]->deltawt_prev;
        layerptr[i]->deltawt_prev = layerptr[i]->deltawt_accum;
        layerptr[i]->deltawt_accum = tmp;

        nin = layerptr[i]->numin;
        nout = layerptr[i]->numout;
        for(j=0; j<nin; ++j) {
            idx = j*nout;
            for(k=0; k<nout; ++k)
                tmp[idx+k] = 0;         // deltawt_accum = 0;
        }
    }
}

void NetworkBP::UpdateStatParms() {

    e_s_cycle = sqrt(e_ss_cycle);
    e_rms_ptn = sqrt(e_ss_cycle/num_ptns);
    e_ss_total += e_ss_cycle;
    e_s_total = sqrt(e_ss_total);
    e_rms_cycle = sqrt(e_ss_total/cycles);
}


void NetworkBP::PrintCycleStats() {

    fs_trainstats << "cycle:                     " << cycles << endl
                  << "sum error of the cycle:    " << e_s_cycle << endl
                  << "rms error/pattern:         " << e_rms_ptn << endl
                  << "sum error of all cycles:   " << e_s_total << endl
                  << "rms error/cycle:           " << e_rms_cycle << endl
                  << delmt2 << endl;
}

void NetworkBP::WriteCycleStats() {

    cout << "cycle:\t\t\t\t" << cycles << endl
         << "sum error of the cycle:\t\t" << e_s_cycle << endl
         << "rms error/pattern:\t\t" << e_rms_ptn << endl
         << "sum error of all cycles:\t" << e_s_total << endl
         << "rms error/cycle:\t\t" << e_rms_cycle << endl
         << delmt2 << endl;
}

void NetworkBP::PrintTotalStats() {

    cout << endl << delmt << endl
         << "\t\t\t_____ Training Statistics _____" << endl << delmt << endl << endl
         << "Noise Factor for Inputs:\t\t" << zeta_ini << endl
         << "Learning Rate:\t\t\t\t" << beta << endl
         << "Learning Momentum:\t\t\t" << alpha << endl
         << "Error Tolerance:\t\t\t" << error_tolerance << endl
         << "Num of Layers in Network:\t\t" << num_layers << endl
         << "Num of Neurons in Each Layer:\t\t";

    for(int i=0; i<num_layers; ++i)
        cout << num_neurons[i] << spt;

    cout << endl << "Max Allowed Cycles:\t\t\t" << cycles_max << endl
         << "Total Cycles Ran:\t\t\t" << cycles << endl
         << "Num of Patterns/Cycle:\t\t\t" << num_ptns << endl
         << "Sum Error of Last Cycle:\t\t" << e_s_cycle << endl
         << "RMS Error/Pattern of Last Cycle:\t" << e_rms_ptn << endl
         << "Sum Error of All Cycles:\t\t" << e_s_total << endl
         << "RMS Error/Cycle:\t\t\t" << e_rms_cycle << endl
         << endl << delmt << endl << endl;

//    PrintWeights();
//    cout << endl << delmt << endl << endl;
}

void NetworkBP::WriteTotalStats() {

    fs_trainstats << endl << delmt << endl
                  << "                        _____ Training Statistics _____" << endl << delmt << endl << endl
                  << "Noise Factor for Inputs:           " << zeta_ini << endl
                  << "Learning Rate:                     " << beta << endl
                  << "Learning Momentum:                 " << alpha << endl
                  << "Error Tolerance:                   " << error_tolerance << endl
                  << "Num of Layers in Network:          " << num_layers << endl
                  << "Num of Neurons in Each Layer:      ";

    for(int i=0; i<num_layers; ++i)
        fs_trainstats << num_neurons[i] << spt;

    fs_trainstats << endl << "Max Allowed Cycles:                " << cycles_max << endl
                  << "Total Cycles Ran:                  " << cycles << endl
                  << "Num of Patterns/Cycle:             " << num_ptns << endl
                  << "Sum Error of Last Cycle:           " << e_s_cycle << endl
                  << "RMS Error/Pattern of Last Cycle:   " << e_rms_ptn << endl
                  << "Sum Error of All Cycles:           " << e_s_total << endl
                  << "RMS Error/Cycle:                   " << e_rms_cycle << endl
                  << endl << delmt << endl;
}

void NetworkBP::PrintOutputs() {

    cout << "outputs of the output layer(cycle:" << cycles << "):" << endl;
    layerptr[num_layers-1]->PrintOutputs();
}

void NetworkBP::PrintErrors() {

    cout << "errors of the output layer(cycle:" << cycles << "):" << endl;
    layerptr[num_layers-1]->PrintErrors();
}

void NetworkBP::PrintWeights() {

    cout.setf(ios::fixed);
    cout.precision(kfPrecision);

    cout << "Weights[in][out] of all layers(cycle:" << cycles << "):" << endl;
    for(int i=1; i<num_layers; ++i) {
        cout << "\tlayer id: " << i << endl;
        layerptr[i]->PrintWeights();
    }
}


void NetworkBP::WriteWeights() {
    int i, j, k, nin, nout;

    fs_weights.setf(ios::fixed);
    fs_weights.precision(kfPrecision);

    for(i=1; i<num_layers; ++i) {
        nin = layerptr[i]->numin;
        nout= layerptr[i]->numout;

        for(j=0; j<nin; ++j) {
            for(k=0; k<nout; ++k)
                fs_weights << setw(kfWidth) << layerptr[i]->weights[j*nout+k];
            fs_weights << endl;
        }
    }
}

void NetworkBP::WriteOutputs() {
    int i = 0;
    int nout = layerptr[num_layers-1]->numout;

    cout.setf(ios::fixed);
    cout.precision(kfPrecision);
    fs_results.setf(ios::fixed);
    fs_results.precision(kfPrecision);

    for(i=0; i<nout; ++i) {
        cout << setw(kfWidth) << layerptr[num_layers-1]->outputs[i];
        fs_results << setw(kfWidth) << layerptr[num_layers-1]->outputs[i];
    }

    cout << endl;
    fs_results << endl;
}




