/////////////////////////////////////////////////////////////////////////////////////

//      BP Neural Network
// Feed Forward Backward Propagation of Errors

//////////////////////////////////////////////////////////////////////////////////////

// Learning Rate:               beta        [0.01, 1.00]
// No momentum learing(alpha)

// Weight Change:               beta * input * errors_bp

// Errors:                      Least Mean Squared

// Threshold Function:          sigmoid
// No output bias

//////////////////////////////////////////////////////////////////////////////////////


#include <fstream>
#include <string>
using namespace std;


const int kfPrecision       = 4;
const int kfWidth           = 10;

const size_t kMaxLayers     = 7;          // max num of layers in network
const size_t kMaxNeurons    = 50;         // max num of neurons in each layer
const size_t kBufferSz      = 50;         // sz * (in+out), 100 sets of in-out pair

const string kFnTrain("dtrain.bp");             // input pattern pairs for training
const string kFnWeights("dweights.bp");         // training mode output & testing mode input
const string kFnTrainStats("dtrainstats.bp");   // statistics for last training process
const string kFnTest("dtest.bp");               // input patterns for testing
const string kFnResults("dresults.bp");         // output of testing mode


class NetworkBP;

// virtual base class layer:
class Layer{
friend NetworkBP;
public:
    Layer() {};
    virtual ~Layer() {};                        // ensure dtor of derived class is called

    virtual void CalculateOuts() {};
    virtual void CalculateErrors() {};

    virtual void ListOutputs() {};
    virtual void ListErrors() {};
    virtual void ListWeights() {};

    virtual void RandomizeWeights() {};
    virtual void UpdateWeights(float beta) {};  // beta: learning rate

protected:
    size_t  numin;
    size_t  numout;

    float   *inputs;           // array of inputs
    float   *outputs;          // array of outputs

    float   *weights;          // weights[in][out], range = [-1,1]
    float   *errors;           // array of errors at output, range = [0,1]
    float   *errors_bp;        // array of errors back-propagated
};


// input layer:
class LayerIn : public Layer {
public:
    LayerIn(size_t nin, size_t nout);
    virtual ~LayerIn();

    virtual void ListOutputs();
};


// middle layer:
class LayerMid : public Layer {
public:
    LayerMid(size_t nin, size_t nout);
    virtual ~LayerMid();

    virtual void CalculateOuts();
    virtual void CalculateErrors();

    virtual void ListOutputs();
    virtual void ListErrors();
    virtual void ListWeights();

    virtual void RandomizeWeights();
    virtual void UpdateWeights(float beta);              // beta: learning rate
};


// output layer:
class LayerOut : public Layer {
friend NetworkBP;
public:
    LayerOut(size_t nin, size_t nout);
    virtual ~LayerOut();

    virtual void CalculateOuts();
    virtual void CalculateErrors();

    virtual void ListOutputs();
    virtual void ListErrors();
    virtual void ListWeights();

    virtual void RandomizeWeights();
    virtual void UpdateWeights(float beta);              // beta: learning rate

private:
    float   *exp_outputs;           // expected outputs
    float   error_sum;              // sum error of a pattern, updated everytime CalculateErrors() is called
};


// back propagation network:
class NetworkBP{
public:
    NetworkBP();
    ~NetworkBP();

    void    Run();

    void    ListOutputs();
    void    ListErrors();
    void    ListWeights();

private:
    void    init();

    void    RandomizeWeights();

    void    BufferData(ifstream &fsin);
    void    FeedNeurons(int bufindex);          // get data from buffer, one pair(in,expt out) a time
    void    FeedForward();
    void    PropBackward();
    void    UpdateWeights();

    void    StatsCycle();               // Training Mode, statistics / cycle
    void    StatsTotal();               // Training Mode, statistics after training finished

    void    WriteWeights();             // called after training mode finished

    void    ReadWeights();              // called at testing mode start
    void    WriteOutputs();             // called in testing mode to write output of each input pattern

private:
    bool        mode;               // true: Training  false: Testing

    float       beta;               // Training Mode: learning rate     [0.01,1.00]
    float       error_tolerance;    // Training Mode: error control     [0.001,100.00]
    size_t      cycles_max;         // Training Mode: max allowed cycles

    ifstream    fs_train;           // Training Mode:
    fstream     fs_weights;         //      fs_train --> fs_weights
    ofstream    fs_trainstats;      //               --> fs_trainstats
    ifstream    fs_test;            // Testing Mode:
    ofstream    fs_results;         //      fs_test + fs_weights --> fs_results

    size_t      num_layers;                 // how many layers in network
    size_t      num_neurons[kMaxLayers];    // array[num_layers], how many neurons in each layer
    Layer       *layerptr[kMaxLayers];      // array[num_layers], ptr to each layer

    float       *buffer;

    // for statistics, updated when running the network:
    size_t      buf_nptns;          // how many patterns(groups of inputs+outputs) read in buffer, updated everytime BufferData() is called
    size_t      num_ptns;           // num of patterns in training/testing file, or patterns/cycle

    size_t      cycles;             // cycles have been run, 1 cycle = all the patterns

                // s:sum;  ss:sum of square;    rms:root mean square;
    float       e_ss_cycle;         // sum of square error in one cycles, e_ss_cycle = sum(error_sum * error_sum)
    float       e_s_cycle;          // sum error of one cycle: e_s_cycle = sqrt(e_ss_cycle)
    float       e_rms_ptn;          // = sqrt(e_ss_cycle/num_ptn)
    float       e_ss_total;         // = sum(e_ss_cycle)
    float       e_s_total;          // = sqrt(e_ss_total)
    float       e_rms_cycle;        // = sqrt(e_ss_total/cycles)
};

