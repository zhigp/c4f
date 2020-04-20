#include<iostream>
#include<cassert>
using namespace std;

// BinSearch return code: -1: Key Not Found;

// Deferred Detection of Equality
int BinSearchD(int ary[], int key, int imin, int imax){
    while(imin < imax){
        int imid = imin + (imax-imin)/2;
        assert(imid < imax);
        
        if(key > ary[imid])
          imin = imid + 1;
        else
          imax = imid;
    }

    if((imax==imin) && (ary[imin]==key))
      return imin;
    else
      return -1;
}

// Iterative
int BinSearchI(int ary[], int key, int imin, int imax){

    while(imin <= imax){
    int imid = imin + (imax-imin)/2;
    if(key < ary[imid])
      imax = imid - 1;
    else if(key > ary[imid])
      imin = imid + 1;
    else 
      return imid;
    }

    return -1;
}

// Recursive
int BinSearchR(int ary[], int key, int imin, int imax){
    if(imin > imax)
      return -1;
    else{
        int imid = imin + (imax - imin)/2;
        if(key > ary[imid])         return BinSearchR(ary, key, imid+1, imax);
        else if(key < ary[imid])    return BinSearchR(ary, key, imin, imid-1);
        else                        return imid;
    }
}

int main(){
    int ar[101], length = 101, key;
    for(int i=0; i<101; i++)
      ar[i]=i;
    cout << " please enter a integer for search: ";
    while(cin >> key)
        cout << " resulet: " << BinSearchD(ar, key, 0, 100) << endl << " please enter a integer for search: ";
    return 0;
}
