// Red-Black Tree
// avg/worst time:  O(lgn) / O(n)
// avg/worst space: O(n) / O(n)


#include <iostream>
#include <queue>
using namespace std;

#define NULL_INT            -999999
#define RED                 0
#define BLK                 1
#define RBTINSERT_KEY_EXIST -1
#define RBTINSERT_NULL_KEY  -2
#define RBTROTATE_LSUB_NULL -1
#define RBTROTATE_RSUB_NULL -1

class RBNode{
public:
    int key=NULL_INT;
    int color;
    RBNode *p=nullptr, *lsub=nullptr, *rsub=nullptr;
    
    RBNode(int k):key(k){}
};

class RBTree{
public:
    RBNode *root=nullptr;
    void InorderWalk(RBNode *x);
    void PreorderWalk(RBNode *x);
    void PostorderWalk(RBNode *x);
    void BreadthWalk(RBNode *x);
    RBNode* Search(RBNode *x, int k);
    RBNode* SearchR(RBNode *x, int k);
    RBNode* Min(RBNode *x);
    RBNode* Max(RBNode *x);
    RBNode* Successor(RBNode *x);
    RBNode* Predecessor(RBNode *x);
    int Insert(RBNode *x);
    int InsertFixRB(RBNode *x);
    int Transplant(RBNode* u, RBNode* v);
    int Delete(RBNode *x);
    int DeleteFixRB(RBNode *x);
    int RotateL(RBNode *x);
    int RotateR(RBNode *x);
};

void RBTree::InorderWalk(RBNode *x){
    if(x==nullptr)
      return ;
    InorderWalk(x->lsub);
    if(x->color==BLK)
        cout << x->key << "(B)  ";
    else
        cout << x->key << "(R)  ";
    InorderWalk(x->rsub);
}

void RBTree::PreorderWalk(RBNode *x){
    if(x==nullptr)
      return ;
    if(x->color==BLK)
      cout << x->key << "(B)  ";
    else
      cout << x->key << "(R)  ";
    PreorderWalk(x->lsub);
    PreorderWalk(x->rsub);
}

void RBTree::PostorderWalk(RBNode *x){
    if(x==nullptr)
      return ;
    PostorderWalk(x->lsub);
    PostorderWalk(x->rsub);
    if(x->color==BLK)
      cout << x->key << "(B)  ";
    else
      cout << x->key << "(R)  ";
}

void RBTree::BreadthWalk(RBNode *x){
    if(x==nullptr)
      return ;
    queue<RBNode *> bsq;
    RBNode *p=root;
    bsq.push(p);
    while(!bsq.empty()){
        p=bsq.front();
        if(p->color==BLK)
          cout << p->key << "(B)  ";
        else
          cout << p->key << "(R)  ";
        bsq.pop();
        if(p->lsub!=0)
          bsq.push(p->lsub);
        if(p->rsub!=0)
          bsq.push(p->rsub);
    }
}

//recurrent
RBNode* RBTree::SearchR(RBNode *x, int k){
    if(k==x->key)
      return x;
    if(k<x->key && x->lsub!=nullptr)
      return SearchR(x->lsub, k);
    if(k>x->key && x->rsub!=nullptr)
      return SearchR(x->rsub, k);

    return nullptr;
}
//iterative
RBNode* RBTree::Search(RBNode *x, int k){
    while(k!=x->key){
        if(k<x->key && x->lsub!=nullptr)
          x=x->lsub;
        else if(k>x->key && x->rsub!=nullptr)
          x=x->rsub;
        else
          return nullptr;
    }
    return x;
}

RBNode* RBTree::Min(RBNode *x){
    while(x->lsub!=nullptr)
      x=x->lsub;
    return x;
}

RBNode* RBTree::Max(RBNode *x){
    while(x->rsub!=nullptr)
      x=x->rsub;
    return x;
}

RBNode* RBTree::Successor(RBNode *x){
//return the smallest element among which >x
    if(x->rsub!=nullptr)
      return Min(x->rsub);
    if(x->p==nullptr)
      return nullptr;
    
    RBNode *y=x->p;
    while(x==y->rsub){
        if(y->p==nullptr)
          return nullptr;
        x=y;
        y=y->p;
    }
    return y;
}

RBNode* RBTree::Predecessor(RBNode *x){
//return the biggest element among which <x
    if(x->lsub!=nullptr)
      return Max(x->lsub);
    if(x->p==nullptr)
      return nullptr;

    RBNode *y=x->p;
    while(x==y->lsub){
        if(y->p==nullptr)
          return nullptr;
        x=y;
        y=y->p;
    }
    return y;
}

int RBTree::Insert(RBNode *x){
    if(x->key==NULL_INT){
        cout << "ERR:Insert(): insert node without key value !!!" << endl;
        return RBTINSERT_NULL_KEY;
    }
    
    RBNode *y=root, *z=nullptr;
    while(y!=nullptr){
        z=y;
        if(x->key==y->key){
            cout << "ERR:Insert(): key already exists in tree !!!" << endl;
            return RBTINSERT_KEY_EXIST;
        }
        if(x->key<y->key)
          y=y->lsub;
        else
          y=y->rsub;
    }
    x->p=z;
    if(z==nullptr)
      root=x;
    else if(x->key<z->key)
      z->lsub=x;
    else
      z->rsub=x;
    x->color=RED;
    InsertFixRB(x);
    return 0;
}

int RBTree::InsertFixRB(RBNode *x){
    RBNode *y=nullptr;
    while(x->p!=nullptr && x->p->color==RED){
        if(x->p->p!=nullptr && x->p==x->p->p->lsub){
            y=x->p->p->rsub;  // y: x's uncle
            // x's uncle is RED
            if(y->color==RED){
                x->p->color=BLK; y->color=BLK; x->p->p->color=RED; x=x->p->p;
            }
            // x's uncle is BLK && x==x.p.rsub
            else if(x==x->p->rsub){
                x=x->p; RotateL(x);
            }
            // x's uncle is BLK && x==x.p.lsub
            else{
                x->p->color=BLK; x->p->p->color=RED; RotateR(x->p->p);
            }
        }
        else if(x->p->p!=nullptr && x->p==x->p->p->rsub){
            y=x->p->p->lsub;
            if(y->color==RED){
                x->p->color=BLK; y->color=BLK; x->p->p->color=RED; x=x->p->p;
            }
            else if(x==x->p->lsub){
                x=x->p; RotateR(x);
            }
            else{
                x->p->color=BLK; x->p->p->color=RED; RotateL(x->p->p);
            } 
        }
    }
    root->color=BLK;
    return 0;
}

int RBTree::Transplant(RBNode *u, RBNode *v){
// make u's parent link to v, u and u's sub tree is droped
    if(u->p==nullptr)
      root=v;
    else if(u==u->p->lsub)
      u->p->lsub=v;
    else
      u->p->rsub=v;
    
    if(v!=nullptr)
      v->p=u->p;
    
    return 0;
}

int RBTree::Delete(RBNode *x){
    RBNode *y=x, *z=nullptr;
    int ycolor=y->color;

    if(x->lsub==nullptr && x->rsub==nullptr)
      Transplant(x, static_cast<RBNode *>(nullptr));
    else if(x->lsub==nullptr && x->rsub!=nullptr){
        z=x->rsub; Transplant(x, x->rsub);
    }
    else if(x->lsub!=nullptr && x->rsub==nullptr){
        z=x->lsub; Transplant(x, x->lsub);
    }
    else{
       if(x->key%2){
            y=Min(x->rsub); ycolor=y->color; z=y->rsub;
            if(y->p==x)
                z->p=y;
            else{
                Transplant(y, y->rsub); y->rsub=x->rsub; y->rsub->p=y;
            }
            Transplant(x, y); y->lsub=x->lsub; y->lsub->p=y; y->color=x->color;
        }
        else{
            y=Max(x->lsub); ycolor=y->color; z=y->lsub;
            if(y->p==x)
                z->p=y;
            else{
                Transplant(y, y->lsub); y->lsub=x->lsub; y->lsub->p=y;
            }
            Transplant(x, y); y->rsub=x->rsub; y->rsub->p=y; y->color=x->color;
        }
    }
    if(ycolor==BLK)
      DeleteFixRB(z);
    return 0;
}

int RBTree::DeleteFixRB(RBNode *x){
    RBNode *y=nullptr;
    while(x!=root && x->color==BLK){
        if(x==x->p->lsub){
            y=x->p->rsub;
            if(y->color==RED){
                y->color=BLK; x->p->color=RED; RotateL(x->p); y=x->p->rsub;
            }
            if(y->lsub->color==BLK && y->rsub->color==BLK){
                y->color=RED; x=x->p;
            }
            else if(y->rsub->color==BLK){
              y->lsub->color=BLK; y->color=RED; RotateR(y); y=x->p->rsub;
            }
            else{
                y->color=x->p->color; x->p->color=BLK; y->rsub->color=BLK; RotateL(x->p); x=root;
            }
        }
        else{
            y=x->p->lsub;
            if(y->color==RED){
                y->color=BLK; x->p->color=RED; RotateR(x->p); y=x->p->lsub;
            }
            if(y->rsub->color==BLK && y->lsub->color==BLK){
                y->color=RED; x=x->p;
            }
            else if(y->lsub->color==BLK){
              y->rsub->color=BLK; y->color=RED; RotateL(y); y=x->p->lsub;
            }
            else{
                y->color=x->p->color; x->p->color=BLK; y->lsub->color=BLK; RotateR(x->p); x=root;
            }
        }
    }
    x->color=BLK;
    return 0;
}

int RBTree::RotateL(RBNode *x){
    if(x->rsub==nullptr){
        cout << "ERR:RotateL(): right subtree of x is null !!!" << endl;
        return RBTROTATE_RSUB_NULL;
    }

    RBNode *y=x->rsub;
// 1.turn y's lsub into x's rsub 
    if(y->lsub!=nullptr)
      y->lsub->p=x;
    x->rsub=y->lsub;
// 2.link x's parent and y
    y->p=x->p;
    if(x->p==nullptr)
      root=y;
    else if(x==x->p->lsub)
      x->p->lsub=y;
    else
      x->p->rsub=y;
// 3.turn x into y's lsub
    x->p=y;
    y->lsub=x;
    return 0;
}

int RBTree::RotateR(RBNode *x){
    if(x->lsub==nullptr){
        cout << "ERR:RotateR(): left subtree of x is null !!!" << endl;
        return RBTROTATE_LSUB_NULL;
    }
    RBNode *y=x->lsub;
// 1.turn y's rsub into x's lsub
    if(y->rsub!=nullptr)
      y->rsub->p=x;
    x->lsub=y->rsub;
// 2.link x's parent and y
    y->p=x->p;
    if(x->p==nullptr)
      root=y;
    else if(x==x->p->lsub)
      x->p->lsub=y;
    else
      x->p->rsub=y;
// 3.turn x into y's rsub
    x->p=y;
    y->rsub=x;
    return 0;
}


int main(){

    RBTree RBT;
    RBNode a(100),b(80),c(125),d(55),e(89),f(110),g(136),h(44),i(67),j(82),k(93),l(103),m(118),n(129),o(140);
    RBT.Insert(&a); RBT.Insert(&b); RBT.Insert(&c); RBT.Insert(&d); RBT.Insert(&e); RBT.Insert(&f); 
    RBT.Insert(&g); RBT.Insert(&h); RBT.Insert(&i); RBT.Insert(&j); RBT.Insert(&k); RBT.Insert(&l);
    RBT.Insert(&m); RBT.Insert(&n); RBT.Insert(&o);

    cout << endl; 
    cout << "Insert sequence:\t\t\t" << "100-80-125-55-89-110-136-44-67-82-93-103-118-129-140" << endl << endl;
    cout << "InorderWalk:\t\t\t\t"; RBT.InorderWalk(RBT.root); cout << endl;
    cout << "BreadthWalk:\t\t\t\t"; RBT.BreadthWalk(RBT.root); cout << endl << endl;

    RBT.Delete(&b);
    cout << "BreadthWalk after delete b(80):\t\t"; RBT.BreadthWalk(RBT.root); cout << endl;
    RBT.Delete(&c);
    cout << "BreadthWalk after delete c(125):\t"; RBT.BreadthWalk(RBT.root); cout << endl << endl;

    return 0;
}
