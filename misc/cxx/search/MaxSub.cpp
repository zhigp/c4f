#include <iostream>
using namespace std;

#define LL -99999

struct Val{
    int l;
    int r;
    int sum;
};

Val MaxX(int ar[], int l, int m, int r){
    int lsum=LL, rsum=LL;
    int sum, i;
    Val xval;

    sum=0;
    for(i=m; i>=l; i--){
        sum+=ar[i];
        if(sum > lsum){
            lsum=sum;
            xval.l=i;
        }
    }

    sum=0;
    for(i=m+1; i<=r; i++){
        sum+=ar[i];
        if(sum > rsum){
            rsum=sum;
            xval.r=i;
        }
    }

    xval.sum=lsum+rsum;
    return xval;
}


Val MaxSub(int ar[], int l, int r){
    if(l==r){
        Val val;
        val.l=l;
        val.r=r;
        val.sum=ar[l];
        return val;
    }
    else{
        Val lval, rval, xval;
        int m=l+(r-l)/2;
        
        lval=MaxSub(ar, l, m);
        rval=MaxSub(ar, m+1, r);
        xval=MaxX(ar, l, m, r);

        if(lval.sum>=rval.sum && lval.sum>=xval.sum)
          return lval;
        else if(rval.sum>=lval.sum && rval.sum>=xval.sum)
          return rval;
        else 
          return xval;
    }
}


int main(){
    int ar[16]={13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7};
    Val val=MaxSub(ar, 0, 15);
    cout << "array:\t\t\t";
    for(int i=0; i<16; i++)
      cout << ar[i] << "  ";
    cout << endl;
    cout << "max subarray sum:\t" << val.sum << endl
        << "left index:\t\t" << val.l << endl 
        << "right index:\t\t" << val.r << endl;

    return 0;
}
