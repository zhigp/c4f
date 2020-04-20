// Knuth-Morris-Pratt Matching Algorithm
// Preprocess time:	O(m)
// Matching time:	O(n)
//	    m: length of the Pattern string(search string)
//	    n: length of the Text string(base string)


#include<iostream>
using namespace std;

#define MMAX	10
int pi[MMAX+1];

void InitPi(string &P){
// q: counting, start from 1, end with m; 
	int m=P.size(), k=0, q;
    for(q=2; q<=m; q++){
        while(k>0 && P[k]!=P[q-1])
          k=pi[k];
        if(P[k]==P[q-1])
          k++;
        pi[q]=k;
    }
}

void KMP(string &T, string &P){
	int m=P.size(), n=T.size();
	int i=0, sum=0;
	int q=0;                //number of char matched

    InitPi(P);
	for(i=0; i<n; i++){     //scan the test from left to right
		while(q>0 && P[q]!=T[i])
		  q=pi[q];          //next char does not match
		if(P[q]==T[i])
		  q++;              //next char matches
		if(q==m){
			sum++;
			cout << "match found at pos: " << i-m+1 << endl;
			q=pi[q];        //look for the next match
		}
	}
	cout << "Total Matches: " << sum << endl << endl;
}

int main(){
	string sP, sT;
    int i;
    cout << "Demo of KMP String Match Algorithm!" << endl; 
    cout << "please input Text(base) string: ";
    cin >> sT;
    cout << "please input Pattern(search) string(" << MMAX << " characters max): ";
    cin >> sP;
    cout << endl;
    KMP(sT, sP);
}
