//	n: size of base str(Text)
//	m: size of search str(Pattern)

// Naive String Match 
// Running=Matching time:
//		O((n-m+1)m)

// Rabin-Karp Match
// Preprocess time:		O(m)
// Worst Matching time: O((n-m+1)m)
// 
// if q>m && effective s is constant, then
// expected matching time:
//		O(n+m)


#include<iostream>
#include<cmath>
using namespace std;

void RKMatch(string &T, string &P, int d, int q){
	int n=T.size(), m=P.size();
	int p=0, ts=0, h=static_cast<int>(pow(d, m-1)) % q;
	int s, i, sum=0;
	// preprocess
	for(i=0; i<m; i++){
		p=(d*p+P[i]-'0')%q;
		ts=(d*ts+T[i]-'0')%q;
	}
	// matching
	for(s=0; s<n-m+1; s++){
		if(p==ts){
			i=0;
			while(P[i]==T[s+i]){
				if(i==m-1){
					cout << "match found at pos: " << s << endl;
					sum++;
					break;
				}
				i++;
			}
		}
		if(s<n-m)
		  ts=(d*(ts-(T[s]-'0')*h)+T[s+m]-'0')%q;
		// ts mod q != ts%q
		if(ts<0)
		  ts=q+ts;
	}
	cout << "Total Matches: " << sum << endl;
}


void NaiveMatch(string &T, string &P){
	int n=T.size(), m=P.size();
	int i, j, sum=0;
	for(i=0; i<n-m+1; i++){
		j=0;
		while(P[j]==T[i+j]){
			if(j==m-1){
				cout << "match found at pos: " << i << endl;
				sum++;
				break;
			}
			else
			  j++;
		}
	}
	cout << "Total Matches: " << sum << endl;
}


int main(){
	string sT, sP;
    cout << "Demo of Naive/Rabin-Karp String Match Algorithm!" << endl;
	cout << "please input Text(base) string: ";
	cin >> sT;
	cout << "please input Pattern(search) string: ";
	cin >> sP;
	
	cout << endl;
	RKMatch(sT, sP, 10, 13);

	return 0;
}
