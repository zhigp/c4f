#include <iostream>
#include <algorithm>
#include <functional>
#include <list>
#include <array>
#include <cmath>
using namespace std;

// xmap:
// wall=1, start=A, end=B
//
//  0   0   0   0   0   0   0
//  0   0   0   1   0   0   0
//  0   A   0   1   0   B   0
//  0   0   0   1   0   0   0
//  0   0   0   0   0   0   0



// map[ROW][COL]
const int ROW=5;
const int COL=7;

const int startx=2;
const int starty=1;
const int endx=2;
const int endy=5;

class Node{
public:    
    int x, y, wall;
    int f, g, h;
    Node *p;

    bool operator()(Node &a){ return (a.x==x && a.y==y); }
    
    Node(){
        x=-1; y=-1; wall=0;
        f=0; g=0; h=0;
        p=NULL;
    }
};

class NodeEq{
public:
    Node _tmp;
    NodeEq(Node &tmp):_tmp(tmp){}
    bool operator()(Node &a){ return (a.x==_tmp.x && a.y==_tmp.y);}
};

class NodeCmp{
public:
    bool operator()(Node &a, Node &b){ return (a.f < b.f); }
};

int main(){

int i=0, j=0, found=3;
int curx=startx, cury=starty;
    
array<array<Node,COL>,ROW> xmap;
xmap[1][3].wall=1;
xmap[2][3].wall=1;
xmap[3][3].wall=1;

list<Node> open;
list<Node> close;
list<Node>::iterator it;
    
for(i=0; i<ROW; i++)
  for(j=0; j<COL; j++){
      xmap[i][j].x=i;
      xmap[i][j].y=j;
  }
   

while(!(curx==endx && cury==endy)){
    close.push_back(xmap[curx][cury]);
    if(!open.empty())
      open.pop_front();

    for(i=curx-1; i<=curx+1; i++)
      for(j=cury-1; j<=cury+1; j++)
        if(!xmap[i][j].wall && i>=0 && j>=0 && i<ROW && j<COL && !count_if(close.begin(), close.end(), NodeEq(xmap[i][j]))){
            
            if(!count_if(open.begin(), open.end(), NodeEq(xmap[i][j]))){
                xmap[i][j].p = &xmap[curx][cury];
                if(xmap[i][j].x==xmap[i][j].p->x || xmap[i][j].y==xmap[i][j].p->y)
                    xmap[i][j].g=xmap[i][j].p->g+10;
                else
                    xmap[i][j].g=xmap[i][j].p->g+14;
                xmap[i][j].h=(abs(xmap[i][j].x-endx)+abs(xmap[i][j].y-endy))*10;
                xmap[i][j].f=xmap[i][j].g+xmap[i][j].h;

                open.push_back(xmap[i][j]);
            }
            
            else{
                int tmpg=0;
                if(xmap[i][j].x==xmap[curx][cury].x || xmap[i][j].y==xmap[curx][cury].y)
                    tmpg=xmap[curx][cury].g+10;
                else
                    tmpg=xmap[curx][cury].g+14;

                if(tmpg < xmap[i][j].g){
                    xmap[i][j].p = &xmap[curx][cury];
            
                    if(xmap[i][j].x==xmap[i][j].p->x || xmap[i][j].y==xmap[i][j].p->y)
                        xmap[i][j].g=xmap[i][j].p->g+10;
                    else
                        xmap[i][j].g=xmap[i][j].p->g+14;
                    xmap[i][j].h=(abs(xmap[i][j].x-endx)+abs(xmap[i][j].y-endy))*10;
                    xmap[i][j].f=xmap[i][j].g+xmap[i][j].h;

                    it=find_if(open.begin(), open.end(), NodeEq(xmap[i][j]));
                    it->f=xmap[i][j].f;
                    it->g=xmap[i][j].g;
                    it->h=xmap[i][j].h;
                }
            }// end of else
        }// end of for-if

    open.sort(NodeCmp());
    curx=open.front().x;
    cury=open.front().y;

    
    
    for(it=open.begin(); it!=open.end(); it++)
        cout << it->x << ", " << it->y << "\t\tf:g:h: " << it->f << ":" << it->g << ":" << it->h << endl;
    cout << endl << "------------------------------------------" << endl;
}//end of while

Node *ptmp=&xmap[endx][endy];
while(ptmp){
    cout << "(" << ptmp->x << ", " << ptmp->y << ")" << endl;
    ptmp=ptmp->p;
}

}//end of main
