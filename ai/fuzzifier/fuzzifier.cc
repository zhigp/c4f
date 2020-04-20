#include <iostream>
#include <cstdlib>
#include "fuzzifier.hh"
using namespace std;


float Category::get_share(float value) {
    // calculate the probability of a data falls into this category

    float share;
    float midlow, midhigh;

    midlow = midval_ - lowval_;         // mid to low distance
    midhigh = highval_ - midval_;       // mid to high distance

    if(value<=lowval_ || value>=highval_ )
        share = 0.0;

    else {
        if(value>midval_)
            share = (highval_-value)/midhigh;   // the closer the data to the mid value, the bigger the probability
        else if(value<midval_)
            share = (value-lowval_)/midlow;     // the closer the data to the mid value, the bigger the probability
        else
            share = 1.0;
    }

    return share;
}



void Fuzzifier::Init() {
    //
    // init Fuzzy Set: fill in parameters for each member category
    //

    string      input;                  // category name
    float       lval, mval, hval;       // category range
    Category*   ptr;

    // input category info, terminate with "end"
    while(true) {
        cout << endl << "Please input a Category name(one word)(input <end> to finish): " << endl;
        cin >> input;
        if(input[0]=='e' && input[1]=='n' && input[2]=='d')
            break;

        ptr = new Category;

        ptr->set_name(input);
        cout << "Assign lowval,midval,highval for this Category, separated by spaces:" << endl;
        cin >> lval >> mval >> hval;
        ptr->set_val(lval,mval,hval);

        fuzzyset_.push_back(ptr);

        n_category_++;
    }
}

void Fuzzifier::Fuzzify() {

    int             j = 0;
    float           invalue = 1.0;        // input value, to be evaluated
    vector<float>   share_100;          // share x 100: share of each Category presented by percentage
    float           share_total = 0, share_tmptotal=0;
    int             randnum;

    cout << endl
         << "==============================================" << endl
         << "======== Fuzzifier is Ready for Data: ========" << endl
         << "==============================================" << endl << endl;

    while(true){
        cout << "Input a single float value(input -9999 to exit):" << endl;
        cin >> invalue;
        if(invalue==-9999) break;

        share_100.clear();
        share_total = 0;

        for(j=0;j<n_category_;j++) {
            share_100.push_back(100 * fuzzyset_[j]->get_share(invalue));    // share_100 == 75  <--  75%
            share_total += share_100[j];                                     // maximum: 100
        }

        if(share_total == 0) {
            cout << endl << "Input value out of range!" << endl;
            continue;
        }

        srand((unsigned)time(NULL));
        randnum = rand() % (int)share_total;

        // decide the given data falls into which category based on its probability on each category
        j = 0;
        share_tmptotal = share_100[0];
        while((share_tmptotal<randnum) && (j<n_category_)){
            j++;
            share_tmptotal += share_100[j];
        }
        // decision made

        cout << "The Category for your input data is --> " << fuzzyset_[j]->get_name() << " <--" << endl;
        cout << "Probability to fall each Category(membership):" << endl;
        for(j=0;j<n_category_;j++)
            cout << fuzzyset_[j]->get_name() << ":\t\t" << (share_100[j]/share_total) << endl;
        cout << endl;
    }
}

Fuzzifier::~Fuzzifier() {
    for(int i=0; i<n_category_; i++)
        delete fuzzyset_[i];

}




