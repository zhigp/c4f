// HeapSort
// best/avg/worst: O(nlgn)
// memory: O(1)
// stable: no

#include <iostream>
using namespace std;

// Heap[0] = Heap Size;

//MaxHeapify assumes:
//  Left(i) & Right(i) are max-heaps;
//  ar[i] might be smaller than its children
void MaxHeapify(int ar[], int i){
    int l=2*i, r=2*i+1, key, tmp;
    
    if(l<=ar[0] && ar[l]>ar[i])
      key=l;
    else 
      key=i;

    if(r<=ar[0] && ar[r]>ar[key])
      key=r;

    if(key!=i){
        tmp=ar[i]; ar[i]=ar[key]; ar[key]=tmp;
        MaxHeapify(ar, key);
    }
}

// convert array ar[] to a max heap
// ar[0] = heap size
void BuildMaxHeap(int ar[]){
    for(int i=ar[0]/2; i>=1; i--)
      MaxHeapify(ar, i);
}

void HeapSort(int ar[]){
    int tmp;
    BuildMaxHeap(ar);
    for(int i=ar[0]; i>=2; i--){
        tmp=ar[1]; ar[1]=ar[i]; ar[i]=tmp;
        ar[0]--;
        MaxHeapify(ar,1);
    }

}

int main(){
    int ar[11]={10, 16, 43, 10, 114, 71, 59, 33, 42, 668, 781};
    HeapSort(ar);
    for(int i=1; i <11; i++)
        cout << ar[i] << "   ";
}
