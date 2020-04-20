// MergeSort
// best/avg/worst: O(nlgn)
// memory: depends; worst case is n
// stable: yes

#include <iostream>
using namespace std;

#define UL 99999

// l: index of left-half start
// m: index of left-half end
// r: index of right-half end
void Merge(int ar[], int l, int m, int r){
    int lar[m-l+2];
    int rar[r-m+1];
    int i, j, k;

    for(i=0; i<m-l+1; i++)
      lar[i]=ar[l+i];
    lar[m-l+1]=UL;
    for(i=0; i<r-m; i++)
      rar[i]=ar[m+1+i];
    rar[r-m]=UL;
   
    i=0;
    j=0;
    for(k=l; k<=r; k++){
        if(lar[i]<=rar[j]){
          ar[k]=lar[i];
          i++;
        }
        else{
          ar[k]=rar[j];
          j++;
        }
    }

}

void MergeSort(int ar[], int l, int r){
    int m;
    if(l<r){
        m=l+(r-l)/2;
        MergeSort(ar, l, m);
        MergeSort(ar, m+1, r);
        Merge(ar, l, m, r);
    }
}

int main(){
   int ar[10]={11, 321, 31, 401, 5, 632, 42, 39, 94, 509}; 
   MergeSort(ar, 0, 9);

   for(int i=0; i<10; i++)
     cout << ar[i] << "   " ;
}
