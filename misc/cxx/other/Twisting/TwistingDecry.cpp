//acm.zju.edu.cn ACM Problem Set 1006: Do The Untwist
//Coded by Zhi Guoping

#include <iostream>
#include <string>
using namespace std;

const int MaxCharNum=71;

int main()
{
    int k,n;
    string src,des;
    int srcc[MaxCharNum],desc[MaxCharNum];

    cout<<"Please Input Cipher Key(k) and Messange Want to Be Dncrypted:"<<endl;
    while(cin>>k && k!=0)
    {
        cin>>des;
        src=des;
        n=des.size();
        for(int i=0; i<n; i++)
          switch(des[i])
          {
              case '_':
                  desc[i]=0; break;
              case '.':
                  desc[i]=27; break;
              default:
                  desc[i]=des[i]-'a'+1;
          }

        for(int i=0; i<n; i++)
            srcc[k*i%n]=(desc[i]+i)%28;
        
        for(int i=0; i<n; i++)
          switch(srcc[i])
          {
              case 0:
                  src[i]='_'; break;
              case 27:
                  src[i]='.'; break;
              default:
                  src[i]=srcc[i]-1+'a';
          }

        cout<<"Crypted Text: "<<des<<endl;
        cout<<"Crypted Code: ";
            for(int i=0; i<n; i++)
              cout<<desc[i]<<' ';
        cout<<endl;
        cout<<"Source Code: ";
            for(int i=0; i<n; i++)
              cout<<srcc[i]<<' ';
        cout<<endl;
        cout<<"Source Text: "<<src<<endl;
    }

    return 0;
}

















