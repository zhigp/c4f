// InsertionSort
// best/avg/worst: O(n) / O(n^2) / O(n^2)
// memory: O(1)
// stable: no

#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
using namespace std;

int main(){
    
    vector<int> ivec;
    int i, j, key;

    cout << "please input the array you wanna sort: ";
    copy(istream_iterator<int>(cin), istream_iterator<int>(), back_inserter(ivec));
    
    for(i=1; i<ivec.size(); i++){
        key=ivec[i];
        j=i-1;
        while(j>=0 && ivec[j]>key){
            ivec[j+1]=ivec[j];
            j--;
        }
        ivec[j+1]=key;
    }

    cout << "the array after sort:" << endl;
    for(vector<int>::iterator it=ivec.begin(); it!=ivec.end(); it++)
      cout << *it << "  ";
        
}
