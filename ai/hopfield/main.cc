#include "hopfield.hh"


// weight matrix to match ptn1 and ptn2:

//      0   -3  3   -3
//      -3  0   -3  3
//      3   -3  0   -3
//      -3  3   -3  0

//int wt1[kDimention] = {0,-3,3,-3};
//int wt2[kDimention] = {-3,0,-3,3};
//int wt3[kDimention] = {3,-3,0,-3};
//int wt4[kDimention] = {-3,3,-3,0};



// weight matrix to match ptn1 ~ ptn4:

//        0   -5  4   4
//        -5  0   4   4
//        4   4   0   -5
//        4   4   -5  0

int wt1[kDimention] = {0,-5,4,4};
int wt2[kDimention] = {-5,0,4,4};
int wt3[kDimention] = {4,4,0,-5};
int wt4[kDimention] = {4,4,-5,0};



// input pattern:

int ptn1[kDimention] = {1,0,1,0};
int ptn2[kDimention] = {0,1,0,1};
int ptn3[kDimention] = {1,0,0,1};
int ptn4[kDimention] = {0,1,1,0};

int ptn5[kDimention] = {1,0,0,0};


int main() {

    Hopfield hfn(wt1,wt2,wt3,wt4);

    cout << endl << "======================================" << endl << endl;

    // pattern 1:
    cout << "Pattern 1:\t( ";
    for(int i=0; i<kDimention; i++)
        if(i<kDimention-1)
            cout << ptn1[i] << ", ";
        else
            cout << ptn1[i];
    cout << " )" << endl << endl;

    hfn.Activate(ptn1);

    // check if the pattern is correctly recalled:
    cout << "Output\t" << "Pattern\t" << endl;
    for(int i=0; i<kDimention; i++)
        if(hfn.get_output(i)==ptn1[i])
            cout << "   " << hfn.get_output(i) << "\t   " << ptn1[i] << "\t\t" << "Matched" << endl;
        else
            cout << "   " << hfn.get_output(i) << "\t   " << ptn1[i] << "\t\t" << "Discrepant" << endl;

    cout << endl << "======================================" << endl << endl;

    // pattern 2:
    cout << "Pattern 2:\t( ";
    for(int i=0; i<kDimention; i++)
        if(i<kDimention-1)
            cout << ptn2[i] << ", ";
        else
            cout << ptn2[i];
    cout << " )" << endl << endl;

    hfn.Activate(ptn2);

    // check if the pattern is correctly recalled:
    cout << "Output\t" << "Pattern\t" << endl;
    for(int i=0; i<kDimention; i++)
        if(hfn.get_output(i)==ptn2[i])
            cout << "   " << hfn.get_output(i) << "\t   " << ptn2[i] << "\t\t" << "Matched" << endl;
        else
            cout << "   " << hfn.get_output(i) << "\t   " << ptn2[i] << "\t\t" << "Discrepant" << endl;

    cout << endl << "======================================" << endl << endl;

    // pattern 3:
    cout << "Pattern 3:\t( ";
    for(int i=0; i<kDimention; i++)
        if(i<kDimention-1)
            cout << ptn3[i] << ", ";
        else
            cout << ptn3[i];
    cout << " )" << endl << endl;

    hfn.Activate(ptn3);

    // check if the pattern is correctly recalled:
    cout << "Output\t" << "Pattern\t" << endl;
    for(int i=0; i<kDimention; i++)
        if(hfn.get_output(i)==ptn3[i])
            cout << "   " << hfn.get_output(i) << "\t   " << ptn3[i] << "\t\t" << "Matched" << endl;
        else
            cout << "   " << hfn.get_output(i) << "\t   " << ptn3[i] << "\t\t" << "Discrepant" << endl;

    cout << endl << "======================================" << endl << endl;

    // pattern 4:
    cout << "Pattern 4:\t( ";
    for(int i=0; i<kDimention; i++)
        if(i<kDimention-1)
            cout << ptn4[i] << ", ";
        else
            cout << ptn4[i];
    cout << " )" << endl << endl;

    hfn.Activate(ptn4);

    // check if the pattern is correctly recalled:
    cout << "Output\t" << "Pattern\t" << endl;
    for(int i=0; i<kDimention; i++)
        if(hfn.get_output(i)==ptn4[i])
            cout << "   " << hfn.get_output(i) << "\t   " << ptn4[i] << "\t\t" << "Matched" << endl;
        else
            cout << "   " << hfn.get_output(i) << "\t   " << ptn4[i] << "\t\t" << "Discrepant" << endl;

    cout << endl << "======================================" << endl << endl;

    // pattern 5:
    cout << "Pattern 5:\t( ";
    for(int i=0; i<kDimention; i++)
        if(i<kDimention-1)
            cout << ptn5[i] << ", ";
        else
            cout << ptn5[i];
    cout << " )" << endl << endl;

    hfn.Activate(ptn5);

    // check if the pattern is correctly recalled:
    cout << "Output\t" << "Pattern\t" << endl;
    for(int i=0; i<kDimention; i++)
        if(hfn.get_output(i)==ptn5[i])
            cout << "   " << hfn.get_output(i) << "\t   " << ptn5[i] << "\t\t" << "Matched" << endl;
        else
            cout << "   " << hfn.get_output(i) << "\t   " << ptn5[i] << "\t\t" << "Discrepant" << endl;

    cout << endl << "======================================" << endl << endl;


    return 0;
}
