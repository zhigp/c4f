#include <string>
#include <vector>
using namespace std;

class Category {
public:
    Category() {};

    void set_name(string &s) { name_=s; };
    string &get_name() { return name_; };

    void set_val(float low, float mid, float high) {
        lowval_ = low; midval_ = mid; highval_ = high;
    };
    float get_lowval() { return lowval_; };
    float get_midval() { return midval_; };
    float get_highval() { return highval_; };

    float get_share(float);

    ~Category(){};

private:
    string  name_;
    float   lowval_,midval_,highval_;
};



class Fuzzifier{
public:
    Fuzzifier():n_category_(0) {};

    void Init();

    void Fuzzify();

    ~Fuzzifier();

private:
    size_t n_category_;
    vector<Category *> fuzzyset_;
};
