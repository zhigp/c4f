// QuickSort
// best/avg/worst: O(nlgn) / O(nlgn) / O(n^2)
// memory: O(lgn)
// stable: no for most implementations

// g++ -std=c++11 QuickSort.cxx -o QuickSort
// (chrono): clang v3.1 don't work with libstdc++
#include <iostream>
#include <random>
#include <chrono>
using namespace std;

int Partition(int ar[], int l, int r){
    int tmp, pivot=ar[r], i=l, j;
    for(j=l; j<r; j++)
        if(ar[j]<=pivot){
            tmp=ar[i]; ar[i]=ar[j]; ar[j]=tmp;
            i++;
        }
    tmp=ar[i]; ar[i]=ar[r]; ar[r]=tmp;
    return i;
}


int RandomPartition(int ar[], int l, int r){
    int id, x;
    unsigned seed=chrono::system_clock::now().time_since_epoch().count();
    default_random_engine gen(seed);
    uniform_int_distribution<int> dis(l,r-1);
    
    id=dis(gen);
    //cout << l << "," << r << "   "<< id << "," << ar[id] << endl;
    x=ar[id]; ar[id]=ar[r]; ar[r]=x;
    
    return Partition(ar, l, r);
}

void QuickSort(int ar[], int l, int r){
    if(l<r){
        int m=RandomPartition(ar, l, r);
        QuickSort(ar, l, m-1);
        QuickSort(ar, m+1, r);
    }
}


int main(){
    int ar[20]={5,23,3,75,2,98,8,29,7,62,9,93,0,37,1,80,4,38,6,69}, i;
    
    QuickSort(ar, 0, 19);
    
    for(i=0; i<20; i++)
      cout << ar[i] << "   ";
    cout << endl;
}
