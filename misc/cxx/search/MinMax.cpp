#include <iostream>
using namespace std;

// total comparisons:
//  3*(n/2)
void MinMax(int ar[], int len, int &min, int &max){
    int i;
    if(len%2==0){
        if(ar[0]<ar[1]){min=ar[0];max=ar[1];} else{min=ar[1];max=ar[0];}
        for(i=2; i<len; i+=2){
            if(ar[i]<ar[i+1]){
                if(ar[i]<min) min=ar[i];
                if(ar[i+1]>max) max=ar[i+1];
            }
            else{
                if(ar[i+1]<min) min=ar[i+1];
                if(ar[i]>max) max=ar[i];
            }
        }
    }
    else{
        min=max=ar[0];
        for(i=1; i<len; i+=2){
            if(ar[i]<ar[i+1]){
                if(ar[i]<min) min=ar[i];
                if(ar[i+1]>max) max=ar[i+1];
            }
            else{
                if(ar[i+1]<min) min=ar[i+1];
                if(ar[i]>max) max=ar[i];
            }
        }
    }

}

int main(){
    int ar[11]={110,134,243,3,343,83,2385,632,734,83,239};
    int min, max;
    MinMax(ar, 11, min, max);
    cout << min << ", " << max << endl;
}
