// CountingSort
// avg/worst: O(n+maxin)
// memory: O(n+maxin)
// stable: yes
// no-comparison sort

#include <iostream>
using namespace std;

void CountingSort(int arin[], int arout[], int len, int maxin){
    int i,tmp[maxin+1];
    
    for(i=0; i<=maxin; i++)
      tmp[i]=0;
    for(i=0; i<len; i++)
      tmp[arin[i]]++;
    for(i=1; i<=maxin; i++)
      tmp[i]+=tmp[i-1];
    for(i=len-1; i>=0; i--){
        arout[tmp[arin[i]]-1]=arin[i];
        tmp[arin[i]]--;
    }
}


int main(){
    int arin[8]={2,5,3,0,2,3,0,3};
    int i,arout[8];
    
    CountingSort(arin, arout, 8, 5);

   for(i=0; i<8; i++)
     cout << arout[i] << "   ";
   cout << endl;
}
