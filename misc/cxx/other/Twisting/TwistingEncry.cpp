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

    cout<<"Please Input Cipher Key(k) and Messange Want to Be Encrypted:"<<endl;
    while(cin>>k && k!=0)
    {
        cin>>src;
        des=src;
        n=src.size();
        for(int i=0; i<n; i++)
          switch(src[i])
          {
              case '_':
                  srcc[i]=0; break;
              case '.':
                  srcc[i]=27; break;
              default:
                  srcc[i]=src[i]-'a'+1;
          }

        for(int i=0; i<n; i++)
        {
            desc[i]=(srcc[k*i%n]-i)%28;
            if(desc[i]<0)
              desc[i]+=28;
        }
        for(int i=0; i<n; i++)
          switch(desc[i])
          {
              case 0:
                  des[i]='_'; break;
              case 27:
                  des[i]='.'; break;
              default:
                  des[i]=desc[i]-1+'a';
          }

        cout<<"Source Text: "<<src<<endl;
        cout<<"Source Code: ";
            for(int i=0; i<n; i++)
              cout<<srcc[i]<<' ';
        cout<<endl;
        cout<<"Crypted Code: ";
            for(int i=0; i<n; i++)
              cout<<desc[i]<<' ';
        cout<<endl;
        cout<<"Crypted Text: "<<des<<endl;
    }

    return 0;
}

















