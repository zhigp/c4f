// Finite State Machine(FSM) Match
// Finite Automata, Finite State Automata, 
//
// Preprocessing Time:	O(m|Σ|)
// Matching Time:		O(n)
//		m: length of pattern string(search)
//		n: length of text string(base)

// T[i] ∈  tset

#include<iostream>
using namespace std;

string tset("abcdefghijklmnopqrstuvwxyz");
//string tset("abc");
#define M_MAX	10
#define N_TSET	26
int stab[M_MAX+1][N_TSET]={0};

void InitStateTab(string &P){
	int m=P.size();
	int i=0, j=0, q=0;
	char c;
	string pic;

	for(i=0; i<=m; i++){
	  j=0;
	  while((c=tset[j++])){
		q=(m<i+1)?(m):(i+1);
		pic=P.substr(0,i)+c;
		do{
			if(pic.compare(pic.size()-q,q,P.substr(0,q))==0)
			  break;
		}while(q--);
		stab[i][c-'a']=q;
	  }
	}
}

void FiniteAutoMatch(string &T, string &P){
	int n=T.size(), m=P.size();
	int q=0, i=0, sum=0;
	InitStateTab(P);
	
    for(i=0; i<n; i++){
		q=stab[q][T[i]-'a'];
		if(q==m){
		  cout << "match found at pos: " << i-m+1 << endl;
		  sum++;
		}
	}
	cout << "Total Matches: " << sum << endl << endl;
}

int main(){
	string sT, sP;
	int i, j;

	cout << "Demo of Finite State Machine String Match Algorithm!" << endl;
    cout << "please input the Text(base) string(limited in the 26 lower alphabet): " << endl;
	cin >> sT;
	cout << "please input the Pattern(search) string: ";
	cin >> sP;	
	cout << endl;
	FiniteAutoMatch(sT, sP);
}
