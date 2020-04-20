#include<iostream>
#include<string>
#include<complex>
#include<utility>

#include<vector>
#include<queue>
#include<deque>
#include<list>
#include<stack>
#include<set>
#include<map>

using namespace std;

int main(){
    
    char ops;
    cout << endl << " Display basic or container type ? " << endl;
    cout << " Input b for basic, c for container, x to quit: ";
    while(cin >> ops){

        string delimiter(" --------------------------------------------------------------------------------");

        if(ops == 'x') return 0;

        else if(ops == 'c'){
            string str;

            vector<int> ivec;
            vector<float> fvec;
            vector<string> svec;
            for(int i=0; i<5; i++)
              ivec.push_back(i);
            
            queue<int> ique;
            queue<float> fque;
            queue<string> sque;
            for(float f=0.0; f<5.0; f++)
              fque.push(f);

            deque<int> idq;
            deque<float> fdq;
            deque<string> sdq;

            for(int i=0; i<5; i++){
                str += static_cast<char>('a' + i);
                sdq.push_back(str);
            }

            list<int> ilst;
            list<float> flst;
            list<string> slst;
            for(int i=0; i<5; i++)
              ilst.push_back(i);

            stack<int> istk;
            stack<float> fstk;
            stack<string> sstk;
            for(float f=0.0; f<5.0; f++)
              fstk.push(f);
            str.clear();
            for(int i=0; i<5; i++){
                str += static_cast<char>('a' +(i%26));
                sstk.push(str);
            }
              
            set<int> iset;
            set<float> fset;
            set<string> sset;
            str.clear();
            for(int i=0; i<5; i++){
                fset.insert(static_cast<float>(i));
                str += static_cast<char>('a' + (i%26));
                sset.insert(str);
            }
           
            map<int, string> int_str;
            map<char, string> c_str;

            c_str.insert(map<char,string>::value_type('A',string("a")));
            c_str.insert(map<char,string>::value_type('B',string("ab")));
            c_str.insert(map<char,string>::value_type('C',string("abc")));
            c_str.insert(map<char,string>::value_type('D',string("abcd")));
            c_str.insert(map<char,string>::value_type('E',string("abcde")));

            str.clear();
            string abc("abc");

            cout<< endl << endl << "\t\t\t+++++ Container Data Type Size +++++" << endl << endl;
            cout<< delimiter << endl
                << " |\t\t\t\tsizeof\t\tsize()\t\tcapacity()\t|" << endl
                << delimiter << endl
                << " |   string:\t\t\t" << sizeof(str) << "\t\t" << str.size() << "\t\t" << str.capacity() << "\t\t|" << endl
                << " |   string(\"abc\"):\t\t" << sizeof(abc) << "\t\t" << abc.size() << "\t\t" << abc.capacity() << "\t\t|" << endl
                << delimiter << endl
                << " |   ivec:\t\t\t" << sizeof(ivec) << "\t\t" << ivec.size() << "\t\t" << ivec.capacity() << "\t\t|" << endl
                << " |   vector<float>:\t\t" << sizeof(vector<float>) << "\t\t" << fvec.size() << "\t\t" << fvec.capacity() << "\t\t|" << endl
                << " |   vector<string>:\t\t" << sizeof(vector<string>) << "\t\t" << svec.size() << "\t\t" << svec.capacity() << "\t\t|" << endl
                << delimiter << endl
                << " |   queue<int>:\t\t" << sizeof(queue<int>) << "\t\t" << ique.size() << "\t\tN/A\t\t|" << endl
                << " |   fque:\t\t\t" << sizeof(fque) << "\t\t" << fque.size() << "\t\tN/A\t\t|" << endl
                << " |   queue<string>:\t\t" << sizeof(queue<string>) << "\t\t" << sque.size() << "\t\tN/A\t\t|" << endl
                << delimiter << endl
                << " |   deque<int>:\t\t" << sizeof(deque<int>) << "\t\t" << idq.size() << "\t\tN/A\t\t|" << endl
                << " |   deque<float>:\t\t" << sizeof(deque<float>) << "\t\t" << fdq.size() << "\t\tN/A\t\t|" << endl
                << " |   sdq:\t\t\t" << sizeof(sdq) << "\t\t" << sdq.size() << "\t\tN/A\t\t| " << endl
                << delimiter << endl
                << " |   ilst:\t\t\t" << sizeof(ilst) << "\t\t" << ilst.size() << "\t\tN/A\t\t|" << endl
                << " |   list<float>:\t\t" << sizeof(list<float>) << "\t\t" << flst.size() << "\t\tN/A\t\t|" << endl
                << " |   list<string>:\t\t" << sizeof(list<string>) << "\t\t" << slst.size() << "\t\tN/A\t\t|" << endl
                << delimiter << endl
                << " |   stack<int>:\t\t" << sizeof(stack<int>) << "\t\t" << istk.size() << "\t\tN/A\t\t|" << endl
                << " |   fstk:\t\t\t" << sizeof(fstk) << "\t\t" << fstk.size() << "\t\tN/A\t\t|" << endl
                << " |   sstk:\t\t\t" << sizeof(sstk) << "\t\t" << sstk.size() << "\t\tN/A\t\t|" << endl
                << delimiter << endl
                << " |   set<int>:\t\t\t" << sizeof(set<int>) << "\t\t" << iset.size() << "\t\tN/A\t\t|" << endl
                << " |   fset:\t\t\t" << sizeof(fset) << "\t\t" << fset.size() << "\t\tN/A\t\t|" << endl
                << " |   sset:\t\t\t" << sizeof(sset) << "\t\t" << sset.size() << "\t\tN/A\t\t|" << endl
                << delimiter << endl
                << " |   map<int, string>:\t\t" << sizeof(map<int, string>) << "\t\t" << int_str.size() << "\t\tN/A\t\t|" << endl
                << " |   map<char,string>:\t\t" << sizeof(map<char, string>) << "\t\t" << c_str.size() << "\t\tN/A\t\t|" << endl
                << delimiter << endl << endl;
            
            cout << " Input b for basic, c for container, x to quit: ";
        }

        else {
            cout<< endl << endl << "\t\t\t+++++ Basic Data Type Size +++++" << endl << endl;
            cout<< delimiter << endl
                << " |   NULL:\t\t\t" << sizeof(NULL) << "\t\tcout:\t\t\t" << NULL << "\t|" << endl
                << " |   nullptr:\t\t\t" << sizeof(nullptr) << "\t\tcout:\t\t\t" << "N/A" << "\t|" << endl
                << " |   bool:\t\t\t" << sizeof(bool) << "\t\tcout:\t\t\t" << true << ":" << false << "     |" << endl
                << delimiter << endl
                << " |   int &:\t\t\t" << sizeof(int &) << "\t|\tint *:\t\t\t" << sizeof(int *) << "\t|" << endl
                << " |   float &:\t\t\t" << sizeof(float &) << "\t|\tfloat *:\t\t" << sizeof(float *) << "\t|" << endl
                << " |   string &:\t\t\t" << sizeof(string &) << "\t|\tstring *:\t\t" << sizeof(string *) << "\t|" << endl
                << delimiter << endl
                << " |   int:\t\t\t" << sizeof(int) << "\t|\tshort:\t\t\t" << sizeof(short) << "\t|" << endl
                << " |   signed:\t\t\t" << sizeof(signed) << "\t|\tlong:\t\t\t" << sizeof(long) << "\t|" << endl
                << " |   unsigned:\t\t\t" << sizeof(unsigned) << "\t|\tlong long:\t\t" << sizeof(long long) << "\t|" <<  endl
                << delimiter << endl
                << " |   float:\t\t\t" << sizeof(float) << "\t|\tchar:\t\t\t" << sizeof(char) << "\t|" << endl
                << " |   double:\t\t\t" << sizeof(double) << "\t|\twchar_t:\t\t" << sizeof(wchar_t) << "\t|" << endl
                << " |   long double:\t\t" << sizeof(long double) << "\t|\tstring:\t\t\t" << sizeof(string) << "\t|" << endl
                << delimiter << endl
                << " |   complex<float>:\t\t" << sizeof(complex<float>) << "\t|\tpair<int, int>:\t\t" << sizeof(pair<int, int>) << "\t|" << endl
                << " |   complex<double>:\t\t" << sizeof(complex<double>) << "\t|\tpair<float, string>:\t" << sizeof(pair<float, string>) << "\t|" << endl
                << " |   complex<long double>:\t" << sizeof(complex<long double>) << "\t|\tpair<string, string>:\t" << sizeof(pair<string, string>) << "\t|" << endl
                << delimiter << endl << endl;

                cout << " Input b for basic, c for container, x to quit: ";
        }
    }
}
